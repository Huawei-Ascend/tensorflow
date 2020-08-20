/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved. foss@huawei.com

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

#include "tf_adapter/util/npu_attrs.h"
#include "securec.h"
#include <mutex>
#include <regex>

namespace tensorflow {

inline void split(const std::string& s, std::vector<std::string>& result,
           const char* delchar = " ") {
  if (s.empty()) {
    return;
  }
  result.clear();
  char* buffer = new char[s.size() + 1];
  buffer[s.size()] = '\0';
  errno_t e = strcpy_s(buffer, s.size() + 1, s.c_str());
  if (e != EOK) {
    delete[] buffer;
    return;
  }
  char *pTmp = NULL;
  char* p = strtok_s(buffer, delchar, &pTmp);
  do {
    result.push_back(p);
  } while ((p = strtok_s(NULL, delchar, &pTmp)));
  delete[] buffer;
  return;
}

inline bool checkProfilingOptions(string& options) {
  if (options.empty()) {
    return false;
  }
  std::set<string> validOptions;
  validOptions.insert("training_trace");
  validOptions.insert("task_trace");
  validOptions.insert("op_trace");

  std::vector<string> optionVec;
  split(options, optionVec, ":");
  if (optionVec.size() == 0) {
    return false;
  }
  for (auto option : optionVec) {
    if (validOptions.find(option) == validOptions.end()) {
      return false;
    }
  }
  return true;
}

inline Status checkDumpStep(string dump_step) {
  std::string tmp_dump_step = dump_step + "|";
  std::smatch result;
  std::vector<string> match_vecs;
  std::regex pattern("(\\d{1,}-\\d{1,}\\||\\d{1,}\\|)+");
  if (regex_match(tmp_dump_step, result, pattern)) {
    split(result.str(), match_vecs, "|");
    // 100 is the max sets of dump steps.
    if (match_vecs.size() > 100) {
      return errors::InvalidArgument("dump_step only support dump <= 100 sets of data");
    }
    for (auto match_vec : match_vecs) {
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

inline Status checkDumpMode(string dump_mode) {
  std::set<string> dump_mode_list = {"input", "output", "all"};
  std::set<string>::iterator iter;

  if ((iter = dump_mode_list.find(dump_mode)) != dump_mode_list.end()) {
    return Status::OK();
  } else {
    return errors::InvalidArgument("dump mode should be one of the list:[input, output, all]");
  }
}

inline Status checkDumpDebugMode(string dump_debug_mode) {
  std::set<string> dump_debug_mode_list = {"aicore_overflow", "atomic_overflow", "all"};
  std::set<string>::iterator iter;

  if ((iter = dump_debug_mode_list.find(dump_debug_mode)) != dump_debug_mode_list.end()) {
    return Status::OK();
  } else {
    return errors::InvalidArgument("dump debug mode should be one of the list:[aicore_overflow, atomic_overflow, all]");
  }
}

std::map<std::string, std::string> NpuAttrs::GetSessOptions(OpKernelConstruction *ctx) {
  std::map<std::string, std::string> sess_options;
  std::string variable_format_optimize = std::to_string(true);
  std::string hcom_parallel = std::to_string(false);
  std::string graph_memory_max_size = "";
  std::string variable_memory_max_size = "";
  std::string enable_dump = std::to_string(false);
  std::string enable_dump_debug = std::to_string(false);
  std::string dump_path = "";
  std::string dump_step = "";
  std::string dump_mode = "output";
  std::string dump_debug_mode = "all";
  std::string stream_max_parallel_num = "";
  string npuOptimizer = "";

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
      if (ctx->GetAttr("_dump_step", &dump_step) == Status::OK() &&
          !dump_step.empty()) {
        Status s = checkDumpStep(dump_step);
        if (!s.ok()) {
          LOG(FATAL) << s.error_message();
        }
      }
      if (ctx->GetAttr("_dump_mode", &dump_mode) == Status::OK()) {
        Status s = checkDumpMode(dump_mode);
        if (!s.ok()) {
          LOG(FATAL) << s.error_message();
        }
      }
    }
    if (enable_dump_debug != std::to_string(false)) {
      if (ctx->GetAttr("_dump_debug_mode", &dump_debug_mode) == Status::OK()) {
        Status s = checkDumpDebugMode(dump_debug_mode);
        if (!s.ok()) {
          LOG(FATAL) << s.error_message();
        }
      }
    }
    ctx->GetAttr("_stream_max_parallel_num", &stream_max_parallel_num);
  }

  // session options
  sess_options["ge.exec.variable_acc"] = variable_format_optimize;
  sess_options[ge::HCOM_PARALLEL] = hcom_parallel;
  sess_options[ge::STREAM_MAX_PARALLEL_NUM] = stream_max_parallel_num;
  if (!graph_memory_max_size.empty()) {
    sess_options[ge::GRAPH_MEMORY_MAX_SIZE] = graph_memory_max_size;
  }
  if (!variable_memory_max_size.empty()) {
    sess_options[ge::VARIABLE_MEMORY_MAX_SIZE] = variable_memory_max_size;
  }
  sess_options[ge::OPTION_EXEC_ENABLE_DUMP] = enable_dump;
  sess_options[ge::OPTION_EXEC_DUMP_PATH] = dump_path;
  sess_options[ge::OPTION_EXEC_DUMP_STEP] = dump_step;
  sess_options[ge::OPTION_EXEC_DUMP_MODE] = dump_mode;
  sess_options[ge::OPTION_EXEC_ENABLE_DUMP_DEBUG] = enable_dump_debug;
  sess_options[ge::OPTION_EXEC_DUMP_DEBUG_MODE] = dump_debug_mode;

  return sess_options;
}

std::map<std::string, std::string> NpuAttrs::GetDefaultInitOptions() {
  std::map<std::string, std::string> init_options;
  init_options["ge.exec.isTailingOptimization"] = std::to_string(false);
  init_options["ge.exec.precision_mode"] = "";
  init_options[ge::OPTION_EXEC_PROFILING_MODE] = std::to_string(false);
  init_options[ge::OPTION_EXEC_PROFILING_OPTIONS] = "training_trace";
  init_options[ge::AUTO_TUNE_MODE] = "";
  init_options[ge::OPTION_GRAPH_RUN_MODE] = "1";
  init_options[ge::OP_DEBUG_LEVEL] = "0";
  init_options[ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES] = "";
  return init_options;
}

std::map<std::string, std::string> NpuAttrs::GetInitOptions(OpKernelConstruction *ctx) {
  std::map<std::string, std::string> init_options;
  std::string is_tailing_optimization = std::to_string(false);
  std::string precision_mode = "";
  std::string profiling_mode = std::to_string(false);
  std::string profiling_options = "training_trace";
  std::string auto_tune_mode = "";
  std::string graph_run_mode = "1";
  std::string op_debug_level = "0";
  std::string enable_scope_fusion_passes = "";
  string npuOptimizer = "";

  if (ctx != nullptr && ctx->GetAttr("_NpuOptimizer", &npuOptimizer) == Status::OK()) {
    ctx->GetAttr("_is_tailing_optimization", &is_tailing_optimization);
    ctx->GetAttr("_precision_mode", &precision_mode);
    ctx->GetAttr("_profiling_mode", &profiling_mode);
    ctx->GetAttr("_profiling_options", &profiling_options);
    ctx->GetAttr("_auto_tune_mode", &auto_tune_mode);
    ctx->GetAttr("_graph_run_mode", &graph_run_mode);
    ctx->GetAttr("_op_debug_level", &op_debug_level);
    ctx->GetAttr("_enable_scope_fusion_passes", &enable_scope_fusion_passes);
  }

  init_options["ge.exec.isTailingOptimization"] = is_tailing_optimization;
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

  return init_options;
}

std::map<std::string, std::string> NpuAttrs::GetPassOptions(const GraphOptimizationPassOptions &options) {
  std::map<std::string, std::string> pass_options;
  const RewriterConfig &rewrite_options =
    options.session_options->config.graph_options().rewrite_options();
  bool do_npu_optimizer = false;
  bool enable_dp = false;
  bool use_off_line = false;
  bool mix_compile_mode = false;
  int iterations_per_loop = 1;
  bool lower_functional_ops = false;
  string job = "default";
  int task_index = 0;
  for (auto custom_optimizer : rewrite_options.custom_optimizers()) {
    if (custom_optimizer.name() == "NpuOptimizer") {
      do_npu_optimizer = true;
      const auto params = custom_optimizer.parameter_map();
      if (params.count("enable_data_pre_proc")) {
        enable_dp = params.at("enable_data_pre_proc").b();
      }
      if (params.count("use_off_line")) {
        use_off_line = params.at("use_off_line").b();
        if (params.count("mix_compile_mode")) {
          mix_compile_mode = params.at("mix_compile_mode").b();
        }
        if (params.count("iterations_per_loop")) {
          iterations_per_loop = params.at("iterations_per_loop").i();
        }
        if (params.count("lower_functional_ops")) {
          lower_functional_ops = params.at("lower_functional_ops").b();
        }
        if (params.count("job")) {
          job = params.at("job").s();
        } else {
          job = "localhost";
        }
        if (params.count("task_index")) {
            task_index = params.at("task_index").i();
        }
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
  string npuOptimizer = "";

  if (ctx != nullptr && ctx->GetAttr("_NpuOptimizer", &npuOptimizer) == Status::OK()) {
    do_npu_optimizer = true;
    ctx->GetAttr("_enable_data_pre_proc", &enable_dp);
    if (ctx->GetAttr("_use_off_line", &use_off_line) == Status::OK()) {
      ctx->GetAttr("_mix_compile_mode", &mix_compile_mode);
      ctx->GetAttr("_iterations_per_loop", &iterations_per_loop);
      ctx->GetAttr("_lower_functional_ops", &lower_functional_ops);
      if (ctx->GetAttr("_job", &job) != Status::OK()) {
        job = "localhost";
      }
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
    if (attrs.Find("_enable_data_pre_proc") != nullptr) {
      enable_dp = attrs.Find("_enable_data_pre_proc")->s();
    }
    if (attrs.Find("_use_off_line") != nullptr) {
      use_off_line = attrs.Find("_use_off_line")->s();
      if (attrs.Find("_mix_compile_mode") != nullptr) {
        mix_compile_mode = attrs.Find("_mix_compile_mode")->s();
      }
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
      if (attrs.Find("_task_index") != nullptr) {
          task_index = attrs.Find("_task_index")->s();
      }
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
  std::string graph_memory_max_size = "";
  std::string variable_memory_max_size = "";
  std::string enable_dump = std::to_string(false);
  std::string enable_dump_debug = std::to_string(false);
  std::string dump_path = "";
  std::string dump_step = "";
  std::string dump_mode = "output";
  std::string dump_debug_mode = "all";
  std::string stream_max_parallel_num = "";

  std::string is_tailing_optimization = std::to_string(false);
  std::string precision_mode = "";
  std::string profiling_mode = std::to_string(false);
  std::string profiling_options = "training_trace";
  std::string auto_tune_mode = "";
  std::string graph_run_mode = "1";
  std::string op_debug_level = "0";
  std::string enable_scope_fusion_passes = "";
  string npuOptimizer = "";

  if (attrs.Find("_NpuOptimizer") != nullptr) {
    do_npu_optimizer = std::to_string(true);
    if (attrs.Find("_enable_data_pre_proc") != nullptr) {
      enable_dp = attrs.Find("_enable_data_pre_proc")->s();
    }
    if (attrs.Find("_use_off_line") != nullptr) {
      use_off_line = attrs.Find("_use_off_line")->s();
      if (attrs.Find("_mix_compile_mode") != nullptr) {
        mix_compile_mode = attrs.Find("_mix_compile_mode")->s();
      }
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
      if (attrs.Find("_task_index") != nullptr) {
          task_index = attrs.Find("_task_index")->s();
      }
    }

    if (attrs.Find("_variable_format_optimize") != nullptr) {
      variable_format_optimize = attrs.Find("_variable_format_optimize")->s();
    }
    if (attrs.Find("_hcom_parallel") != nullptr) {
      hcom_parallel = attrs.Find("_hcom_parallel")->s();
    }
    if (attrs.Find("_graph_memory_max_size") != nullptr) {
      graph_memory_max_size = attrs.Find("_graph_memory_max_size")->s();
    }
    if (attrs.Find("_variable_memory_max_size") != nullptr) {
      variable_memory_max_size = attrs.Find("_variable_memory_max_size")->s();
    }
    if (attrs.Find("_enable_dump") != nullptr) {
      enable_dump = attrs.Find("_enable_dump")->s();
    }
    if (attrs.Find("_enable_dump_debug") != nullptr) {
      enable_dump_debug = attrs.Find("_enable_dump_debug")->s();
    }
    if (enable_dump != std::to_string(false) || enable_dump_debug != std::to_string(false)) {
      if (attrs.Find("_dump_path") != nullptr) {
        dump_path = attrs.Find("_dump_path")->s();
      }
    }
    if (enable_dump != std::to_string(false)) {
      if (attrs.Find("_dump_step") != nullptr) {
        dump_step = attrs.Find("_dump_step")->s();
        if (!dump_step.empty()) {
          Status s = checkDumpStep(dump_step);
          if (!s.ok()) {
            LOG(FATAL) << s.error_message();
          }
        }
      }
      if (attrs.Find("_dump_mode") != nullptr) {
        dump_mode = attrs.Find("_dump_mode")->s();
        Status s = checkDumpMode(dump_mode);
        if (!s.ok()) {
          LOG(FATAL) << s.error_message();
        }
      }
    }
    if (enable_dump_debug != std::to_string(false)) {
      if (attrs.Find("_dump_debug_mode") != nullptr) {
        dump_debug_mode = attrs.Find("_dump_debug_mode")->s();
        Status s = checkDumpDebugMode(dump_debug_mode);
        if (!s.ok()) {
          LOG(FATAL) << s.error_message();
        }
      }
    }
    if (attrs.Find("_stream_max_parallel_num") != nullptr) {
      stream_max_parallel_num = attrs.Find("_stream_max_parallel_num")->s();
    }

    if (attrs.Find("_is_tailing_optimization") != nullptr) {
      is_tailing_optimization = attrs.Find("_is_tailing_optimization")->s();
    }
    if (attrs.Find("_precision_mode") != nullptr) {
      precision_mode = attrs.Find("_precision_mode")->s();
    }
    if (attrs.Find("_profiling_mode") != nullptr) {
      profiling_mode = attrs.Find("_profiling_mode")->s();
    }
    if (attrs.Find("_profiling_options") != nullptr) {
      profiling_options = attrs.Find("_profiling_options")->s();
    }
    if (attrs.Find("_auto_tune_mode") != nullptr) {
      auto_tune_mode = attrs.Find("_auto_tune_mode")->s();
    }
    if (attrs.Find("_graph_run_mode") != nullptr) {
      graph_run_mode = attrs.Find("_graph_run_mode")->s();
    }
    if (attrs.Find("_op_debug_level") != nullptr) {
      op_debug_level = attrs.Find("_op_debug_level")->s();
    }
    if (attrs.Find("_enable_scope_fusion_passes") != nullptr) {
      enable_scope_fusion_passes = attrs.Find("_enable_scope_fusion_passes")->s();
    }
  }

  all_options["variable_format_optimize"] = variable_format_optimize;
  all_options["hcom_parallel"] = hcom_parallel;
  all_options["stream_max_parallel_num"] = stream_max_parallel_num;
  if (!graph_memory_max_size.empty()) {
    all_options["graph_memory_max_size"] = graph_memory_max_size;
  }
  if (!variable_memory_max_size.empty()) {
    all_options["variable_memory_max_size"] = variable_memory_max_size;
  }
  
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

  all_options["do_npu_optimizer"] = do_npu_optimizer;
  all_options["enable_data_pre_proc"] = enable_dp;
  all_options["use_off_line"] = use_off_line;
  all_options["mix_compile_mode"] = mix_compile_mode;
  all_options["iterations_per_loop"] = iterations_per_loop;
  all_options["lower_functional_ops"] = lower_functional_ops;
  all_options["job"] = job;
  all_options["task_index"] = task_index;

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

Status NpuAttrs::SetNpuOptimizerAttr(const GraphOptimizationPassOptions &options, Node *node)
{
  std::map<std::string, std::string> sess_options;
  bool variable_format_optimize = true;
  bool hcom_parallel = false;
  std::string graph_memory_max_size = "";
  std::string variable_memory_max_size = "";
  bool enable_dump = false;
  bool enable_dump_debug = false;
  std::string dump_path = "";
  std::string dump_step = "";
  std::string dump_mode = "output";
  std::string dump_debug_mode = "all";
  std::string stream_max_parallel_num = "";

  std::map<std::string, std::string> init_options;
  bool is_tailing_optimization = false;
  std::string precision_mode = "";
  bool profiling_mode = false;
  std::string profiling_options = "training_trace";
  std::string auto_tune_mode = "";
  int graph_run_mode = 1;
  int op_debug_level = 0;
  std::string enable_scope_fusion_passes = "";

  std::map<std::string, std::string> pass_options;
  bool do_npu_optimizer = false;
  bool enable_dp = false;
  bool use_off_line = false;
  bool mix_compile_mode = false;
  int iterations_per_loop = 1;
  bool lower_functional_ops = false;
  string job = "default";
  int task_index = 0;


  const RewriterConfig &rewrite_options = options.session_options->config.graph_options().rewrite_options();
  for (auto custom_optimizer : rewrite_options.custom_optimizers()) {
    if (custom_optimizer.name() == "NpuOptimizer") {
      const auto params = custom_optimizer.parameter_map();
      if (params.count("variable_format_optimize")) {
        variable_format_optimize = params.at("variable_format_optimize").b();
      }
      if (params.count("hcom_parallel")) {
        hcom_parallel = params.at("hcom_parallel").b();
      }
      if (params.count("graph_memory_max_size")) {
        graph_memory_max_size = params.at("graph_memory_max_size").s();
      }
      if (params.count("variable_memory_max_size")) {
        variable_memory_max_size = params.at("variable_memory_max_size").s();
      }
      if (params.count("enable_dump")) {
        enable_dump = params.at("enable_dump").b();
      }
      if (params.count("enable_dump_debug")) {
        enable_dump_debug = params.at("enable_dump_debug").b();
      }
      if (enable_dump || enable_dump_debug) {
        if (params.count("dump_path")) {
          dump_path = params.at("dump_path").s();
        }
      }
      if (enable_dump) {
        if (params.count("dump_step")) {
          dump_step = params.at("dump_step").s();
          Status s = checkDumpStep(dump_step);
          if (!s.ok()) {
            LOG(FATAL) << s.error_message();
          }
        }
        if (params.count("dump_mode")) {
          dump_mode = params.at("dump_mode").s();
          Status s = checkDumpMode(dump_mode);
          if (!s.ok()) {
            LOG(FATAL) << s.error_message();
          }
        }
      }
      if (enable_dump_debug) {
        if (params.count("dump_debug_mode")) {
          dump_debug_mode = params.at("dump_debug_mode").s();
          Status s = checkDumpDebugMode(dump_debug_mode);
          if (!s.ok()) {
            LOG(FATAL) << s.error_message();
          }
        }
      }
      if (params.count("stream_max_parallel_num")) {
        stream_max_parallel_num = params.at("stream_max_parallel_num").s();
      }

      if (params.count("is_tailing_optimization")) {
        is_tailing_optimization = params.at("is_tailing_optimization").b();
      }
      if (params.count("precision_mode")) {
        precision_mode = params.at("precision_mode").s();
      }
      if (params.count("profiling_mode")) {
        profiling_mode = params.at("profiling_mode").b();
      }
      if (params.count("profiling_options") && profiling_mode) {
        profiling_options = params.at("profiling_options").s();
      }
      if (params.count("auto_tune_mode")) {
        auto_tune_mode = params.at("auto_tune_mode").s();
      }
      if (params.count("graph_run_mode")) {
        graph_run_mode = params.at("graph_run_mode").i();
      }
      if (params.count("op_debug_level")) {
        op_debug_level = params.at("op_debug_level").i();
      }
      if (params.count("enable_scope_fusion_passes")) {
        enable_scope_fusion_passes = params.at("enable_scope_fusion_passes").s();
      }

      do_npu_optimizer = true;
      if (params.count("enable_data_pre_proc")) {
        enable_dp = params.at("enable_data_pre_proc").b();
      }
      if (params.count("use_off_line")) {
        use_off_line = params.at("use_off_line").b();
        if (params.count("mix_compile_mode")) {
          mix_compile_mode = params.at("mix_compile_mode").b();
        }
        if (params.count("iterations_per_loop")) {
          iterations_per_loop = params.at("iterations_per_loop").i();
        }
        if (params.count("lower_functional_ops")) {
          lower_functional_ops = params.at("lower_functional_ops").b();
        }
        if (params.count("job")) {
          job = params.at("job").s();
        } else {
          job = "localhost";
        }
        if (params.count("task_index")) {
            task_index = params.at("task_index").i();
        }
      }
    }
  }

  // session options
  sess_options["variable_format_optimize"] = std::to_string(variable_format_optimize);
  sess_options["hcom_parallel"] = std::to_string(hcom_parallel);
  sess_options["stream_max_parallel_num"] = stream_max_parallel_num;
  if (!graph_memory_max_size.empty()) {
    sess_options["graph_memory_max_size"] = graph_memory_max_size;
  }
  if (!variable_memory_max_size.empty()) {
    sess_options["variable_memory_max_size"] = variable_memory_max_size;
  }
  
  sess_options["enable_dump"] = std::to_string(enable_dump);
  sess_options["dump_path"] = dump_path;
  sess_options["dump_step"] = dump_step;
  sess_options["dump_mode"] = dump_mode;
  sess_options["enable_dump_debug"] = std::to_string(enable_dump_debug);
  sess_options["dump_debug_mode"] = dump_debug_mode;

  init_options["is_tailing_optimization"] = std::to_string(is_tailing_optimization);
  init_options["precision_mode"] = precision_mode;
  init_options["profiling_mode"] = std::to_string(profiling_mode);
  if (profiling_mode && !checkProfilingOptions(profiling_options)) {
    LOG(FATAL) << "profiling options must be in 'training_trace', 'task_trace' or 'op_trace'";
  }
  init_options["profiling_options"] = profiling_options;
  init_options["auto_tune_mode"] = auto_tune_mode;
  init_options["graph_run_mode"] = std::to_string(graph_run_mode);
  init_options["op_debug_level"] = std::to_string(op_debug_level);
  init_options["enable_scope_fusion_passes"] = enable_scope_fusion_passes;

  pass_options["do_npu_optimizer"] = std::to_string(do_npu_optimizer);
  pass_options["enable_data_pre_proc"] = std::to_string(enable_dp);
  pass_options["use_off_line"] = std::to_string(use_off_line);
  pass_options["mix_compile_mode"] = std::to_string(mix_compile_mode);
  pass_options["iterations_per_loop"] = std::to_string(iterations_per_loop);
  pass_options["lower_functional_ops"] = std::to_string(lower_functional_ops);
  pass_options["job"] = job;
  pass_options["task_index"] = std::to_string(task_index);

  std::string attr_name = "";
  for (auto option : sess_options) {
    attr_name = std::string("_") + option.first;
    node->AddAttr(attr_name, option.second);
  }
  for (auto option : init_options) {
    attr_name = std::string("_") + option.first;
    node->AddAttr(attr_name, option.second);
  }
  for (auto option : pass_options) {
    attr_name = std::string("_") + option.first;
    node->AddAttr(attr_name, option.second);
  }
  node->AddAttr("_NpuOptimizer", "NpuOptimizer");

  return Status::OK();
}

}
