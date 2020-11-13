/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tdt/index_transform.h"
#include "tf_adapter/util/npu_attrs.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/util/env_var.h"
#include "securec.h"
#include "mmpa/mmpa_api.h"
#include <mutex>
#include <regex>

namespace tensorflow {
Status GetEnvDeviceID(uint32_t &device_id) {
  int64 phy_device_id = -1;
  int64 logic_device_id = -1;
  const char* tmp_ascend_device_id = std::getenv("ASCEND_DEVICE_ID");
  string env_ascend_device_id(tmp_ascend_device_id == nullptr ? "" : tmp_ascend_device_id);
  const char* tmp_device_id = std::getenv("DEVICE_ID");
  string env_device_id(tmp_device_id == nullptr ? "" : tmp_device_id);
  if (env_ascend_device_id.empty() && env_device_id.empty()) {
    LOG(WARNING) << "[GePlugin] DEVICE_ID and ASCEND_DEVICE_ID is none, use default device id : 0";
  } else if (!env_ascend_device_id.empty()) {
    if (!strings::safe_strto64(env_ascend_device_id, &logic_device_id)) {
      return errors::InvalidArgument("ASCEND_DEVICE_ID is valid, not digit.");
    }
    if (logic_device_id < 0) {
      return errors::InvalidArgument("ASCEND_DEVICE_ID should be >= 0.");
    }
    device_id = static_cast<uint32_t>(logic_device_id);
  } else {
    if (!strings::safe_strto64(env_device_id, &phy_device_id)) {
      return errors::InvalidArgument("DEVICE_ID is valid, not digit.");
    }
    if (phy_device_id < 0) {
      return errors::InvalidArgument("DEVICE_ID should be >= 0.");
    }
    if (IndexTransform(static_cast<uint32_t>(phy_device_id), device_id) != 0) {
      return errors::InvalidArgument("get logic device id by DEVICE_ID failed.");
    }
  }
  return Status::OK();
}
inline void split(const std::string &s, std::vector<std::string> &result, const char *delchar = " ") {
  if (s.empty()) { return; }
  result.clear();
  char *buffer = new char[s.size() + 1];
  buffer[s.size()] = '\0';
  errno_t e = strcpy_s(buffer, s.size() + 1, s.c_str());
  if (e != EOK) {
    delete[] buffer;
    return;
  }
  char *p_tmp = nullptr;
  char *p = strtok_s(buffer, delchar, &p_tmp);
  if (p != nullptr) {
    do { result.emplace_back(p); } while ((p = strtok_s(nullptr, delchar, &p_tmp)));
  }
  delete[] buffer;
}

inline bool checkProfilingOptions(string &options) {
  if (options.empty()) { return false; }
  std::set<string> validOptions;
  validOptions.insert("training_trace");
  validOptions.insert("task_trace");
  validOptions.insert("op_trace");

  std::vector<string> optionVec;
  split(options, optionVec, ":");
  if (optionVec.empty()) { return false; }
  for (const auto &option : optionVec) {
    if (validOptions.find(option) == validOptions.end()) { return false; }
  }
  return true;
}

inline Status checkDumpStep(const string &dump_step) {
  std::string tmp_dump_step = dump_step + "|";
  std::smatch result;
  std::vector<string> match_vecs;
  std::regex pattern(R"((\d{1,}-\d{1,}\||\d{1,}\|)+)");
  if (regex_match(tmp_dump_step, result, pattern)) {
    split(result.str(), match_vecs, "|");
    // 100 is the max sets of dump steps.
    if (match_vecs.size() > 100) { return errors::InvalidArgument("dump_step only support dump <= 100 sets of data"); }
    for (const auto &match_vec : match_vecs) {
      std::vector<string> tmp_vecs;
      split(match_vec, tmp_vecs, "-");
      if (tmp_vecs.size() > 1) {
        if (std::atoi(tmp_vecs[0].c_str()) >= std::atoi(tmp_vecs[1].c_str())) {
          return errors::InvalidArgument("in range steps, the first step is >= "
                                         "second step, correct example:'0|5|10-20'");
        }
      }
    }
  } else {
    return errors::InvalidArgument("dump_step string style is error,"
                                   " correct example:'0|5|10|50-100'");
  }
  return Status::OK();
}

inline Status checkDumpMode(const string &dump_mode) {
  std::set<string> dump_mode_list = {"input", "output", "all"};
  std::set<string>::iterator iter;

  if ((iter = dump_mode_list.find(dump_mode)) != dump_mode_list.end()) {
    return Status::OK();
  } else {
    return errors::InvalidArgument("dump mode should be one of the list:[input, output, all]");
  }
}

inline Status checkDumpDebugMode(const string &dump_debug_mode) {
  std::set<string> dump_debug_mode_list = {"aicore_overflow", "atomic_overflow", "all"};
  std::set<string>::iterator iter;

  if ((iter = dump_debug_mode_list.find(dump_debug_mode)) != dump_debug_mode_list.end()) {
    return Status::OK();
  } else {
    return errors::InvalidArgument("dump debug mode should be one of the list:[aicore_overflow, atomic_overflow, all]");
  }
}

inline Status CheckPath(const string &input, string &output) {
  if (mmIsDir(input.c_str()) != EN_OK) {
    return errors::InvalidArgument("the path ", input.c_str(), " is not directory.");
  }
  char trusted_path[MMPA_MAX_PATH] = { "\0" };
  if (mmRealPath(input.c_str(), trusted_path, MMPA_MAX_PATH) != EN_OK) {
    return errors::InvalidArgument("the path ", input.c_str(), " is invalid.");
  }
  if (mmAccess2(trusted_path, R_OK | W_OK) != EN_OK) {
    return errors::InvalidArgument("the path ", input.c_str(), " does't have read, write permissions.");
  }
  output = trusted_path;
  return Status::OK();
}

inline Status CheckOpImplMode(const string &op_select_implmode) {
  std::set<string> op_impl_mode_list = {"high_precision", "high_performance"};

  if (op_impl_mode_list.find(op_select_implmode) != op_impl_mode_list.end()) {
    return Status::OK();
  } else {
    return errors::InvalidArgument("op select impl mode should be one of the list:[high_precision, high_performance]");
  }
}

inline Status CheckMstuneMode(const string &mstune_mode) {
  std::set<string> mstune_mode_list = {"1", "2", "3", "4"};

  if (mstune_mode_list.find(mstune_mode) != mstune_mode_list.end()) {
    return Status::OK();
  } else {
    return errors::InvalidArgument("mstune mode should be one of the list:['1', '2', '3', '4']");
  }
}

std::map<std::string, std::string> NpuAttrs::GetSessOptions(OpKernelConstruction *ctx) {
  std::map<std::string, std::string> sess_options;
  std::string variable_format_optimize = std::to_string(true);
  std::string hcom_parallel = std::to_string(false);
  std::string graph_memory_max_size;
  std::string variable_memory_max_size;
  std::string enable_dump = std::to_string(false);
  std::string enable_dump_debug = std::to_string(false);
  std::string dump_path;
  std::string dump_step;
  std::string dump_mode = "output";
  std::string dump_debug_mode = "all";
  std::string stream_max_parallel_num;
  string npuOptimizer;
  std::string is_tailing_optimization = std::to_string(false);
  std::string op_select_implmode;
  std::string optypelist_for_implmode;
  string input_shape;
  string dynamic_dims;
  std::string buffer_optimize;
  std::string enable_small_channel = "0";
  std::string fusion_switch_file;
  std::string enable_compress_weight = std::to_string(false);
  std::string compress_weight_conf;

  if (ctx != nullptr && ctx->GetAttr("_NpuOptimizer", &npuOptimizer) == Status::OK()) {
    ctx->GetAttr("_variable_format_optimize", &variable_format_optimize);
    ctx->GetAttr("_hcom_parallel", &hcom_parallel);
    ctx->GetAttr("_graph_memory_max_size", &graph_memory_max_size);
    ctx->GetAttr("_variable_memory_max_size", &variable_memory_max_size);
    ctx->GetAttr("_enable_dump", &enable_dump);
    ctx->GetAttr("_enable_dump_debug", &enable_dump_debug);
    if (enable_dump != std::to_string(false) || enable_dump_debug != std::to_string(false)) {
      ctx->GetAttr("_dump_path", &dump_path);
    }
    if (enable_dump != std::to_string(false)) {
      if (ctx->GetAttr("_dump_step", &dump_step) == Status::OK() && !dump_step.empty()) {
        Status s = checkDumpStep(dump_step);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
      }
      if (ctx->GetAttr("_dump_mode", &dump_mode) == Status::OK()) {
        Status s = checkDumpMode(dump_mode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
      }
    }
    if (enable_dump_debug != std::to_string(false)) {
      if (ctx->GetAttr("_dump_debug_mode", &dump_debug_mode) == Status::OK()) {
        Status s = checkDumpDebugMode(dump_debug_mode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
      }
    }
    ctx->GetAttr("_stream_max_parallel_num", &stream_max_parallel_num);
    ctx->GetAttr("_is_tailing_optimization", &is_tailing_optimization);
    ctx->GetAttr("_op_select_implmode", &op_select_implmode);
    ctx->GetAttr("_optypelist_for_implmode", &optypelist_for_implmode);
    ctx->GetAttr("_input_shape", &input_shape);
    ctx->GetAttr("_dynamic_dims", &dynamic_dims);
    ctx->GetAttr("_buffer_optimize", &buffer_optimize);
    ctx->GetAttr("_enable_small_channel", &enable_small_channel);
    ctx->GetAttr("_fusion_switch_file", &fusion_switch_file);
    ctx->GetAttr("_enable_compress_weight", &enable_compress_weight);
    ctx->GetAttr("_compress_weight_conf", &compress_weight_conf);
  }

  // session options
  sess_options["ge.exec.variable_acc"] = variable_format_optimize;
  sess_options[ge::HCOM_PARALLEL] = hcom_parallel;
  sess_options[ge::STREAM_MAX_PARALLEL_NUM] = stream_max_parallel_num;
  if (!graph_memory_max_size.empty()) { sess_options[ge::GRAPH_MEMORY_MAX_SIZE] = graph_memory_max_size; }
  if (!variable_memory_max_size.empty()) { sess_options[ge::VARIABLE_MEMORY_MAX_SIZE] = variable_memory_max_size; }
  sess_options[ge::OPTION_EXEC_ENABLE_DUMP] = enable_dump;
  sess_options[ge::OPTION_EXEC_DUMP_PATH] = dump_path;
  sess_options[ge::OPTION_EXEC_DUMP_STEP] = dump_step;
  sess_options[ge::OPTION_EXEC_DUMP_MODE] = dump_mode;
  sess_options[ge::OPTION_EXEC_ENABLE_DUMP_DEBUG] = enable_dump_debug;
  sess_options[ge::OPTION_EXEC_DUMP_DEBUG_MODE] = dump_debug_mode;
  sess_options["ge.exec.isTailingOptimization"] = is_tailing_optimization;
  sess_options[ge::OP_SELECT_IMPL_MODE] = op_select_implmode;
  sess_options[ge::OPTYPELIST_FOR_IMPLMODE] = optypelist_for_implmode;
  sess_options["ge.inputShape"] = input_shape;
  sess_options["ge.dynamicDims"] = dynamic_dims;
  sess_options["ge.bufferOptimize"] = buffer_optimize;
  sess_options["ge.enableSmallChannel"] = enable_small_channel;
  sess_options["ge.fusionSwitchFile"] = fusion_switch_file;
  sess_options["ge.enableCompressWeight"] = enable_compress_weight;
  sess_options["compress_weight_conf"] = compress_weight_conf;

  return sess_options;
}

std::map<std::string, std::string> NpuAttrs::GetDefaultInitOptions() {
  std::map<std::string, std::string> init_options;
  init_options["ge.exec.precision_mode"] = "allow_fp32_to_fp16";
  init_options[ge::OPTION_EXEC_PROFILING_MODE] = std::to_string(false);
  init_options[ge::OPTION_EXEC_PROFILING_OPTIONS] = "training_trace";
  init_options[ge::AUTO_TUNE_MODE] = "";
  init_options[ge::OPTION_GRAPH_RUN_MODE] = "1";
  init_options[ge::OP_DEBUG_LEVEL] = "0";
  init_options[ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES] = "";
  init_options[ge::OPTION_EXEC_PROFILING_FPPONIT_OPTIONS] = "";
  init_options[ge::OPTION_EXEC_PROFILING_BPPONIT_OPTIONS] = "";
  init_options["ge.buildMode"] = "";
  init_options["ge.tuningPath"] = "";
  return init_options;
}

std::map<std::string, std::string> NpuAttrs::GetInitOptions(OpKernelConstruction *ctx) {
  std::map<std::string, std::string> init_options;
  std::string precision_mode;
  std::string profiling_mode = std::to_string(false);
  std::string profiling_options = "training_trace";
  std::string auto_tune_mode;
  std::string graph_run_mode = "1";
  std::string op_debug_level = "0";
  std::string enable_scope_fusion_passes;
  std::string enable_exception_dump;
  string npuOptimizer;
  string bp_point;
  string fp_point;
  string mstune_mode;
  string work_path;
  std::string op_compiler_cache_mode;
  std::string op_compiler_cache_dir;
  std::string debug_dir;

  if (ctx != nullptr && ctx->GetAttr("_NpuOptimizer", &npuOptimizer) == Status::OK()) {
    ctx->GetAttr("_precision_mode", &precision_mode);
    ctx->GetAttr("_profiling_mode", &profiling_mode);
    ctx->GetAttr("_profiling_options", &profiling_options);
    ctx->GetAttr("_auto_tune_mode", &auto_tune_mode);
    ctx->GetAttr("_graph_run_mode", &graph_run_mode);
    ctx->GetAttr("_op_debug_level", &op_debug_level);
    ctx->GetAttr("_enable_scope_fusion_passes", &enable_scope_fusion_passes);
    ctx->GetAttr("_bp_point", &bp_point);
    ctx->GetAttr("_fp_point", &fp_point);
    ctx->GetAttr("_enable_exception_dump", &enable_exception_dump);
    ctx->GetAttr("_mstune_mode", &mstune_mode);
    ctx->GetAttr("_work_path", &work_path);
    ctx->GetAttr("_op_compiler_cache_mode", &op_compiler_cache_mode);
    ctx->GetAttr("_op_compiler_cache_dir", &op_compiler_cache_dir);
    ctx->GetAttr("_debug_dir", &debug_dir);
  }

  init_options["ge.exec.precision_mode"] = precision_mode;
  init_options[ge::OPTION_EXEC_PROFILING_MODE] = profiling_mode;
  if (profiling_mode != std::to_string(false) && !checkProfilingOptions(profiling_options)) {
    LOG(FATAL) << "profiling options must be in 'training_trace', 'task_trace' or 'op_trace'";
  }
  init_options[ge::OPTION_EXEC_PROFILING_OPTIONS] = profiling_options;
  init_options[ge::AUTO_TUNE_MODE] = auto_tune_mode;
  init_options[ge::OPTION_GRAPH_RUN_MODE] = graph_run_mode;
  init_options[ge::OP_DEBUG_LEVEL] = op_debug_level;
  init_options[ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES] = enable_scope_fusion_passes;
  init_options[ge::OPTION_EXEC_PROFILING_BPPONIT_OPTIONS] = bp_point;
  init_options[ge::OPTION_EXEC_PROFILING_FPPONIT_OPTIONS] = fp_point;
  init_options["ge.exec.enable_exception_dump"] = enable_exception_dump;
  init_options["ge.buildMode"] = mstune_mode;
  init_options["ge.tuningPath"] = work_path;
  init_options["ge.op_compiler_cache_mode"] = op_compiler_cache_mode;
  init_options["ge.op_compiler_cache_dir"] = op_compiler_cache_dir;
  init_options["ge.debugDir"] = debug_dir;

  return init_options;
}

std::map<std::string, std::string> NpuAttrs::GetPassOptions(const GraphOptimizationPassOptions &options) {
  std::map<std::string, std::string> pass_options;
  const RewriterConfig &rewrite_options = options.session_options->config.graph_options().rewrite_options();
  bool do_npu_optimizer = false;
  bool enable_dp = false;
  bool use_off_line = false;
  bool mix_compile_mode = false;
  int iterations_per_loop = 1;
  bool lower_functional_ops = false;
  string job = "default";
  int task_index = 0;
  for (const auto &custom_optimizer : rewrite_options.custom_optimizers()) {
    if (custom_optimizer.name() == "NpuOptimizer") {
      do_npu_optimizer = true;
      const auto &params = custom_optimizer.parameter_map();
      if (params.count("enable_data_pre_proc")) { enable_dp = params.at("enable_data_pre_proc").b(); }
      if (params.count("use_off_line")) {
        use_off_line = params.at("use_off_line").b();
        if (params.count("mix_compile_mode")) { mix_compile_mode = params.at("mix_compile_mode").b(); }
        if (params.count("iterations_per_loop")) { iterations_per_loop = params.at("iterations_per_loop").i(); }
        if (params.count("lower_functional_ops")) { lower_functional_ops = params.at("lower_functional_ops").b(); }
        if (params.count("job")) {
          job = params.at("job").s();
        } else {
          job = "localhost";
        }
        if (params.count("task_index")) { task_index = params.at("task_index").i(); }
      }
    }
  }
  // pass options
  pass_options["do_npu_optimizer"] = std::to_string(do_npu_optimizer);
  pass_options["enable_dp"] = std::to_string(enable_dp);
  pass_options["use_off_line"] = std::to_string(use_off_line);
  pass_options["mix_compile_mode"] = std::to_string(mix_compile_mode);
  pass_options["iterations_per_loop"] = std::to_string(iterations_per_loop);
  pass_options["lower_functional_ops"] = std::to_string(lower_functional_ops);
  pass_options["job"] = job;
  pass_options["task_index"] = std::to_string(task_index);

  return pass_options;
}

std::map<std::string, std::string> NpuAttrs::GetPassOptions(OpKernelConstruction *ctx) {
  std::map<std::string, std::string> pass_options;
  std::string do_npu_optimizer = std::to_string(false);
  std::string enable_dp = std::to_string(false);
  std::string use_off_line = std::to_string(false);
  std::string mix_compile_mode = std::to_string(false);
  std::string iterations_per_loop = "1";
  std::string lower_functional_ops = std::to_string(false);
  string job = "default";
  std::string task_index = "0";
  Status s = Status::OK();
  string npuOptimizer;

  if (ctx != nullptr && ctx->GetAttr("_NpuOptimizer", &npuOptimizer) == Status::OK()) {
    do_npu_optimizer = "1";
    ctx->GetAttr("_enable_data_pre_proc", &enable_dp);
    if (ctx->GetAttr("_use_off_line", &use_off_line) == Status::OK()) {
      ctx->GetAttr("_mix_compile_mode", &mix_compile_mode);
      ctx->GetAttr("_iterations_per_loop", &iterations_per_loop);
      ctx->GetAttr("_lower_functional_ops", &lower_functional_ops);
      if (ctx->GetAttr("_job", &job) != Status::OK()) { job = "localhost"; }
      ctx->GetAttr("_task_index", &task_index);
    }
  }
  // pass options
  pass_options["do_npu_optimizer"] = do_npu_optimizer;
  pass_options["enable_dp"] = enable_dp;
  pass_options["use_off_line"] = use_off_line;
  pass_options["mix_compile_mode"] = mix_compile_mode;
  pass_options["iterations_per_loop"] = iterations_per_loop;
  pass_options["lower_functional_ops"] = lower_functional_ops;
  pass_options["job"] = job;
  pass_options["task_index"] = task_index;

  return pass_options;
}

std::map<std::string, std::string> NpuAttrs::GetPassOptions(AttrSlice attrs) {
  std::map<std::string, std::string> pass_options;
  std::string do_npu_optimizer = std::to_string(false);
  std::string enable_dp = std::to_string(false);
  std::string use_off_line = std::to_string(false);
  std::string mix_compile_mode = std::to_string(false);
  std::string iterations_per_loop = "1";
  std::string lower_functional_ops = std::to_string(false);
  string job = "default";
  std::string task_index = "0";
  Status s = Status::OK();

  if (attrs.Find("_NpuOptimizer") != nullptr) {
    do_npu_optimizer = std::to_string(true);
    if (attrs.Find("_enable_data_pre_proc") != nullptr) { enable_dp = attrs.Find("_enable_data_pre_proc")->s(); }
    if (attrs.Find("_use_off_line") != nullptr) {
      use_off_line = attrs.Find("_use_off_line")->s();
      if (attrs.Find("_mix_compile_mode") != nullptr) { mix_compile_mode = attrs.Find("_mix_compile_mode")->s(); }
      if (attrs.Find("_iterations_per_loop") != nullptr) {
        iterations_per_loop = attrs.Find("_iterations_per_loop")->s();
      }
      if (attrs.Find("_lower_functional_ops") != nullptr) {
        lower_functional_ops = attrs.Find("_lower_functional_ops")->s();
      }
      if (attrs.Find("_job") != nullptr) {
        job = attrs.Find("_job")->s();
      } else {
        job = "localhost";
      }
      if (attrs.Find("_task_index") != nullptr) { task_index = attrs.Find("_task_index")->s(); }
    }
  }
  // pass options
  pass_options["do_npu_optimizer"] = do_npu_optimizer;
  pass_options["enable_dp"] = enable_dp;
  pass_options["use_off_line"] = use_off_line;
  pass_options["mix_compile_mode"] = mix_compile_mode;
  pass_options["iterations_per_loop"] = iterations_per_loop;
  pass_options["lower_functional_ops"] = lower_functional_ops;
  pass_options["job"] = job;
  pass_options["task_index"] = task_index;

  return pass_options;
}

std::map<std::string, std::string> NpuAttrs::GetAllAttrOptions(AttrSlice attrs) {
  std::map<std::string, std::string> all_options;
  std::string do_npu_optimizer = std::to_string(false);
  std::string enable_dp = std::to_string(false);
  std::string use_off_line = std::to_string(false);
  std::string mix_compile_mode = std::to_string(false);
  std::string iterations_per_loop = "1";
  std::string lower_functional_ops = std::to_string(false);
  string job = "default";
  std::string task_index = "0";
  Status s = Status::OK();

  std::string variable_format_optimize = std::to_string(true);
  std::string hcom_parallel = std::to_string(false);
  std::string graph_memory_max_size;
  std::string variable_memory_max_size;
  std::string enable_dump = std::to_string(false);
  std::string enable_dump_debug = std::to_string(false);
  std::string dump_path;
  std::string dump_step;
  std::string dump_mode = "output";
  std::string dump_debug_mode = "all";
  std::string stream_max_parallel_num;

  std::string is_tailing_optimization = std::to_string(false);
  std::string precision_mode;
  std::string profiling_mode = std::to_string(false);
  std::string profiling_options = "training_trace";
  std::string auto_tune_mode;
  std::string graph_run_mode = "1";
  std::string op_debug_level = "0";
  std::string enable_scope_fusion_passes;
  std::string enable_exception_dump;
  string npuOptimizer;
  string bp_point;
  string fp_point;
  std::string op_select_implmode;
  std::string optypelist_for_implmode;
  string input_shape;
  string dynamic_dims;
  string mstune_mode;
  string work_path;
  std::string buffer_optimize;
  std::string enable_small_channel = "0";
  std::string fusion_switch_file;
  std::string enable_compress_weight = std::to_string(false);
  std::string compress_weight_conf;
  std::string op_compiler_cache_mode;
  std::string op_compiler_cache_dir;
  std::string debug_dir;

  if (attrs.Find("_NpuOptimizer") != nullptr) {
    do_npu_optimizer = std::to_string(true);
    if (attrs.Find("_enable_data_pre_proc") != nullptr) { enable_dp = attrs.Find("_enable_data_pre_proc")->s(); }
    if (attrs.Find("_use_off_line") != nullptr) {
      use_off_line = attrs.Find("_use_off_line")->s();
      if (attrs.Find("_mix_compile_mode") != nullptr) { mix_compile_mode = attrs.Find("_mix_compile_mode")->s(); }
      if (attrs.Find("_iterations_per_loop") != nullptr) {
        iterations_per_loop = attrs.Find("_iterations_per_loop")->s();
      }
      if (attrs.Find("_lower_functional_ops") != nullptr) {
        lower_functional_ops = attrs.Find("_lower_functional_ops")->s();
      }
      if (attrs.Find("_job") != nullptr) {
        job = attrs.Find("_job")->s();
      } else {
        job = "localhost";
      }
      if (attrs.Find("_task_index") != nullptr) { task_index = attrs.Find("_task_index")->s(); }
    }

    if (attrs.Find("_variable_format_optimize") != nullptr) {
      variable_format_optimize = attrs.Find("_variable_format_optimize")->s();
    }
    if (attrs.Find("_hcom_parallel") != nullptr) { hcom_parallel = attrs.Find("_hcom_parallel")->s(); }
    if (attrs.Find("_graph_memory_max_size") != nullptr) {
      graph_memory_max_size = attrs.Find("_graph_memory_max_size")->s();
    }
    if (attrs.Find("_variable_memory_max_size") != nullptr) {
      variable_memory_max_size = attrs.Find("_variable_memory_max_size")->s();
    }
    if (attrs.Find("_enable_dump") != nullptr) { enable_dump = attrs.Find("_enable_dump")->s(); }
    if (attrs.Find("_enable_dump_debug") != nullptr) { enable_dump_debug = attrs.Find("_enable_dump_debug")->s(); }
    if (enable_dump != std::to_string(false) || enable_dump_debug != std::to_string(false)) {
      if (attrs.Find("_dump_path") != nullptr) { dump_path = attrs.Find("_dump_path")->s(); }
    }
    if (enable_dump != std::to_string(false)) {
      if (attrs.Find("_dump_step") != nullptr) {
        dump_step = attrs.Find("_dump_step")->s();
        if (!dump_step.empty()) {
          Status s = checkDumpStep(dump_step);
          if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        }
      }
      if (attrs.Find("_dump_mode") != nullptr) {
        dump_mode = attrs.Find("_dump_mode")->s();
        Status s = checkDumpMode(dump_mode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
      }
    }
    if (enable_dump_debug != std::to_string(false)) {
      if (attrs.Find("_dump_debug_mode") != nullptr) {
        dump_debug_mode = attrs.Find("_dump_debug_mode")->s();
        Status s = checkDumpDebugMode(dump_debug_mode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
      }
    }
    if (attrs.Find("_stream_max_parallel_num") != nullptr) {
      stream_max_parallel_num = attrs.Find("_stream_max_parallel_num")->s();
    }

    if (attrs.Find("_is_tailing_optimization") != nullptr) {
      is_tailing_optimization = attrs.Find("_is_tailing_optimization")->s();
    }
    if (attrs.Find("_precision_mode") != nullptr) { precision_mode = attrs.Find("_precision_mode")->s(); }
    if (attrs.Find("_profiling_mode") != nullptr) { profiling_mode = attrs.Find("_profiling_mode")->s(); }
    if (attrs.Find("_profiling_options") != nullptr) { profiling_options = attrs.Find("_profiling_options")->s(); }
    if (attrs.Find("_auto_tune_mode") != nullptr) { auto_tune_mode = attrs.Find("_auto_tune_mode")->s(); }
    if (attrs.Find("_graph_run_mode") != nullptr) { graph_run_mode = attrs.Find("_graph_run_mode")->s(); }
    if (attrs.Find("_op_debug_level") != nullptr) { op_debug_level = attrs.Find("_op_debug_level")->s(); }
    if (attrs.Find("_enable_scope_fusion_passes") != nullptr) {
      enable_scope_fusion_passes = attrs.Find("_enable_scope_fusion_passes")->s();
    }
    if (attrs.Find("_fp_point") != nullptr) { fp_point = attrs.Find("_fp_point")->s(); }
    if (attrs.Find("_bp_point") != nullptr) { bp_point = attrs.Find("_bp_point")->s(); }
    if (attrs.Find("_enable_exception_dump") != nullptr) {
      enable_exception_dump = attrs.Find("_enable_exception_dump")->s();
    }
    if (attrs.Find("_op_select_implmode") != nullptr) {
      op_select_implmode = attrs.Find("_op_select_implmode")->s();
    }
    if (attrs.Find("_optypelist_for_implmode") != nullptr) {
      optypelist_for_implmode = attrs.Find("_optypelist_for_implmode")->s();
    }
    if (attrs.Find("_input_shape") != nullptr) { input_shape = attrs.Find("_input_shape")->s(); }
    if (attrs.Find("_dynamic_dims") != nullptr) { dynamic_dims = attrs.Find("_dynamic_dims")->s(); }
    if (attrs.Find("_mstune_mode") != nullptr) { mstune_mode = attrs.Find("_mstune_mode")->s(); }
    if (attrs.Find("_work_path") != nullptr) { work_path = attrs.Find("_work_path")->s(); }
    if (attrs.Find("_buffer_optimize") != nullptr) { buffer_optimize = attrs.Find("_buffer_optimize")->s(); }
    if (attrs.Find("_enable_small_channel") != nullptr) {
      enable_small_channel = attrs.Find("_enable_small_channel")->s();
    }
    if (attrs.Find("_fusion_switch_file") != nullptr) {
      fusion_switch_file = attrs.Find("_fusion_switch_file")->s();
    }
    if (attrs.Find("_enable_compress_weight") != nullptr) {
      enable_compress_weight = attrs.Find("_enable_compress_weight")->s();
    }
    if (attrs.Find("_compress_weight_conf") != nullptr) {
      compress_weight_conf = attrs.Find("_compress_weight_conf")->s();
    }
    if (attrs.Find("_op_compiler_cache_mode") != nullptr) {
      op_compiler_cache_mode = attrs.Find("_op_compiler_cache_mode")->s();
    }
    if (attrs.Find("_op_compiler_cache_dir") != nullptr) {
      op_compiler_cache_dir = attrs.Find("_op_compiler_cache_dir")->s();
    }
    if (attrs.Find("_debug_dir") != nullptr) {
      debug_dir = attrs.Find("_debug_dir")->s();
    }
  }

  all_options["variable_format_optimize"] = variable_format_optimize;
  all_options["hcom_parallel"] = hcom_parallel;
  all_options["stream_max_parallel_num"] = stream_max_parallel_num;
  if (!graph_memory_max_size.empty()) { all_options["graph_memory_max_size"] = graph_memory_max_size; }
  if (!variable_memory_max_size.empty()) { all_options["variable_memory_max_size"] = variable_memory_max_size; }

  all_options["enable_dump"] = enable_dump;
  all_options["dump_path"] = dump_path;
  all_options["dump_step"] = dump_step;
  all_options["dump_mode"] = dump_mode;
  all_options["enable_dump_debug"] = enable_dump_debug;
  all_options["dump_debug_mode"] = dump_debug_mode;

  all_options["is_tailing_optimization"] = is_tailing_optimization;
  all_options["precision_mode"] = precision_mode;
  all_options["profiling_mode"] = profiling_mode;
  if (profiling_mode != std::to_string(false) && !checkProfilingOptions(profiling_options)) {
    LOG(FATAL) << "profiling options must be in 'training_trace', 'task_trace' or 'op_trace'";
  }
  all_options["profiling_options"] = profiling_options;
  all_options["auto_tune_mode"] = auto_tune_mode;
  all_options["graph_run_mode"] = graph_run_mode;
  all_options["op_debug_level"] = op_debug_level;
  all_options["enable_scope_fusion_passes"] = enable_scope_fusion_passes;
  all_options["enable_exception_dump"] = enable_exception_dump;

  all_options["do_npu_optimizer"] = do_npu_optimizer;
  all_options["enable_data_pre_proc"] = enable_dp;
  all_options["use_off_line"] = use_off_line;
  all_options["mix_compile_mode"] = mix_compile_mode;
  all_options["iterations_per_loop"] = iterations_per_loop;
  all_options["lower_functional_ops"] = lower_functional_ops;
  all_options["job"] = job;
  all_options["task_index"] = task_index;
  all_options["fp_point"] = fp_point;
  all_options["bp_point"] = bp_point;
  all_options["op_select_implmode"] = op_select_implmode;
  all_options["optypelist_for_implmode"] = optypelist_for_implmode;
  all_options["input_shape"] = input_shape;
  all_options["dynamic_dims"] = dynamic_dims;
  all_options["mstune_mode"] = mstune_mode;
  all_options["work_path"] = work_path;
  all_options["buffer_optimize"] = buffer_optimize;
  all_options["enable_small_channel"] = enable_small_channel;
  all_options["fusion_switch_file"] = fusion_switch_file;
  all_options["enable_compress_weight"] = enable_compress_weight;
  all_options["compress_weight_conf"] = compress_weight_conf;
  all_options["op_compiler_cache_mode"] = op_compiler_cache_mode;
  all_options["op_compiler_cache_dir"] = op_compiler_cache_dir;
  all_options["debug_dir"] = debug_dir;

  return all_options;
}

std::map<std::string, std::string> NpuAttrs::GetDefaultPassOptions() {
  std::map<std::string, std::string> pass_options;
  pass_options["do_npu_optimizer"] = std::to_string(false);
  pass_options["enable_dp"] = std::to_string(false);
  pass_options["use_off_line"] = std::to_string(false);
  pass_options["mix_compile_mode"] = std::to_string(false);
  pass_options["iterations_per_loop"] = std::to_string(1);
  pass_options["lower_functional_ops"] = std::to_string(false);
  pass_options["job"] = "default";
  pass_options["task_index"] = std::to_string(0);
  return pass_options;
}

Status NpuAttrs::SetNpuOptimizerAttr(const GraphOptimizationPassOptions &options, Node *node) {
  std::map<std::string, std::string> sess_options;
  bool variable_format_optimize = true;
  bool hcom_parallel = false;
  std::string graph_memory_max_size;
  std::string variable_memory_max_size;
  bool enable_dump = false;
  bool enable_dump_debug = false;
  std::string dump_path;
  std::string dump_step;
  std::string dump_mode = "output";
  std::string dump_debug_mode = "all";
  std::string stream_max_parallel_num;

  std::map<std::string, std::string> init_options;
  bool is_tailing_optimization = false;
  std::string precision_mode;
  bool profiling_mode = false;
  std::string profiling_options = "training_trace";
  std::string auto_tune_mode;
  int graph_run_mode = 1;
  int op_debug_level = 0;
  std::string enable_scope_fusion_passes;

  std::map<std::string, std::string> pass_options;
  bool do_npu_optimizer = false;
  bool enable_dp = false;
  bool use_off_line = false;
  bool mix_compile_mode = false;
  int iterations_per_loop = 1;
  bool lower_functional_ops = false;
  string job = "default";
  int task_index = 0;
  string bp_point;
  string fp_point;
  int enable_exception_dump = 0;
  string op_select_implmode;
  string optypelist_for_implmode;
  string input_shape;
  string dynamic_dims;
  string mstune_mode;
  string work_path;
  std::string buffer_optimize;
  int enable_small_channel = 0;
  std::string fusion_switch_file;
  bool enable_compress_weight = false;
  std::string compress_weight_conf;
  std::string op_compiler_cache_mode;
  std::string op_compiler_cache_dir;
  std::string debug_dir;

  const RewriterConfig &rewrite_options = options.session_options->config.graph_options().rewrite_options();
  for (const auto &custom_optimizer : rewrite_options.custom_optimizers()) {
    if (custom_optimizer.name() == "NpuOptimizer") {
      const auto &params = custom_optimizer.parameter_map();
      if (params.count("variable_format_optimize")) {
        variable_format_optimize = params.at("variable_format_optimize").b();
      }
      if (params.count("hcom_parallel")) { hcom_parallel = params.at("hcom_parallel").b(); }
      if (params.count("graph_memory_max_size")) { graph_memory_max_size = params.at("graph_memory_max_size").s(); }
      if (params.count("variable_memory_max_size")) {
        variable_memory_max_size = params.at("variable_memory_max_size").s();
      }
      if (params.count("enable_dump")) { enable_dump = params.at("enable_dump").b(); }
      if (params.count("enable_dump_debug")) { enable_dump_debug = params.at("enable_dump_debug").b(); }
      if (enable_dump || enable_dump_debug) {
        if (params.count("dump_path")) {
          string tmp_path = params.at("dump_path").s();
          Status s = CheckPath(tmp_path, dump_path);
          if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        } else {
          LOG(FATAL) << "if use dump function, dump_path must be set.";
        }
      }
      if (enable_dump) {
        if (params.count("dump_step")) {
          dump_step = params.at("dump_step").s();
          Status s = checkDumpStep(dump_step);
          if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        }
        if (params.count("dump_mode")) {
          dump_mode = params.at("dump_mode").s();
          Status s = checkDumpMode(dump_mode);
          if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        }
      }
      if (enable_dump_debug) {
        if (params.count("dump_debug_mode")) {
          dump_debug_mode = params.at("dump_debug_mode").s();
          Status s = checkDumpDebugMode(dump_debug_mode);
          if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        }
      }
      if (params.count("stream_max_parallel_num")) {
        stream_max_parallel_num = params.at("stream_max_parallel_num").s();
      }

      if (params.count("is_tailing_optimization")) {
        is_tailing_optimization = params.at("is_tailing_optimization").b();
      }
      if (params.count("profiling_mode")) { profiling_mode = params.at("profiling_mode").b(); }
      if (params.count("profiling_options") && profiling_mode) {
        profiling_options = params.at("profiling_options").s();
      }
      if (params.count("fp_point")) { fp_point = params.at("fp_point").s(); }
      if (params.count("bp_point")) { bp_point = params.at("bp_point").s(); }
      if (params.count("auto_tune_mode")) { auto_tune_mode = params.at("auto_tune_mode").s(); }
      if (params.count("graph_run_mode")) {
        graph_run_mode = params.at("graph_run_mode").i();
        if (graph_run_mode > 1) { LOG(FATAL) << "graph_run_mode value must be 0 or 1"; }
      }
      if (params.count("op_debug_level")) { op_debug_level = params.at("op_debug_level").i(); }
      if (params.count("enable_scope_fusion_passes")) {
        enable_scope_fusion_passes = params.at("enable_scope_fusion_passes").s();
      }
      int64 rank_size = 1;
      (void) ReadInt64FromEnvVar("RANK_SIZE", 1, &rank_size);
      if (rank_size > 1 && params.count("mstune_mode")) {
        mstune_mode = params.at("mstune_mode").s();
        Status s  = CheckMstuneMode(mstune_mode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        if (params.count("work_path")) {
          string tmp_path = params.at("work_path").s();
          s = CheckPath(tmp_path, work_path);
          if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        } else {
          LOG(FATAL) << "work_path must be set when use mstune_mode.";
        }
      }
      if (params.count("precision_mode")) {
        precision_mode = params.at("precision_mode").s();
      } else {
        if (graph_run_mode) {
          precision_mode = "allow_fp32_to_fp16";
        } else {
          precision_mode = "force_fp16";
        }
      }

      do_npu_optimizer = true;
      if (params.count("enable_data_pre_proc")) { enable_dp = params.at("enable_data_pre_proc").b(); }
      if (params.count("use_off_line")) {
        use_off_line = params.at("use_off_line").b();
        if (params.count("mix_compile_mode")) { mix_compile_mode = params.at("mix_compile_mode").b(); }
        if (params.count("iterations_per_loop")) { iterations_per_loop = params.at("iterations_per_loop").i(); }
        if (params.count("lower_functional_ops")) { lower_functional_ops = params.at("lower_functional_ops").b(); }
        if (params.count("job")) {
          job = params.at("job").s();
        } else {
          job = "localhost";
        }
        if (params.count("task_index")) { task_index = params.at("task_index").i(); }
      }
      if (params.count("enable_exception_dump")) { enable_exception_dump = params.at("enable_exception_dump").i(); }
      if (!params.count("op_select_implmode") && !params.count("optypelist_for_implmode")) {
        op_select_implmode = "high_performance";
      } else if (params.count("op_select_implmode") && !params.count("optypelist_for_implmode")) {
        op_select_implmode = params.at("op_select_implmode").s();
        Status s = CheckOpImplMode(op_select_implmode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
      } else if (params.count("optypelist_for_implmode") && !params.count("op_select_implmode")) {
        LOG(FATAL) << "when use optypelist_for_implmode, op_select_implmode must be set.";
      } else {
        op_select_implmode = params.at("op_select_implmode").s();
        Status s = CheckOpImplMode(op_select_implmode);
        if (!s.ok()) { LOG(FATAL) << s.error_message(); }
        optypelist_for_implmode = params.at("optypelist_for_implmode").s();
      }
      if (params.count("input_shape") && params.count("dynamic_dims")) {
        input_shape = params.at("input_shape").s();
        dynamic_dims = params.at("dynamic_dims").s();
      } else if ((params.count("input_shape") && !params.count("dynamic_dims")) ||
                 (!params.count("input_shape") && params.count("dynamic_dims"))) {
        LOG(FATAL) << "input_shape and dynamic_dims should be paired.";
      }
      if (params.count("buffer_optimize")) {
        buffer_optimize = params.at("buffer_optimize").s();
        if (buffer_optimize != "l2_optimize" && buffer_optimize != "off_optimize") {
          LOG(FATAL) << "buffer_optimize is valid, should be one of [l2_optimize, off_optimize]";
        }
      }
      if (params.count("enable_small_channel")) { enable_small_channel = params.at("enable_small_channel").i(); }
      if (params.count("fusion_switch_file")) { fusion_switch_file = params.at("fusion_switch_file").s(); }
      if (params.count("enable_compress_weight") && params.count("compress_weight_conf")) {
        LOG(FATAL) << "enable_compress_weight can not use with compress_weight_conf.";
      }
      if (params.count("enable_compress_weight")) { enable_compress_weight = params.at("enable_compress_weight").b(); }
      if (params.count("compress_weight_conf")) { compress_weight_conf = params.at("compress_weight_conf").s(); }
      if (params.count("op_compiler_cache_mode")) {
        op_compiler_cache_mode = params.at("op_compiler_cache_mode").s();
      }
      if (params.count("op_compiler_cache_dir")) {
        op_compiler_cache_dir = params.at("op_compiler_cache_dir").s();
      }
      if (params.count("debug_dir")) {
        debug_dir = params.at("debug_dir").s();
      }
    }
  }

  // session options
  sess_options["variable_format_optimize"] = std::to_string(variable_format_optimize);
  sess_options["hcom_parallel"] = std::to_string(hcom_parallel);
  sess_options["stream_max_parallel_num"] = stream_max_parallel_num;
  if (!graph_memory_max_size.empty()) { sess_options["graph_memory_max_size"] = graph_memory_max_size; }
  if (!variable_memory_max_size.empty()) { sess_options["variable_memory_max_size"] = variable_memory_max_size; }

  sess_options["enable_dump"] = std::to_string(enable_dump);
  sess_options["dump_path"] = dump_path;
  sess_options["dump_step"] = dump_step;
  sess_options["dump_mode"] = dump_mode;
  sess_options["enable_dump_debug"] = std::to_string(enable_dump_debug);
  sess_options["dump_debug_mode"] = dump_debug_mode;
  sess_options["is_tailing_optimization"] = std::to_string(is_tailing_optimization);
  sess_options["op_select_implmode"] = op_select_implmode;
  sess_options["optypelist_for_implmode"] = optypelist_for_implmode;
  sess_options["input_shape"] = input_shape;
  sess_options["dynamic_dims"] = dynamic_dims;
  sess_options["buffer_optimize"] = buffer_optimize;
  sess_options["enable_small_channel"] = std::to_string(enable_small_channel);
  sess_options["fusion_switch_file"] = fusion_switch_file;
  sess_options["enable_compress_weight"] = std::to_string(enable_compress_weight);
  sess_options["compress_weight_conf"] = compress_weight_conf;

  init_options["precision_mode"] = precision_mode;
  init_options["profiling_mode"] = std::to_string(profiling_mode);
  if (profiling_mode && !checkProfilingOptions(profiling_options)) {
    LOG(FATAL) << "profiling options must be in 'training_trace', 'task_trace' or 'op_trace'";
  }
  if (profiling_mode && (profiling_options.find("task_trace") != string::npos ||
      profiling_options.find("training_trace") != string::npos)) {
    if (bp_point == "" || fp_point == "") {
      LOG(WARNING) << "profiling training_trace option should use with bp_point and fp_point";
    } else {
      init_options["bp_point"] = bp_point;
      init_options["fp_point"] = fp_point;
    }
  }
  init_options["profiling_options"] = profiling_options;
  init_options["auto_tune_mode"] = auto_tune_mode;
  init_options["graph_run_mode"] = std::to_string(graph_run_mode);
  init_options["op_debug_level"] = std::to_string(op_debug_level);
  init_options["enable_scope_fusion_passes"] = enable_scope_fusion_passes;
  init_options["enable_exception_dump"] = std::to_string(enable_exception_dump);
  init_options["mstune_mode"] = mstune_mode;
  init_options["work_path"] = work_path;
  init_options["op_compiler_cache_mode"] = op_compiler_cache_mode;
  init_options["op_compiler_cache_dir"] = op_compiler_cache_dir;
  init_options["debug_dir"] = debug_dir;

  pass_options["do_npu_optimizer"] = std::to_string(do_npu_optimizer);
  pass_options["enable_data_pre_proc"] = std::to_string(enable_dp);
  pass_options["use_off_line"] = std::to_string(use_off_line);
  pass_options["mix_compile_mode"] = std::to_string(mix_compile_mode);
  pass_options["iterations_per_loop"] = std::to_string(iterations_per_loop);
  pass_options["lower_functional_ops"] = std::to_string(lower_functional_ops);
  pass_options["job"] = job;
  pass_options["task_index"] = std::to_string(task_index);

  std::string attr_name;
  for (const auto &option : sess_options) {
    attr_name = std::string("_") + option.first;
    node->AddAttr(attr_name, option.second);
  }
  for (const auto &option : init_options) {
    attr_name = std::string("_") + option.first;
    node->AddAttr(attr_name, option.second);
  }
  for (const auto &option : pass_options) {
    attr_name = std::string("_") + option.first;
    node->AddAttr(attr_name, option.second);
  }
  node->AddAttr("_NpuOptimizer", "NpuOptimizer");

  return Status::OK();
}

}  // namespace tensorflow
