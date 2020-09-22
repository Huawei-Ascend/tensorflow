# Copyright 2019-2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import six
import tensorflow as tf
from tensorflow.python.framework import ops
from tensorflow.python.training import training_util
from tensorflow.python.ops import variable_scope
from tensorflow.python.ops import init_ops
from tensorflow.python.framework import dtypes
from tensorflow.core.framework import variable_pb2
from tensorflow.python.ops import resource_variable_ops

_NPU_RUNCONFIG = 'npu_runconfig'
_ITERATIONS_PER_LOOP_VAR = 'iterations_per_loop'
_LOOP_COND_VAR = 'loop_cond'
_CONST_ZERO = 'zero'
_CONST_ONE = 'one'

def check_not_none(value, name):
  """Checks whether `value` is not None."""
  if value is None:
    error_msg = '{} must not be None'.format(name)
    raise ValueError(error_msg)


def check_positive_integer(value, name):
  """Checks whether `value` is a positive integer."""
  if not isinstance(value, six.integer_types):
    error_msg = '{} must be int, got {}'.format(name, type(value))
    raise TypeError(error_msg)
  if value <= 0:
    error_msg = '{} must be positive, got {}'.format(name, value)
    raise ValueError(error_msg)


def check_nonnegative_integer(value, name):
  """Checks whether `value` is a nonnegative integer."""
  if not isinstance(value, six.integer_types):
    error_msg = '{} must be int, got {}'.format(name, type(value))
    raise TypeError(error_msg)

  if value < 0:
    error_msg = '{} must be nonnegative, got {}'.format(name, value)
    raise ValueError(error_msg)

def check_bool_type(value, name):
  """Checks whether `value` is True or false."""
  if not isinstance(value, bool):
    error_msg = '{} must be True or False, got {}'.format(name, value)
    raise TypeError(error_msg)

def convert_bool_to_int(value):
  """convert True/False to 1/0."""
  if value is True:
    return int(1)

  if value is False:
    return int(0)

  return int(-1)

def format_string(value, name):
  """fomat input to string type"""
  if value is None:
    return ""

  if not isinstance(value, six.string_types):
    error_msg = '{} must be string, got {}'.format(name, type(value))
    raise TypeError(error_msg)

  return str(value)

def check_profiling_options(self, profiling_options=[]):
  """Check profiling options .
  Args:
      profiling_options: Profiling options.
  Return:
      Valid options
  Raise:
      If profiling_options is null or option is not `training_trace` or `task_trace`, `op_trace`'.
  """

  error_mag = 'profiling options must be in `training_trace`, `task_trace` or `op_trace`'

  if len(profiling_options) == 0:
      raise ValueError(error_mag)

  profiling_types = ["training_trace", "task_trace", "op_trace"]
  for option in profiling_options :
      if option not in profiling_types:
          raise ValueError(error_mag)

  result = ":".join(profiling_options)
  return result

def register_func(var_name):
  ops.register_proto_function(
    '{}_{}'.format(_NPU_RUNCONFIG, var_name),
    proto_type=variable_pb2.VariableDef,
    to_proto=resource_variable_ops._to_proto_fn,
    from_proto=resource_variable_ops._from_proto_fn)

def create_or_get_var(var_name):
  graph = ops.get_default_graph()
  collection_name = '{}_{}'.format(_NPU_RUNCONFIG, var_name)
  iter_vars = graph.get_collection(collection_name)
  if len(iter_vars) == 1:
      return iter_vars[0]
  elif len(iter_vars) > 1:
      raise RuntimeError('Multiple var in collection.')
  ignore_existing = False
  if training_util.get_global_step() is None:
      ignore_existing = True
  with ops.colocate_with(training_util.get_global_step(), ignore_existing=ignore_existing):
      with variable_scope.variable_scope(_NPU_RUNCONFIG, reuse=variable_scope.AUTO_REUSE):
        return variable_scope.get_variable(
          var_name,
          initializer=init_ops.zeros_initializer(),
          shape=[],
          dtype=dtypes.int64,
          trainable=False,
          collections=[collection_name, ops.GraphKeys.LOCAL_VARIABLES],
          use_resource=True)

def set_iteration_per_loop(sess, train_op, iterations_per_loop=1):
  """
  Constructs a set_iteration_per_loop.
  Args:
  sess: A TensorFlow Session that has been created.
  train_op: An Operation that updates the variables
    or applies the specified gradients.
  iterations_per_loop： This is the number of train steps running in NPU
    system before returning to CPU host for each `Session.run`.

  Returns:
  An Operation named IterationOp that executes all its inputs.
  """
  if not isinstance(train_op, ops.Operation):
    raise ValueError(
          "The incoming 'train_op' type is '%s', "
          "and the need type is 'Operation'" %(train_op.dtype.name))
  check_positive_integer(iterations_per_loop, "iterations_per_loop")
  if iterations_per_loop == 1 :
    return train_op

  iterations_per_loop_var = create_or_get_var(_ITERATIONS_PER_LOOP_VAR)
  loop_cond_var = create_or_get_var(_LOOP_COND_VAR)
  const_zero = create_or_get_var(_CONST_ZERO)
  const_one = create_or_get_var(_CONST_ONE)

  iterations_per_loop_var.load(iterations_per_loop - 1, session=sess)
  loop_cond_var.load(0, session=sess)
  const_zero.load(0, session=sess)
  const_one.load(1, session=sess)

  # Add IterationOp denpend on train_op
  group_train_op = tf.group(train_op, name="IterationOp")

  return group_train_op

class IterationPerLoop():
  """
  An object provide two API to create and set iterations_per_loop
  """
  def create_iteration_per_loop_var(self, train_op):
    """
    Constructs a set_iteration_per_loop.
    Args:
        train_op: An Operation that updates the variables
          or applies the specified gradients.
        iterations_per_loop: This is the number of train steps running in NPU
          system before returning to CPU host for each `Session.run`.

    Returns:
    An Operation named IterationOp that executes all its inputs.
    """
    if not isinstance(train_op, ops.Operation):
      raise ValueError(
            "The incoming 'train_op' type is '%s', "
            "and the need type is 'Operation'" %(train_op.dtype.name))

    self._iterations_per_loop_var = create_or_get_var(_ITERATIONS_PER_LOOP_VAR)
    self._loop_cond_var = create_or_get_var(_LOOP_COND_VAR)
    self._const_zero = create_or_get_var(_CONST_ZERO)
    self._const_one = create_or_get_var(_CONST_ONE)

    # Add IterationOp denpend on train_op
    group_train_op = tf.group(train_op, name="IterationOp")

    return group_train_op

  def load_iteration_per_loop_var(self, sess, iterations_per_loop=1):
    """
    Constructs a load_iteration_per_loop_var.
    Args:
    sess: A TensorFlow Session that has been created.
    iterations_per_loop: This is the number of train steps running in NPU
      system before returning to CPU host for each `Session.run`.
    """
    check_positive_integer(iterations_per_loop, "iterations_per_loop")
    self._iterations_per_loop_var.load(iterations_per_loop - 1, session=sess)
    self._loop_cond_var.load(0, session=sess)
    self._const_zero.load(0, session=sess)
    self._const_one.load(1, session=sess)