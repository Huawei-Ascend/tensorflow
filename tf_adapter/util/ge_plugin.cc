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

#include "framework/common/ge_inner_error_codes.h"
#include "framework/common/types.h"
#include "framework/omg/parser/parser_api.h"
#include "framework/omg/omg_inner_types.h"
#include "ge/ge_api.h"
#include "ge/ge_api_types.h"
#include "tdt/tdt_host_interface.h"
#include "tensorflow/core/util/env_var.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/npu_attrs.h"
#include "tf_adapter/util/npu_plugin.h"
#include <thread>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

using namespace tensorflow;
using namespace tdt;
constexpr int kFatalSleepTime = 3000;
namespace {
inline string ToString(ge::Status status) { return ::ge::StatusFactory::Instance()->GetErrDesc(status); }
}  // namespace

GePlugin::GePlugin()

    : device_id_(0), isInit_(false), isGlobal_(false) {
  LOG(INFO) << "[GePlugin] new constructor";
}

GePlugin::~GePlugin() {
  LOG(INFO) << "[GePlugin] destroy constructor begin";
  Finalize();
  LOG(INFO) << "[GePlugin] destroy constructor end";
}

GePlugin *GePlugin::GetInstance() {
  static GePlugin instance;
  return &instance;
}

void GePlugin::Init(std::map<std::string, std::string> &init_options, bool is_global) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (isInit_) {
    LOG(INFO) << "[GePlugin] Ge has already initialized";
    return;
  }
  init_options_ = init_options;
  const char *tf_config = std::getenv("TF_CONFIG");
  int exec_hccl_flag = 1;
  if (tf_config != nullptr) {
    json config_info;
    try {
        config_info = json::parse(tf_config);
    } catch (json::exception &e) {
        LOG(WARNING) << "[GePlugin] Failed to convert TF_CONFIG info from string to json ,reason: " << e.what();
    }
    if (config_info.is_object()) {
      if (config_info["task"]["type"] == "ps") {
        LOG(INFO) << "The ps process does not need to be initialized";
        return;
      }
      if (config_info["task"]["type"] == "evaluator") {
        exec_hccl_flag = 0;
      }
    }
  }
  init_options[OPTION_EXEC_HCCL_FLAG] = std::to_string(exec_hccl_flag);

  LOG(INFO) << "[GePlugin] graph run mode : " << init_options[ge::OPTION_GRAPH_RUN_MODE];

  Status s = GetEnvDeviceID(device_id_);
  if (!s.ok()) { LOG(FATAL) << s.error_message(); }
  init_options[ge::OPTION_EXEC_DEVICE_ID] = std::to_string(device_id_);
  LOG(INFO) << "[GePlugin] device id : " << init_options[ge::OPTION_EXEC_DEVICE_ID];

  const char *env_job_id = std::getenv("JOB_ID");
  if (env_job_id != nullptr) {
    init_options[ge::OPTION_EXEC_JOB_ID] = env_job_id;
  } else {
    LOG(WARNING) << "[GePlugin] can not find Environment variable : JOB_ID";
  }

  int64 rankSizeNum = 1;
  (void) ReadInt64FromEnvVar("RANK_SIZE", 1, &rankSizeNum);
  if (rankSizeNum > UINT32_MAX) {
    rankSizeNum = UINT32_MAX;
    LOG(WARNING) << "[GePlugin] RANK_SIZE is larger than UINT32_MAX, set to UINT32_MAX.";
  }

  bool is_use_hcom = false;
  bool deploy_mode = false;
  char *env_rank_table_file = std::getenv("RANK_TABLE_FILE");
  if ((env_rank_table_file != nullptr) && (rankSizeNum > 0)) {
    LOG(INFO) << "[GePlugin] env RANK_TABLE_FILE:" << env_rank_table_file;
    is_use_hcom = true;
    init_options[ge::OPTION_EXEC_RANK_TABLE_FILE] = env_rank_table_file;
    char *env_pod_name = std::getenv("POD_NAME");
    if (env_pod_name != nullptr) {
      deploy_mode = true;
      init_options[ge::OPTION_EXEC_POD_NAME] = env_pod_name;
    } else {
      char *env_rank_id = std::getenv("RANK_ID");
      if (env_rank_id != nullptr) {
        LOG(INFO) << "[GePlugin] env RANK_ID:" << env_rank_id;
        deploy_mode = false;
        init_options[ge::OPTION_EXEC_RANK_ID] = env_rank_id;
      } else {
        LOG(ERROR) << "[GePlugin] Can't find rank_id or pod_name in env.";
      }
    }
  }

  init_options[ge::OPTION_EXEC_IS_USEHCOM] = std::to_string(is_use_hcom);
  init_options[ge::OPTION_EXEC_DEPLOY_MODE] = std::to_string(deploy_mode);

  // profiling configuration
  LOG(INFO) << "[GePlugin] profiling_mode : " << init_options[ge::OPTION_EXEC_PROFILING_MODE]
            << ", profiling_options:" << init_options[ge::OPTION_EXEC_PROFILING_OPTIONS]
            << ", fp_point: " << init_options[ge::OPTION_EXEC_PROFILING_FPPONIT_OPTIONS]
            << ", bp_point: " << init_options[ge::OPTION_EXEC_PROFILING_BPPONIT_OPTIONS];

  // mix precision configuration
  LOG(INFO) << "[GePlugin] precision_mode : " << init_options[ge::PRECISION_MODE];

  // auto tune configuration
  if (!init_options["ge.buildMode"].empty() && !init_options["ge.tuningPath"].empty()) {
    init_options[ge::AUTO_TUNE_MODE] = "";
  }
  LOG(INFO) << "[GePlugin] auto_tune_mode : " << init_options[ge::AUTO_TUNE_MODE];

  // debug configuration
  LOG(INFO) << "[GePlugin] op_debug_level : " << init_options[ge::OP_DEBUG_LEVEL];

  // scope fusion configuration
  LOG(INFO) << "[GePlugin] enable_scope_fusion_passes : " << init_options[ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES];

  // exception dump configuration
  LOG(INFO) << "[GePlugin] enable_exception_dump : " << init_options["ge.exec.enable_exception_dump"];

  LOG(INFO) << "[GePlugin] job_id : " << init_options[ge::OPTION_EXEC_JOB_ID];

  LOG(INFO) << "[GePlugin] op_compiler_cache_mode : " << init_options["ge.op_compiler_cache_mode"];

  LOG(INFO) << "[GePlugin] op_compiler_cache_dir : " << init_options["ge.op_compiler_cache_dir"];

  LOG(INFO) << "[GePlugin] debugDir : " << init_options["ge.debugDir"];

  // mstune mode and work path
  if (!init_options["ge.buildMode"].empty()) {
    init_options["ge.buildMode"] = "tuning";
  }
  LOG(INFO) << "[GePlugin] mstune mode : " << init_options["ge.buildMode"]
            << ", work path : " << init_options["ge.tuningPath"];

  // Open TsdClient first, then call GEInitialize
  LOG(INFO) << "[GePlugin] Open TsdClient and Init tdt host.";
  int32_t ret = tdt::TdtHostInit(static_cast<uint32_t>(device_id_));
  if (ret != 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GePlugin] Tdt host init failed, tdt error code : " << ret;
  }

  // ge Initialize
  ge::Status status = ge::GEInitialize(init_options);
  if (status != ge::SUCCESS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GePlugin] Initialize ge failed, ret : " << ToString(status);
  }
  domi::GetContext().train_flag = true;
  LOG(INFO) << "[GePlugin] Initialize ge success.";

  // parser Initialize
  ge::Status status_parser = ge::ParserInitialize(init_options);
  if (status_parser != ge::SUCCESS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GePlugin] Initialize parser failed, ret : " << ToString(status_parser);
  }
  LOG(INFO) << "[GePlugin] Initialize parser success.";
  isInit_ = true;
  isGlobal_ = is_global;
}

std::map<std::string, std::string> GePlugin::GetInitOptions() {
  return init_options_;
}

void GePlugin::Finalize() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!isInit_) {
    LOG(INFO) << "[GePlugin] Ge has already finalized.";
    return;
  }

  // ge finalize
  ge::Status status = ge::GEFinalize();
  if (status != ge::SUCCESS) { LOG(ERROR) << "[GePlugin] GE finalize failed, ret : " << ToString(status); }

  // parser finalize
  ge::Status status_parser = ge::ParserFinalize();
  if (status_parser != ge::SUCCESS) { LOG(ERROR) << "[GePlugin] Parser finalize failed, ret : " << ToString(status); }

  LOG(INFO) << "[GePlugin] Close TsdClient and destroy tdt.";
  int32_t ret = tdt::TdtHostDestroy();
  isInit_ = false;
}

bool GePlugin::IsGlobal() {
  std::lock_guard<std::mutex> lock(mutex_);
  return isGlobal_;
}

void PluginInit(std::map<std::string, std::string> &init_options) {
  GePlugin::GetInstance()->Init(init_options, true);
  LOG(INFO) << "[GePlugin] npu plugin init success";
}

void PluginFinalize() {
  GePlugin::GetInstance()->Finalize();
  LOG(INFO) << "[GePlugin] npu plugin finalize success";
}

int32_t RdmaInitAndRegister(const std::vector<ge::HostVarInfo> &var_info, size_t size) {
  ge::Status ret = ge::InitRdmaPool(size);
  if (ret != ge::SUCCESS) {
    LOG(ERROR) << "[GePlugin] init rdma pool failed, ret : " << ToString(ret);
    return -1;
  }
  LOG(INFO) << "[GePlugin] init rdma pool success.";
  ret = ge::RdmaRemoteRegister(var_info);
  if (ret != ge::SUCCESS) {
    LOG(ERROR) << "[GePlugin] rdma remote register failed, ret : " << ToString(ret);
    return -1;
  }
  LOG(INFO) << "[GePlugin] rdma remote register success.";
  return 0;
}

int32_t GetVarAddrAndSize(const string &var_name, uint64_t &base_addr, uint64_t &var_size) {
  ge::Status ret = ge::GetVarBaseAddrAndSize(var_name, base_addr, var_size);
  if (ret != ge::SUCCESS) {
    LOG(ERROR) << "[GePlugin] get " << var_name << " base addr and size failed, ret : " << ToString(ret);
    return -1;
  }
  LOG(INFO) << "[GePlugin] get " << var_name << " base addr and size success.";
  return 0;
}

int32_t MallocSharedMem(const ge::TensorInfo &tensor_info, uint64_t &dev_addr, uint64_t &memory_size) {
  ge::Status ret = ge::MallocSharedMemory(tensor_info, dev_addr, memory_size);
  if (ret != ge::SUCCESS) {
    LOG(ERROR) << "[GePlugin] malloc shared memory failed, ret : " << ToString(ret);
    return -1;
  }
  LOG(INFO) << "[GePlugin] malloc shared memory success.";
  return 0;
}