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
from npu_bridge import tf_adapter
from npu_bridge.estimator.npu import util
import json
import os

__auto_tune_mode = str(tf_adapter.AUTO_TUNE_MODE)
__op_debug_level = str(tf_adapter.OP_DEBUG_LEVEL)
__option_exec_enable_scope_fusion_passes = str(tf_adapter.OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES)
__option_exec_profiling_mode = str(tf_adapter.OPTION_EXEC_PROFILING_MODE)
__option_exec_profiling_options = str(tf_adapter.OPTION_EXEC_PROFILING_OPTIONS)
__option_graph_run_mode = str(tf_adapter.OPTION_GRAPH_RUN_MODE)
__option_exec_option_exec_hccl_flag = str(tf_adapter.OPTION_EXEC_HCCL_FLAG)

def npu_global_init(graph_run_mode = 1,
                    op_debug_level = 0,
                    is_tailing_optimization = False,
                    enable_profiling = False,
                    enable_options = "training_trace",
                    auto_tune_mode = None,
                    precision_mode = None,
                    enable_scope_fusion_passes = None,
                    ):

    util.check_nonnegative_integer(graph_run_mode, "graph_run_mode")
    if graph_run_mode > 1:
        raise ValueError('"graph_run_mode" value must be 0 or 1')

    util.check_nonnegative_integer(op_debug_level, "op_debug_level")
    util.check_bool_type(is_tailing_optimization, "is_tailing_optimization")
    util.check_bool_type(enable_profiling, "enable_profiling")
    graph_run_mode = str(graph_run_mode)
    op_debug_level = str(op_debug_level)
    is_tailing_optimization = str(util.convert_bool_to_int(is_tailing_optimization))
    enable_profiling = str(util.convert_bool_to_int(enable_profiling))

    init={}
    init[__option_graph_run_mode] = graph_run_mode
    init[__op_debug_level] = op_debug_level
    init["ge.exec.isTailingOptimization"] = is_tailing_optimization
    init[__option_exec_profiling_mode] = enable_profiling

    if enable_profiling is True:
        init[__option_exec_profiling_options] = str(util.check_profiling_options(enable_options))
    else:
        init[__option_exec_profiling_options] = str("training_trace")

    if auto_tune_mode is not None:
        init[__auto_tune_mode] = str(auto_tune_mode)

    if precision_mode is not None:
        init["ge.exec.precision_mode"] = str(precision_mode)

    if enable_scope_fusion_passes is not None:
        init[__option_exec_enable_scope_fusion_passes] = str(enable_scope_fusion_passes)

    config_info = json.loads(os.environ.get('TF_CONFIG') or '{}')
    task_env = config_info.get('task', {})
    task_type = task_env.get('type', None)
    exec_hccl_flag = 1
    if task_type == 'evaluator':
        exec_hccl_flag = 0
    init[__option_exec_option_exec_hccl_flag] = str(exec_hccl_flag)

    init_options=tf_adapter.map_string_string(init)
    tf_adapter.PluginInit(init_options)

def npu_global_shutdown():
    tf_adapter.PluginFinalize()
