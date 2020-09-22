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
from tensorflow.python.util import compat_internal
from tensorflow.python.platform import gfile

import os
import re
import json
from npu_bridge.estimator.npu import util
from tensorflow.python.estimator import run_config as run_config_lib
from tensorflow.distribute.experimental import ParameterServerStrategy
from tensorflow.contrib.distribute import DistributeConfig
from tensorflow.python.training import server_lib



class NPURunConfig(run_config_lib.RunConfig):
    """RunConfig with NPU support."""

    def __init__(self,
                 iterations_per_loop=1,
                 profiling_config=None,
                 model_dir=None,
                 tf_random_seed=None,
                 save_summary_steps=0,
                 save_checkpoints_steps=None,
                 save_checkpoints_secs=None,
                 session_config=None,
                 keep_checkpoint_max=5,
                 keep_checkpoint_every_n_hours=10000,
                 log_step_count_steps=100,
                 distribute=None,
                 enable_data_pre_proc=True,
                 precision_mode=None,
                 enable_reduce_precision=False,
                 variable_format_optimize=True,
                 mix_compile_mode=False,
                 hcom_parallel=False,
                 graph_memory_max_size=None,
                 variable_memory_max_size=None,
                 auto_tune_mode=None,
                 dump_config=None,
                 stream_max_parallel_num=None,
                 is_tailing_optimization=False,
                 horovod_mode = False,
                 graph_run_mode = 1,
                 op_debug_level = 0,
                 enable_scope_fusion_passes = None
                 ):
        """
        Constructs a NPUConfig.

        Args:
        iterations_per_loop: This is the number of train steps running in NPU
            system before returning to CPU host for each `Session.run`. This means
            global step is increased `iterations_per_loop` times in one `Session.run`.
            It is recommended to be set as number of global steps for next checkpoint.
        profiling_config: The profiling configuration.
        model_dir: Directory where model parameters, graph, etc are saved. If
            `PathLike` object, the path will be resolved. If `None`, will use a
            default value set by the Estimator.
        tf_random_seed: Random seed for TensorFlow initializers.
            Setting this value allows consistency between reruns.
        save_summary_steps: Save summaries every this many steps.
        save_checkpoints_steps: Save checkpoints every this many steps. Can not be
            specified with `save_checkpoints_secs`.
        save_checkpoints_secs: Save checkpoints every this many seconds. Can not
            be specified with `save_checkpoints_steps`. Defaults to 600 seconds if
            both `save_checkpoints_steps` and `save_checkpoints_secs` are not set
            in constructor.  If both `save_checkpoints_steps` and
            `save_checkpoints_secs` are None, then checkpoints are disabled.
        session_config: A ConfigProto used to set session parameters, or None.
        keep_checkpoint_max: The maximum number of recent checkpoint files to
            keep. As new files are created, older files are deleted. If None or 0,
            all checkpoint files are kept. Defaults to 5 (that is, the 5 most recent
            checkpoint files are kept.)
        keep_checkpoint_every_n_hours: Number of hours between each checkpoint
            to be saved. The default value of 10,000 hours effectively disables
            the feature.
        log_step_count_steps: The frequency, in number of global steps, that the
            global step/sec and the loss will be logged during training.
        enabel_data_pre_proc: This is the switch of data preprocess.
        precision_mode: enable or disable mix precision.
        variable_format_optimize: enable or disable variable format optimize while graph
            engineer optimize process.
        mix_compile_mode: This is the swith of mix_compile_mode. When the value is
            False, all graphs run on device. Otherwise, some graphs run on host.
        hcom_parallel: This is the switch of hcom parallel. When the value is True,
            hcom will execute with parallel mode. Otherwise, hcom will execute with
            serialize mode.
        graph_memory_max_size: The max size of ge graph memory size.
        variable_memory_max_size: The max size of ge variable memory size.
        auto_tune_mode: None, or `GA` ,or `RL` or `GA|RL`
        dump_config: The dump configuration.
        stream_max_parallel_num: Specify the degree of parallelism of the AICPU / AICORE engine
                                 to achieve parallel execution between AICPU / AICORE operators.
        """

        # Check iterations_per_loop.
        util.check_positive_integer(iterations_per_loop, "iterations_per_loop")
        if isinstance(mix_compile_mode, bool) == False:
            raise ValueError('"mix_compile_mode" type must be bool')
        if mix_compile_mode is True and iterations_per_loop != 1:
            raise ValueError(
                '"iterations_per_loop" must be 1 with "mix_compile_mode" is True')
        tf_config = json.loads(os.environ.get(run_config_lib._TF_CONFIG_ENV, '{}'))
        tmp_cluster_spec = server_lib.ClusterSpec(tf_config.get(run_config_lib._CLUSTER_KEY, {}))
        if ((tmp_cluster_spec and not isinstance(distribute, ParameterServerStrategy)) or
            (not tmp_cluster_spec and isinstance(distribute, ParameterServerStrategy))):
            raise ValueError('"cluster" and "distribute" must all be set in ps mode')
        if tmp_cluster_spec and mix_compile_mode is False:
            raise ValueError(
                '"mix_compile_mode" can only be True with "cluster" is set')

        self.iterations_per_loop = iterations_per_loop
        self.mix_compile_mode = mix_compile_mode
        self.enable_data_pre_proc = enable_data_pre_proc
        self.is_tailing_optimization = is_tailing_optimization
        if save_checkpoints_secs==None and save_checkpoints_steps==None :
            save_checkpoints_steps=100

        self._profiling_config = profiling_config

        # mix precision configuration
        self._precision_mode = precision_mode
        self._enable_reduce_precision = enable_reduce_precision
        self._variable_format_optimize = variable_format_optimize
        self._hcom_parallel = hcom_parallel
        self._graph_memory_max_size = graph_memory_max_size
        self._variable_memory_max_size = variable_memory_max_size

        self._auto_tune_mode = auto_tune_mode

        if dump_config is not None and  not isinstance(dump_config, DumpConfig):
            raise ValueError(
                '`dump_config` must be provided with type `DumpConfig`')
        self._dump_config = dump_config
        self._stream_max_parallel_num = stream_max_parallel_num

        if isinstance(horovod_mode, bool) == False:
            raise ValueError('"horovod_mode" type must be bool')
        self.horovod_mode = horovod_mode
        util.check_nonnegative_integer(graph_run_mode, "graph_run_mode")
        if graph_run_mode > 1:
            raise ValueError('"graph_run_mode" value must be 0 or 1')
        self.graph_run_mode = graph_run_mode
        self.op_debug_level = op_debug_level
        self.enable_scope_fusion_passes = enable_scope_fusion_passes
        experimental_distribute = None
        if tmp_cluster_spec and isinstance(distribute, ParameterServerStrategy):
            experimental_distribute = DistributeConfig(distribute, distribute, None)

        super(NPURunConfig, self).__init__(
            model_dir=model_dir,
            tf_random_seed=tf_random_seed,
            save_summary_steps=save_summary_steps,
            save_checkpoints_steps=save_checkpoints_steps,
            save_checkpoints_secs=save_checkpoints_secs,
            session_config=session_config,
            keep_checkpoint_max=keep_checkpoint_max,
            keep_checkpoint_every_n_hours=keep_checkpoint_every_n_hours,
            log_step_count_steps=log_step_count_steps,
            experimental_distribute=experimental_distribute)

class ProfilingConfig():
    """Profiling config with NPU support."""

    def __init__(self,
                 enable_profiling=False,
                 enable_options=[]):
        """
        Constructs a ProfilingConfig.
        Args:
            enable_profiling: Enable profiling, default is False.
            enable_options: Profiling options, list of `training_trace` or `task_trace`
                or `op_trace`.
        """

        self._enable_profiling = enable_profiling
        self._enable_options = enable_options


class DumpConfig():
    """Dump Config with NPU support."""
    def __init__(self,
                 enable_dump=False,
                 dump_path=None,
                 dump_step=None,
                 dump_mode="output",
                 enable_dump_debug=False,
                 dump_debug_mode="all"):
        """
        Constructs a DumpConfig.

        Args:
            enable_dump: Enable dump, default is False.
            dump_path: The dump path.
            dump_step: Specify step dump data. eg."0|5|10".
            dump_mode: Specify dump Op input or output or both.
            enable_dump_debug: Enable dump debug, default is False.
            dump_debug_mode: Debug dump mode, only support three kinds of mode(aicore_overflow, atomic_overflow or all).
        """
        self._enable_dump = enable_dump
        self._dump_path = dump_path
        self._dump_step = dump_step
        self._dump_mode = dump_mode
        self._enable_dump_debug = enable_dump_debug
        self._dump_debug_mode = dump_debug_mode
