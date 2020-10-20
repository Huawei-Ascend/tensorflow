from hccl.manage.api import get_local_rank_size
from hccl.manage.api import get_rank_id
from npu_bridge import tf_adapter
from npu_bridge.estimator.npu import util
from tensorflow.python.platform import tf_logging as logging
import json
import os

__auto_tune_mode = str(tf_adapter.AUTO_TUNE_MODE)
__op_debug_level = str(tf_adapter.OP_DEBUG_LEVEL)
__option_exec_enable_scope_fusion_passes = str(tf_adapter.OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES)
__option_exec_profiling_mode = str(tf_adapter.OPTION_EXEC_PROFILING_MODE)
__option_exec_profiling_options = str(tf_adapter.OPTION_EXEC_PROFILING_OPTIONS)
__option_graph_run_mode = str(tf_adapter.OPTION_GRAPH_RUN_MODE)
__option_exec_option_exec_hccl_flag = str(tf_adapter.OPTION_EXEC_HCCL_FLAG)
__option_exec_fp_point = str(tf_adapter.OPTION_EXEC_PROFILING_FPPONIT_OPTIONS)
__option_exec_bp_point = str(tf_adapter.OPTION_EXEC_PROFILING_BPPONIT_OPTIONS)

def npu_resource_init(graph_run_mode = 1,
                      op_debug_level = 0,
                      enable_profiling = False,
                      enable_options = ["training_trace"],
                      auto_tune_mode = None,
                      precision_mode = None,
                      enable_scope_fusion_passes = None,
                      enable_exception_dump = 0,
                      fp_point = None,
                      bp_point = None,
                      mstune_mode = None,
                      work_path = None):

    util.check_nonnegative_integer(graph_run_mode, "graph_run_mode")
    if graph_run_mode > 1:
        raise ValueError('"graph_run_mode" value must be 0 or 1')
    util.check_nonnegative_integer(enable_exception_dump, "enable_exception_dump")
    util.check_nonnegative_integer(op_debug_level, "op_debug_level")
    util.check_bool_type(enable_profiling, "enable_profiling")
    enable_profiling = util.convert_bool_to_int(enable_profiling)

    init={}
    init[__option_graph_run_mode] = str(graph_run_mode)
    init[__op_debug_level] = str(op_debug_level)
    init[__option_exec_profiling_mode] = str(enable_profiling)

    if enable_profiling:
        enable_options = str(util.check_profiling_options(enable_options))
        init[__option_exec_profiling_options] = enable_options
        if "task_trace" in enable_options or "training_trace" in enable_options:
          if fp_point is None or bp_point is None:
            logging.warning("profiling training_trace option should use with bp_point and fp_point")
          else:
            init[__option_exec_fp_point] = str(fp_point)
            init[__option_exec_bp_point] = str(bp_point)
    else:
        init[__option_exec_profiling_options] = str("training_trace")

    if auto_tune_mode is not None:
        init[__auto_tune_mode] = str(auto_tune_mode)

    if precision_mode is not None:
        init["ge.exec.precision_mode"] = str(precision_mode)
    else:
        if graph_run_mode:
            init["ge.exec.precision_mode"] = str("allow_fp32_to_fp16")
        else:
            init["ge.exec.precision_mode"] = str("force_fp16")

    if enable_scope_fusion_passes is not None:
        init[__option_exec_enable_scope_fusion_passes] = str(enable_scope_fusion_passes)

    init["ge.exec.enable_exception_dump"] = str(enable_exception_dump)
    rank_size = os.getenv('RANK_SIZE')
    if int(rank_size) > 1 and mstune_mode is not None:
      mstune_mode = util.check_mstune_mode(mstune_mode)
      if mstune_mode == "4":
        mstune_mode = "tuning"
      init["ge.buildMode"] = str(mstune_mode)
      if work_path is not None:
        init["ge.tuningPath"] = str(util.check_path(work_path))
      else:
        raise ValueError('work_path must be set when use mstune_mode')

    init_options=tf_adapter.map_string_string(init)
    tf_adapter.PluginInit(init_options)

def npu_resource_shutdown():
    tf_adapter.PluginFinalize()

def rdma_remote_init(remote_var_list, mem_size):
  '''
  remote_var_list: embedding and opt var list.
  mem_size: ramd pool memory size to be allocated. type:int
  '''
  if not isinstance(remote_var_list, (tuple, list)):
    raise ValueError('{} should be tuple or list'.format(remote_var_list))
  if not isinstance(mem_size, int):
    raise ValueError('{} should be int'.format(mem_size))
  var_addr_list = []
  local_rank_size = get_local_rank_size()
  rank_id = get_rank_id()
  server_id = int(rank_id / local_rank_size)
  for var in remote_var_list:
    server_var = var[server_id]
    host_var_info = tf_adapter.HostVarInfo()
    host_var_info.base_addr = server_var[1]
    host_var_info.var_size = server_var[2]
    var_addr_list.append(host_var_info)
  res = tf_adapter.RdmaInitAndRegister(var_addr_list, mem_size)
  if res != 0:
    raise RuntimeError('rdma init and register failed')

def get_var_addr_and_size(var_name):
  if not isinstance(var_name, str):
    raise ValueError('{} should be str'.format(var_name))
  res = tf_adapter.GetVarAddrAndSize(var_name)
  if res[0] != 0:
    raise RuntimeError('{} get var addr and size failed'.format(var_name))
  return res[1], res[2]

def malloc_shared_memory(var_name, shape, data_type):
  tensor_info = tf_adapter.TensorInfo()
  tensor_info.var_name = var_name
  tensor_info.dims = tf_adapter.int64_vec(shape)
  tensor_info.data_type = data_type
  res = tf_adapter.MallocSharedMem(tensor_info)
  if res[0] != 0:
    raise RuntimeError('{} malloc shared memory failed'.format(var_name))
  return res[1], res[2]