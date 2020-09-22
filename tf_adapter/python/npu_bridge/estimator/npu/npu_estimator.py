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

import os
import tensorflow as tf
from tensorflow.core.protobuf import config_pb2
from tensorflow.core.protobuf import rewriter_config_pb2
from tensorflow.python.platform import tf_logging as logging
from tensorflow.python.framework import ops
from tensorflow.python.framework import constant_op
from tensorflow.python.ops import control_flow_ops
from tensorflow.python.ops import array_ops
from tensorflow.python.estimator import estimator as estimator_lib
from tensorflow.python.estimator import util as estimator_util
from tensorflow.python.estimator import model_fn as model_fn_lib
from tensorflow.python.training import session_run_hook
from tensorflow.python.util import function_utils
from tensorflow.python.util import tf_inspect

from npu_bridge.estimator.npu.npu_config import NPURunConfig
from npu_bridge.estimator.npu.npu_hook import *
from npu_bridge.estimator.npu.npu_common import NPUBasics
from npu_bridge.estimator import npu_ops

import six
from six.moves import queue as Queue
from six.moves import xrange
import collections
import copy
import os
import signal
import sys
import threading
import json
import random
import string

def no_check_override():
    class _Manager:
        def __init__(self):
            pass
        def __enter__(self):
            self.__orign = estimator_lib.Estimator._assert_members_are_not_overridden
            estimator_lib.Estimator._assert_members_are_not_overridden = lambda x : None
        def __exit__(self, exc_type, exc_val, exc_tb):
            estimator_lib.Estimator._assert_members_are_not_overridden = self.__orign
    return _Manager()


def _wrap_computation_in_while_loop(iterations_per_loop_var, op_fn):

  def computation(i):
    with ops.control_dependencies([op_fn]):
      return i + 1

  iterations = array_ops.identity(iterations_per_loop_var)
  return control_flow_ops.while_loop(
        lambda i: i < iterations,
        computation, [constant_op.constant(0)],
        parallel_iterations=1)

class _OutfeedHostCall(object):
    def __init__(self, channel_name):
        self._channel_name = str(channel_name)
        self._names = []
        # All of these are dictionaries of lists keyed on the name.
        self._host_fns = {}
        self._tensor_keys = collections.defaultdict(list)
        self._tensors = collections.defaultdict(list)
        self._tensor_dtypes = collections.defaultdict(list)
        self._tensor_shapes = collections.defaultdict(list)

    @staticmethod
    def validate(host_calls):
        """Validates the `eval_metrics` and `host_call` in `NPUEstimatorSpec`."""

        for name, host_call in host_calls.items():
            if not isinstance(host_call, (tuple, list)):
                raise ValueError('{} should be tuple or list'.format(name))
            if len(host_call) != 2:
                raise ValueError('{} should have two elements.'.format(name))
            if not callable(host_call[0]):
                raise TypeError('{}[0] should be callable.'.format(name))
            if not isinstance(host_call[1], (tuple, list, dict)):
                raise ValueError('{}[1] should be tuple or list, or dict.'.format(name))

            if isinstance(host_call[1], (tuple, list)):
                fullargspec = tf_inspect.getfullargspec(host_call[0])
                fn_args = function_utils.fn_args(host_call[0])
                # wrapped_hostcall_with_global_step uses varargs, so we allow that.
                if fullargspec.varargs is None and len(host_call[1]) != len(fn_args):
                    raise RuntimeError(
                        'In NPUEstimatorSpec.{}, length of tensors {} does not match '
                        'method args of the function, which takes {}.'.format(
                            name, len(host_call[1]), len(fn_args)))

    def create_npu_hostcall(self):
        """Sends the tensors through outfeed and runs the host_fn on CPU.

            The tensors are concatenated along dimension 0 to form a global tensor
            across all shards. The concatenated function is passed to the host_fn and
            executed on the first host.

            Returns:
              A dictionary mapping name to the return type of the host_call by that
              name.

            Raises:
              RuntimeError: If outfeed tensor is scalar.
            """
        if not self._names:
            return {}

        ret = {}
        # For each i, dequeue_ops[i] is a list containing the tensors from all
        # shards. This list is concatenated later.
        dequeue_ops = []
        tensor_dtypes = []
        tensor_shapes = []
        for name in self._names:
            for _ in self._tensors[name]:
                dequeue_ops.append([])
            for dtype in self._tensor_dtypes[name]:
                tensor_dtypes.append(dtype)
            for shape in self._tensor_shapes[name]:
                tensor_shapes.append(shape)

        outfeed_tensors = npu_ops.outfeed_dequeue_op(
            channel_name=self._channel_name,
            output_types=tensor_dtypes,
            output_shapes=tensor_shapes)

        # Deconstruct dequeue ops.
        outfeed_tensors_by_name = {}
        pos = 0
        for name in self._names:
            outfeed_tensors_by_name[name] = outfeed_tensors[pos:pos + len(self._tensors[name])]
            pos += len(self._tensors[name])

        for name in self._names:
            host_fn_tensors = outfeed_tensors_by_name[name]
            if self._tensor_keys[name] is not None:
                host_fn_tensors = dict(zip(self._tensor_keys[name], host_fn_tensors))
                try:
                    ret[name] = self._host_fns[name](**host_fn_tensors)
                except TypeError as e:
                    logging.warning(
                        'Exception while calling %s: %s. It is likely the tensors '
                        '(%s[1]) do not match the '
                        'function\'s arguments', name, e, name)
                    raise e
            else:
                ret[name] = self._host_fns[name](*host_fn_tensors)

        return ret

    def create_enqueue_op(self):
        """Create the op to enqueue the recorded host_calls.

        Returns:
          A list of enqueue ops, which is empty if there are no host calls.
        """
        if not self._names:
            return []

        tensors = []
        for name in self._names:
            tensors.extend(self._tensors[name])
        if len(tensors) == 0:
            return []
        return npu_ops.outfeed_enqueue_op(inputs=tensors, channel_name=self._channel_name)


    def record(self, host_calls): # ref def record(self, host_calls):@tpu_estimator.py+1618
        for name, host_call in host_calls.items():
            host_fn, tensor_list_or_dict = host_call
            self._names.append(name)
            self._host_fns[name] = host_fn

            if isinstance(tensor_list_or_dict, dict):
                for (key, tensor) in six.iteritems(tensor_list_or_dict):
                    self._tensor_keys[name].append(key)
                    self._tensors[name].append(tensor)
                    self._tensor_dtypes[name].append(tensor.dtype)
                    self._tensor_shapes[name].append(tensor.shape)
            else:
                # List or tuple.
                self._tensor_keys[name] = None
                for tensor in tensor_list_or_dict:
                    self._tensors[name].append(tensor)
                    self._tensor_dtypes[name].append(tensor.dtype)
                    self._tensor_shapes[name].append(tensor.shape)

class NPUEstimatorSpec(model_fn_lib.EstimatorSpec):
    """Ops and objects returned from a `model_fn` and passed to an `NPUEstimator`.

    `NPUEstimatorSpec` fully defines the model to be run by an `Estimator`.
    """
    def __new__(cls,
        mode,
        predictions=None,
        loss=None,
        train_op=None,
        eval_metric_ops=None,
        export_outputs=None,
        training_chief_hooks=None,
        training_hooks=None,
        scaffold=None,
        evaluation_hooks=None,
        prediction_hooks=None,
        host_call=None):

        """Creates a validated `EstimatorSpec` instance.

        Depending on the value of `mode`, different arguments are required. Namely

        * For `mode == ModeKeys.TRAIN`: required fields are `loss` and `train_op`.
        * For `mode == ModeKeys.EVAL`: required field is `loss`.
        * For `mode == ModeKeys.PREDICT`: required fields are `predictions`.

        model_fn can populate all arguments independent of mode. In this case, some
        arguments will be ignored by an `Estimator`. E.g. `train_op` will be
        ignored in eval and infer modes. Example:

        ```python
        def my_model_fn(features, labels, mode):
          predictions = ...
          loss = ...
          train_op = ...
          return tf.estimator.EstimatorSpec(
              mode=mode,
              predictions=predictions,
              loss=loss,
              train_op=train_op)
        ```

        Alternatively, model_fn can just populate the arguments appropriate to the
        given mode. Example:

        ```python
        def my_model_fn(features, labels, mode):
          if (mode == tf.estimator.ModeKeys.TRAIN or
              mode == tf.estimator.ModeKeys.EVAL):
            loss = ...
          else:
            loss = None
          if mode == tf.estimator.ModeKeys.TRAIN:
            train_op = ...
          else:
            train_op = None
          if mode == tf.estimator.ModeKeys.PREDICT:
            predictions = ...
          else:
            predictions = None

          return tf.estimator.EstimatorSpec(
              mode=mode,
              predictions=predictions,
              loss=loss,
              train_op=train_op)
        ```

        Args:
            mode: A `ModeKeys`. Specifies if this is training, evaluation or
                prediction.
            predictions: Predictions `Tensor` or dict of `Tensor`.
            loss: Training loss `Tensor`. Must be either scalar, or with shape `[1]`.
            train_op: Op for the training step.
            eval_metric_ops: Dict of metric results keyed by name.
                The values of the dict can be one of the following:
                (1) instance of `Metric` class.
                (2) Results of calling a metric function, namely a
                `(metric_tensor, update_op)` tuple. `metric_tensor` should be
                evaluated without any impact on state (typically is a pure computation
                results based on variables.). For example, it should not trigger the
                `update_op` or requires any input fetching.
            export_outputs: Describes the output signatures to be exported to
                `SavedModel` and used during serving.
                A dict `{name: output}` where:
                * name: An arbitrary name for this output.
                * output: an `ExportOutput` object such as `ClassificationOutput`,
                    `RegressionOutput`, or `PredictOutput`.
                Single-headed models only need to specify one entry in this dictionary.
                Multi-headed models should specify one entry for each head, one of
                which must be named using
                signature_constants.DEFAULT_SERVING_SIGNATURE_DEF_KEY.
                If no entry is provided, a default `PredictOutput` mapping to
                `predictions` will be created.
            training_chief_hooks: Iterable of `tf.train.SessionRunHook` objects to
                run on the chief worker during training.
            training_hooks: Iterable of `tf.train.SessionRunHook` objects to run
                on all workers during training.
            scaffold: A `tf.train.Scaffold` object that can be used to set
                initialization, saver, and more to be used in training.
            evaluation_hooks: Iterable of `tf.train.SessionRunHook` objects to
                run during evaluation.
            prediction_hooks: Iterable of `tf.train.SessionRunHook` objects to
                run during predictions.
            host_call:  A tuple of `func`, or a list of `tensor` or `dict`.Get	255
                summary infomation, and send to host every step. Only used if mode	256
                is `ModeKeys.TRAIN` or  `ModeKeys.EVAL`.

        Returns:
            A validated `EstimatorSpec` object.

        Raises:
            ValueError: If validation fails.
            TypeError: If any of the arguments is not the expected type.
        """
        host_calls = {}
        if host_call is not None:
            host_calls["host_call"] = host_call
        _OutfeedHostCall.validate(host_calls)
        spec = super(NPUEstimatorSpec, cls).__new__(
            cls,
            mode=mode,
            predictions=predictions,
            loss=loss,
            train_op=train_op,
            eval_metric_ops=eval_metric_ops,
            export_outputs=export_outputs,
            training_chief_hooks=training_chief_hooks,
            training_hooks=training_hooks,
            scaffold=scaffold,
            evaluation_hooks=evaluation_hooks,
            prediction_hooks=prediction_hooks,
        )
        spec._host_call = host_call
        return spec

class NPUEstimator(estimator_lib.Estimator):
    """Estimator with NPU support.

    NPUEstimator handles many of the details of running on NPU devices, such as
    replicating inputs and models for each core, and returning to host
    periodically to run hooks.
    """
    def __init__(self,
                 model_fn=None,
                 model_dir=None,
                 config=None,
                 params=None,
                 job_start_file='',
                 warm_start_from=None
                 ):
        """Constructs an `NPUEstimator` instance.

        Args:
            model_fn: Model function as required by `Estimator` which returns
                EstimatorSpec. `training_hooks`, 'evaluation_hooks',
                and `prediction_hooks` must not capure any NPU Tensor inside the model_fn.
            config: An `NPURunConfig` configuration object. Cannot be `None`.
            params: An optional `dict` of hyper parameters that will be passed into
                `input_fn` and `model_fn`.  Keys are names of parameters, values are
                basic python types..
            job_start_file: The path of the job start file. Cannot be `None`.
            warm_start_from: Optional string filepath to a checkpoint or SavedModel to
               warm-start from, or a `tf.estimator.WarmStartSettings`
               object to fully configure warm-starting.  If the string
               filepath is provided instead of a`tf.estimator.WarmStartSettings`,
               then all variables are warm-started, and it is assumed that vocabularies
               and `tf.Tensor` names are unchanged.
         """
        logging.info("NPUEstimator init...")

        if config is None or not isinstance(config, NPURunConfig):
            raise ValueError(
                '`config` must be provided with type `NPUConfigs`')

        # Verifies the model_fn signature according to Estimator framework.
        estimator_lib._verify_model_fn_args(model_fn, params)  # pylint: disable=protected-access

        # Load the graph optimizers.
        config = self.__load_graph_optimizers(config)

        # Init npu system: get task and device info from configuration file.
        self.__load_job_info(job_start_file)

        # Check modie dir in NPUEstimator and NPURunConfig
        model_dir = self.__check_model_dir(model_dir, config)

        # Wrap model_fn to adding npu sessionhooks.
        model_function = self.__augment_model_fn(model_fn, model_dir, config)

        # Get the checkpoint file.
        if not warm_start_from:
            restore_from = self.__job_info._local_checkpoint_dir
            # tf use restore_from variable, no need to check safety.
            if restore_from is None or restore_from == "":
                restore_from = os.getenv('RESTORE_FROM')
        else:
            restore_from = warm_start_from

        # Passing non-None params as wrapped model_fn use it.
        params = params or {}
        with no_check_override():
            super(NPUEstimator, self).__init__(
                model_fn=model_function,
                model_dir=model_dir,
                config=config,
                params=params,
                warm_start_from=restore_from)

    def __augment_model_fn(self, model_fn, model_dir, config):
        """Returns a new model_fn, which wraps the NPU support."""
        def _model_fn(features, labels, mode, params):
            """A Estimator `model_fn` for NPUEstimator."""
            model_fn_args = function_utils.fn_args(model_fn)
            kwargs = {}
            if 'labels' in model_fn_args:
                kwargs['labels'] = labels
            if 'mode' in model_fn_args:
                kwargs['mode'] = mode
            if 'params' in model_fn_args:
                kwargs['params'] = params
            if 'config' in model_fn_args:
                kwargs['config'] = config
            estimator_spec = model_fn(features=features, **kwargs)

            """
            add hooks:
                NPUInitHook: for all mode, NPUInitHook should be the first session hook
                NPUShutDownHook: for all mode, NPUShutDownHook should be the first session hook
                NPUBroadcastGlobalVariablesHook: train
                NPUCheckpointSaverHook:train
            """
            npu_hooks = []

            if mode == model_fn_lib.ModeKeys.TRAIN:
                if not isinstance(estimator_spec, NPUEstimatorSpec) and not isinstance(estimator_spec, model_fn_lib.EstimatorSpec):
                    raise RuntimeError('estimator_spec used by NPU train must have type '
                        '`NPUEstimatorSpec` or `EstimatorSpec`. Got {}'.format(type(estimator_spec)))
                # 1. NPUBroadcastGlobalVariablesHook
                rank_size = os.getenv('RANK_SIZE')
                if rank_size != None and rank_size.isdigit() and int(rank_size) > 1 and not config.horovod_mode:
                    npu_hooks.append(NPUBroadcastGlobalVariablesHook(self.__device_info._root_rank, self.__device_info._index))

                # 2. NPUCheckpointSaverHook
                if config.save_checkpoints_steps or config.save_checkpoints_secs:
                    npu_hooks.append(NPUCheckpointSaverHook(
                        checkpoint_dir=model_dir,
                        save_secs=config.save_checkpoints_secs,
                        save_steps=config.save_checkpoints_steps))

                if isinstance(estimator_spec, NPUEstimatorSpec):
                    if estimator_spec._host_call is not None:
                        host_call = _OutfeedHostCall(mode)
                        host_call.record({"host_call" : estimator_spec._host_call})
                        # add outfeed enqueue op
                        loss, train_op = estimator_spec.loss, estimator_spec.train_op
                        with ops.control_dependencies([train_op]):
                            host_call_outfeed_op = host_call.create_enqueue_op()
                            with ops.control_dependencies([host_call_outfeed_op]):
                                loss = array_ops.identity(loss)
                                estimator_spec = estimator_spec._replace(loss=loss)
                        # add outfeed dnqueue op
                        host_call_ops = host_call.create_npu_hostcall()
                        npu_hooks.append(NPUInfeedOutfeedSessionHook(host_call_ops, mode))
                    npu_hooks.append(NPULogOutfeedSessionHook(sys.stderr))

                # 3. set iterations per loop hook
                if config.iterations_per_loop > 1 :
                    npu_hooks.append(SetIterationsVarHook(config.iterations_per_loop))
                    train_op = tf.group(estimator_spec.train_op, name="IterationOp")
                    estimator_spec = estimator_spec._replace(train_op=train_op)

                train_hooks = estimator_spec.training_hooks
                train_hooks = list(train_hooks or [])
                new_train_hooks = npu_hooks + train_hooks

                estimator_spec = estimator_spec._replace(training_hooks=tuple(new_train_hooks))

            elif mode == model_fn_lib.ModeKeys.EVAL:
                if not isinstance(estimator_spec, NPUEstimatorSpec) and not isinstance(estimator_spec, model_fn_lib.EstimatorSpec):
                    raise RuntimeError('estimator_spec used by NPU evaluate must have type '
                        '`NPUEstimatorSpec` or `EstimatorSpec`. Got {}'.format(type(estimator_spec)))
                if isinstance(estimator_spec, NPUEstimatorSpec):
                    if estimator_spec._host_call is not None:
                        host_call = _OutfeedHostCall(mode)
                        host_call.record({"host_call" : estimator_spec._host_call})
                        # add outfeed enqueue op
                        loss, train_op = estimator_spec.loss, estimator_spec.train_op
                        with ops.control_dependencies([loss]):
                            host_call_outfeed_op = host_call.create_enqueue_op()
                            with ops.control_dependencies([host_call_outfeed_op]):
                                loss = array_ops.identity(loss)
                                estimator_spec = estimator_spec._replace(loss=loss)
                        # add outfeed dnqueue op
                        host_call_ops = host_call.create_npu_hostcall()
                        npu_hooks.append(NPUInfeedOutfeedSessionHook(host_call_ops, mode))
                    npu_hooks.append(NPULogOutfeedSessionHook(sys.stderr))
                if len(npu_hooks) > 0:
                    evaluation_hooks = estimator_spec.evaluation_hooks
                    evaluation_hooks = list(evaluation_hooks or [])
                    new_evaluation_hooks = npu_hooks + evaluation_hooks
                    estimator_spec = estimator_spec._replace(evaluation_hooks=tuple(new_evaluation_hooks))

            elif mode == model_fn_lib.ModeKeys.PREDICT:
                if len(npu_hooks) > 0:
                    prediction_hooks = estimator_spec.prediction_hooks
                    prediction_hooks = list(prediction_hooks or [])
                    new_prediction_hooks = npu_hooks + prediction_hooks

                    estimator_spec = estimator_spec._replace(prediction_hooks=tuple(new_prediction_hooks))
            return estimator_spec

        return _model_fn

    def __check_profiling_options(self, profiling_options=[]):
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

    def __load_profiling_options(self, config, custom_op):
        """Load profiling config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Custom optimizers.
        """
        if config._profiling_config is None:
            """
            there is no profiling config in user's python script,
            then use the default profiling configuration
            """
            custom_op.parameter_map["profiling_mode"].b = False
            profiling_options = "training_trace"
            custom_op.parameter_map["profiling_options"].s = tf.compat.as_bytes(profiling_options)

        else:
            if config._profiling_config._enable_profiling:
                # User enable profiling
                custom_op.parameter_map["profiling_mode"].b = True
                # check profiling ,and get valid options
                profiling_options = self.__check_profiling_options(config._profiling_config._enable_options)
                custom_op.parameter_map["profiling_options"].s = tf.compat.as_bytes(profiling_options)
            else:
                # User disable profiling,
                custom_op.parameter_map["profiling_mode"].b = False
                profiling_options = "training_trace"
                custom_op.parameter_map["profiling_options"].s = tf.compat.as_bytes(profiling_options)

    def __load_mix_precision(self, config, custom_op):
        """Load mix precision config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Custom optimizers.
        """
        if config._precision_mode is not None:
            custom_op.parameter_map["precision_mode"].s = tf.compat.as_bytes(config._precision_mode)
        custom_op.parameter_map["enable_reduce_precision"].b = config._enable_reduce_precision

    def __load__variable_format_optimize(self, config, custom_op):
        """Load variable acceleration config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Customer optimizers.
        """

        custom_op.parameter_map["variable_format_optimize"].b = config._variable_format_optimize

    def __load_auto_tune_config(self, config, custom_op):
        """Load auto tune config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Custom optimizers.
        """

        if config._auto_tune_mode is not None:
            custom_op.parameter_map["auto_tune_mode"].s = tf.compat.as_bytes(config._auto_tune_mode)

    def __load_dump_config(self, config, custom_op):
        """Load dump config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Customer optimizers.
        """
        if config._dump_config is None:
            """
            there is no dump config in user's python script,
            then use the default dump configuration
            """
            custom_op.parameter_map["enable_dump"].b = False
            custom_op.parameter_map["enable_dump_debug"].b = False

        else:
            custom_op.parameter_map["enable_dump"].b = config._dump_config._enable_dump
            custom_op.parameter_map["enable_dump_debug"].b = config._dump_config._enable_dump_debug
            if config._dump_config._dump_path is not None:
                custom_op.parameter_map["dump_path"].s = tf.compat.as_bytes(config._dump_config._dump_path)
            if config._dump_config._dump_step is not None:
                custom_op.parameter_map["dump_step"].s = tf.compat.as_bytes(config._dump_config._dump_step)
            if config._dump_config._dump_mode is not None:
                custom_op.parameter_map["dump_mode"].s = tf.compat.as_bytes(config._dump_config._dump_mode)
            if config._dump_config._dump_mode is not None:
                custom_op.parameter_map["dump_debug_mode"].s = tf.compat.as_bytes(config._dump_config._dump_debug_mode)

    def __load_stream_max_config(self, config, custom_op):
        """Load stream_max_parallel_num config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Customer optimizers.
        """
        if config._stream_max_parallel_num is not None:
            custom_op.parameter_map["stream_max_parallel_num"].s = tf.compat.as_bytes(config._stream_max_parallel_num)

    def __load_ps_mode_config(self, config, custom_op):
        """Load stream_max_parallel_num config ,and add to custom_optimizers
        Args:
            config: NPURunConfig.
            custom_op: Customer optimizers.
        """
        config_info = json.loads(os.environ.get('TF_CONFIG') or '{}')

        # Set task_type and task_id if the TF_CONFIG environment variable is
        # present.  Otherwise, use the respective default (None / 0).
        task_env = config_info.get('task', {})
        task_type = task_env.get('type', None)
        task_index = task_env.get('index', 0)
        if task_type:
            custom_op.parameter_map["job"].s = tf.compat.as_bytes(task_type)
            custom_op.parameter_map["task_index"].i = int(task_index)
        else:
            custom_op.parameter_map["job"].s = tf.compat.as_bytes('localhost')
            custom_op.parameter_map["task_index"].i = 0

    def __load_graph_optimizers(self, config):
        """Change the session config and load the graph optimizers:
        GradFusionOptimizer and OMPartitionSubgraphsPass."""

        if config.session_config is None:
            config = config.replace(session_config=tf.ConfigProto())

        config.session_config.graph_options.rewrite_options.optimizers.extend(["pruning",
                                                                               "function",
                                                                               "constfold",
                                                                               "shape",
                                                                               "arithmetic",
                                                                               "loop",
                                                                               "dependency",
                                                                               "layout",
                                                                               "memory",
                                                                               "GradFusionOptimizer"])
        # config set
        custom_op = config.session_config.graph_options.rewrite_options.custom_optimizers.add()
        custom_op.name = "NpuOptimizer"
        custom_op.parameter_map["enable_data_pre_proc"].b = config.enable_data_pre_proc
        custom_op.parameter_map["mix_compile_mode"].b = config.mix_compile_mode
        custom_op.parameter_map["use_off_line"].b = True
        custom_op.parameter_map["iterations_per_loop"].i = config.iterations_per_loop
        custom_op.parameter_map["is_tailing_optimization"].b = config.is_tailing_optimization
        custom_op.parameter_map["min_group_size"].b = 1
        custom_op.parameter_map["hcom_parallel"].b = config._hcom_parallel
        if config._graph_memory_max_size is not None:
            custom_op.parameter_map["graph_memory_max_size"].s = tf.compat.as_bytes(str(config._graph_memory_max_size))
        if config._variable_memory_max_size is not None:
            custom_op.parameter_map["variable_memory_max_size"].s = tf.compat.as_bytes(str(config._variable_memory_max_size))
        custom_op.parameter_map["graph_run_mode"].i = config.graph_run_mode
        custom_op.parameter_map["op_debug_level"].i = config.op_debug_level
        if config.enable_scope_fusion_passes is not None:
            custom_op.parameter_map["enable_scope_fusion_passes"].s = tf.compat.as_bytes(config.enable_scope_fusion_passes)

        # add profiling options to custom_op
        self.__load_profiling_options(config, custom_op)

        # add mix precision to custom_op
        self.__load_mix_precision(config, custom_op)

        # add variable acceleration to custom_op
        self.__load__variable_format_optimize(config, custom_op)

        # add auto une config to custom_op
        self.__load_auto_tune_config(config, custom_op)

        # add dump config to custom_op
        self.__load_dump_config(config, custom_op)

        # add stream_max_parallel to custom_op
        self.__load_stream_max_config(config, custom_op)

        self.__load_ps_mode_config(config, custom_op)

        return config


    def __load_job_info(self, job_start_file):
        """Parse the file from the CSA."""
        # Read the job config file.
        basic = NPUBasics(job_start_file)
        if basic.jobinfo is None:
            return False

        # Get Device info from config file.
        self.__job_info = basic.jobinfo
        self.__device_info = basic.jobinfo._device_info
        return True

    def __check_model_dir(self, model_dir, config):
        """Check model dir. If model dir is None, create temp dir.

        Returns:
        Model dir.

        Raises:
        ValueError: If model_dir of NPUEstimator is different with model_dir of NPURunConfig.
        """
        if (model_dir is not None) and (config.model_dir is not None):
            if model_dir != config.model_dir:
                raise ValueError(
                    'model_dir are set both in NPUEstimator and NPURunConfig, but with '
                    "different values. In constructor: '{}', in NPURunConfig: "
                    "'{}' ".format(model_dir, config.model_dir))

        model_dir = model_dir or config.model_dir
        if model_dir is None:
            while True:
                model_dir = "model_dir_" + \
                            "".join(random.sample(string.ascii_letters + string.digits, 10))
                cwd = os.getcwd()
                model_dir = os.path.join(cwd, model_dir)
                if not tf.io.gfile.exists(model_dir):
                    break
            logging.warning('Using temporary folder as model directory: %s', model_dir)
            tf.io.gfile.mkdir(model_dir)
        return model_dir