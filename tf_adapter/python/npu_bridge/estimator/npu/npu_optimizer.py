"""
Optimizer that implements distributed gradient reduction for NPU.
"""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import tensorflow as tf
from tensorflow.python.eager import context
from tensorflow.python.ops import control_flow_ops
from tensorflow.python.ops import gen_control_flow_ops
from tensorflow.python.ops import gen_math_ops
from tensorflow.python.ops import math_ops
from tensorflow.python.training import optimizer
from npu_bridge.hccl import hccl_ops
from npu_bridge.estimator import npu_ops

from npu_bridge.helper import helper
gen_npu_ops = helper.get_gen_ops();

from tensorflow.python.platform import tf_logging as logging
from npu_bridge.estimator.npu.npu_common import NPUBasics

def allreduce(tensor, var, average=True):
    """
    Perform an allreduce on a tf.Tensor or tf.IndexedSlices.

    Arguments:
        tensor: tf.Tensor, tf.Variable, or tf.IndexedSlices to reduce.
        The shape of the input must be identical across all ranks.
        average: If True, computes the average over all ranks.
                 Otherwise, computes the sum over all ranks.

    This function performs a bandwidth-optimal ring allreduce on the input
    tensor. If the input is an tf.IndexedSlices, the function instead does an
    allgather on the values and the indices, effectively doing an allreduce on
    the represented tensor.
    """
    basic = NPUBasics("")
    size = basic.size()
    # the tensor is the instance of tf.IndexedSlices
    if isinstance(tensor, tf.IndexedSlices):
        # For IndexedSlices, do two allgathers intead of an allreduce.
        logging.debug("HcomAllgather...")
        values=hccl_ops.allgather(tensor.values, size)
        indices=hccl_ops.allgather(tensor.indices, size)

        if values is  None:
            raise ValueError('the result of tf.HcomAllgather([tensor.values]) is empty')
        if indices is None:
            raise ValueError('the result of tf.HcomAllgather([tensor.indices]) is empty')

        # To make this operation into an average, divide all gathered values by the size.
        rank_size = tf.cast(size, tensor.values.dtype)
        new_values = tf.div(values, rank_size) if average else values

        return tf.IndexedSlices(new_values, indices,dense_shape=tensor.dense_shape)

    else:
        logging.debug("HcomAllReduce...")
        summed_tensor=hccl_ops.allreduce(tensor,"sum")

        if summed_tensor is None:# and summed_tensor:
            raise ValueError('the result of tf.DavinciAllreduce([tensor]) is empty')

        rank_size = tf.cast(size, dtype=tensor.dtype)
        new_tensor = tf.div(summed_tensor, rank_size) if average else summed_tensor

        return new_tensor

class NPUOptimizer(optimizer.Optimizer):
  """An optimizer that wraps another tf.Optimizer that can using an allreduce to
  average gradient values before applying gradients to model weights when
  'is_distributed' is True. And applies loss scaling in backprop when 'is_loss_scale'
  is True. 'is_tailing_optimization' is used to determine whether to enable
  communication tailing optimization to improve training performance,
  this setting only takes effect when 'is_distributed' is True. """

  def __init__(self, opt, loss_scale_manager=None, is_distributed=False, is_loss_scale=False,
                is_tailing_optimization=False, name=None):
    """Construct a loss scaling optimizer.

    Args:
        opt: The actual optimizer that will be used to compute and apply the
            gradients. Must be an implementation of the
            `tf.compat.v1.train.Optimizer` interface.
        loss_scale_manager: A LossScaleManager object.
    """
    self._opt = opt
    self._loss_scale_manager = loss_scale_manager
    self._float_status = tf.constant([0.0], dtype=tf.float32)
    self._is_distributed = is_distributed
    self._is_loss_scale = is_loss_scale
    self._is_tailing_optimization = is_tailing_optimization
    if is_loss_scale and loss_scale_manager is None:
      raise ValueError("is_loss_scale is True, loss_scale_manager can not be None")
    if name is None:
      name = "NPUOptimizer{}".format(type(opt).__name__)
    self._name = name

  def compute_gradients(self,
                        loss,
                        var_list=None,
                        gate_gradients=optimizer.Optimizer.GATE_OP,
                        aggregation_method=None,
                        colocate_gradients_with_ops=False,
                        grad_loss=None):
    """Compute gradients. See base class `tf.compat.v1.train.Optimizer`."""
    if self._is_loss_scale:
      loss_scale = self._loss_scale_manager.get_loss_scale()
      if context.executing_eagerly():

        def scaled_loss():
          loss_val = loss()
          return loss_val * math_ops.cast(loss_scale, loss_val.dtype.base_dtype)
      else:
        if callable(loss):
          loss_val = loss()
        else:
          loss_val = loss
        scaled_loss = loss_val * math_ops.cast(loss_scale,
                                              loss_val.dtype.base_dtype)

      self._float_status = gen_npu_ops.npu_alloc_float_status()
    else :
      scaled_loss = loss

    logging.debug("compute_gradients...")
    gradients = self._opt.compute_gradients(
        scaled_loss,
        var_list=var_list,
        gate_gradients=gate_gradients,
        aggregation_method=aggregation_method,
        colocate_gradients_with_ops=colocate_gradients_with_ops,
        grad_loss=grad_loss)
    if not self._is_distributed:
      if self._is_loss_scale:
        return self._down_scale(gradients, loss_scale)
      else:
        return gradients

    averaged_gradients = []
    grads = []
    with tf.name_scope(self._name + "_Allreduce"):
        for grad, var in gradients:
            grads.append(grad)
            if self._is_loss_scale and (len(grads) == len(gradients)) and self._is_tailing_optimization:
              self._reduce_all(grads)
              with tf.get_default_graph().control_dependencies([self._is_overall_finite]):
                avg_grad = allreduce(grad, var, True) if grad is not None else None
                averaged_gradients.append((avg_grad, var))
            else:
              avg_grad = allreduce(grad, var, True) if grad is not None else None
              averaged_gradients.append((avg_grad, var))
    if self._is_loss_scale:
      return self._down_scale(averaged_gradients, loss_scale)
    else:
      return averaged_gradients

  def apply_gradients(self, grads_and_vars, global_step=None, name=None):
    """Apply gradients. See base class `tf.compat.v1.train.Optimizer`."""

    if self._is_loss_scale:
      if not self._is_tailing_optimization:
        grads = [g for (g, _) in grads_and_vars]
        self._reduce_all(grads)

      def true_apply_gradients_fn():
        def true_apply_gradients(grads_and_vars, global_step=None, name=None):
            return self._opt.apply_gradients(grads_and_vars, global_step, name)

        return true_apply_gradients(grads_and_vars, global_step, name)

      update_vars = control_flow_ops.cond(self._is_overall_finite,
                                          true_apply_gradients_fn,
                                          gen_control_flow_ops.no_op)

      # Potentially adjust gradient scale in case of finite gradients.
      return control_flow_ops.group(
          update_vars,
          self._loss_scale_manager.update_loss_scale(self._is_overall_finite))
    else:
        return self._opt.apply_gradients(grads_and_vars, global_step, name)

  def _down_scale(self, grads_vars, loss_scale):
    # Down scale grads by the loss_scale.
    gv = []
    inv_loss_scale = gen_math_ops.reciprocal(loss_scale)
    for g, v in grads_vars:
      if g is not None:
        gv.append((g * math_ops.cast(inv_loss_scale, g.dtype.base_dtype), v))
      else:
        gv.append((g, v))
    return gv

  def _reduce_all(self, grads):
    with tf.get_default_graph().control_dependencies(grads):
      local_float_status = gen_npu_ops.npu_get_float_status(self._float_status)
      cleared_float_status = gen_npu_ops.npu_clear_float_status(local_float_status)

    if self._is_distributed:
      with tf.get_default_graph().control_dependencies([local_float_status]):
        aggregated_float_status = hccl_ops.allreduce([self._float_status], "sum", fusion=0)
        self._is_overall_finite = math_ops.reduce_all(tf.equal(aggregated_float_status,
                                                      cleared_float_status))
    else:
      self._is_overall_finite = math_ops.reduce_all(tf.equal(self._float_status,
                                                      cleared_float_status))

  def get_slot(self, *args, **kwargs):
    """Calls this same method on the underlying optimizer."""
    return self._opt.get_slot(*args, **kwargs)

  def get_slot_names(self, *args, **kwargs):
    """Calls this same method on the underlying optimizer."""
    return self._opt.get_slot_names(*args, **kwargs)

  def variables(self, *args, **kwargs):
    """Calls this same method on the underlying optimizer."""
    return self._opt.variables(*args, **kwargs)


class NPUDistributedOptimizer(tf.train.Optimizer):
    """
    An optimizer that wraps another tf.Optimizer, using an allreduce to
    average gradient values before applying gradients to model weights.
    """

    def __init__(self, optimizer, name=None):
        """
        Construct a new DistributedOptimizer, which uses another optimizer
        under the hood for computing single-process gradient values and
        applying gradient updates after the gradient values have been averaged
        across all the hcom ranks.

        Args:
            optimizer: Optimizer to use for computing gradients and applying updates.
            name: Optional name prefix for the operations created when applying
                gradients. Defaults to "Distributed" followed by the provided
                optimizer type.
                See Optimizer.__init__ for more info.
        """
        if name is None:
            name = "Distributed{}".format(type(optimizer).__name__)
        self._optimizer = optimizer
        super(NPUDistributedOptimizer, self).__init__(name=name, use_locking=False)

    def compute_gradients(self, *args, **kwargs):
        """
        Compute gradients of all trainable variables.
        See Optimizer.compute_gradients() for more info.
        In DistributedOptimizer, compute_gradients() is overriden to also
        allreduce the gradients before returning them.
        """
        logging.debug("compute_gradients...")
        gradients = self._optimizer.compute_gradients(*args, **kwargs)
        rank_size = os.getenv('RANK_SIZE')
        if rank_size == None or int(rank_size) <= 1:
            return gradients

        averaged_gradients = []
        with tf.name_scope(self._name + "_Allreduce"):
            for grad, var in gradients:
                avg_grad = allreduce(grad, var, True) if grad is not None else None
                averaged_gradients.append((avg_grad, var))
        return averaged_gradients

    def apply_gradients(self, *args, **kwargs):
        """Calls this same method on the underlying optimizer."""
        return self._optimizer.apply_gradients(*args, **kwargs)

    def get_slot(self, *args, **kwargs):
        """Calls this same method on the underlying optimizer."""
        return self._optimizer.get_slot(*args, **kwargs)

    def get_slot_names(self, *args, **kwargs):
        """Calls this same method on the underlying optimizer."""
        return self._optimizer.get_slot_names(*args, **kwargs)

    def variables(self, *args, **kwargs):
        """Calls this same method on the underlying optimizer."""
        return self._optimizer.variables(*args, **kwargs)
