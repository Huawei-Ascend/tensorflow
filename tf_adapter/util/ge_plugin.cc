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
#include "framework/omg/parser/model_parser.h"
#include "framework/omg/parser/parser_api.h"
#include "framework/omg/parser/parser_factory.h"
#include "ge/ge_api.h"
#include "ge/ge_api_types.h"
#include "tdt/tdt_host_interface.h"
#include "tdt/tsd_client.h"
#include "tensorflow/core/util/env_var.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/npu_attrs.h"
#include "tf_adapter/util/npu_plugin.h"
#include <thread>

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

  LOG(INFO) << "[GePlugin] graph run mode : " << init_options[ge::OPTION_GRAPH_RUN_MODE];
  // prepare options for ge Initialize

  const int64 kMaxDeviceID = 7;
  (void) ReadInt64FromEnvVar("DEVICE_ID", 0, &device_id_);
  if (device_id_ < 0 || device_id_ > kMaxDeviceID) {
    LOG(WARNING) << "[GePlugin] device_id should in [0, 7]. use default device id : 0.";
  }
  init_options[ge::OPTION_EXEC_DEVICE_ID] = std::to_string(device_id_);

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
  char *env_rank_id = std::getenv("RANK_ID");
  char *env_pod_name = std::getenv("POD_NAME");
  char *env_rank_table_file = std::getenv("RANK_TABLE_FILE");
  if ((env_rank_table_file != nullptr) && (rankSizeNum > 0)) {
    LOG(INFO) << "[GePlugin] env RANK_TABLE_FILE:" << env_rank_table_file;
    is_use_hcom = true;
    init_options[ge::OPTION_EXEC_RANK_TABLE_FILE] = env_rank_table_file;
    if (env_pod_name != nullptr) {
      deploy_mode = true;
      init_options[ge::OPTION_EXEC_POD_NAME] = env_pod_name;
    } else if (env_rank_id != nullptr) {
      LOG(INFO) << "[GePlugin] env RANK_ID:" << env_rank_id;
      deploy_mode = false;
      init_options[ge::OPTION_EXEC_RANK_ID] = env_rank_id;
    } else {
      LOG(ERROR) << "[GePlugin] Can't find rank_id or pod_name in env.";
    }
  }

  init_options[ge::OPTION_EXEC_IS_USEHCOM] = std::to_string(is_use_hcom);
  init_options[ge::OPTION_EXEC_DEPLOY_MODE] = std::to_string(deploy_mode);

  // tailing optimization
  LOG(INFO) << "[GePlugin] is_tailing_optimization : " << init_options["ge.exec.isTailingOptimization"];

  // profiling configuration
  LOG(INFO) << "[GePlugin] profiling_mode : " << init_options[ge::OPTION_EXEC_PROFILING_MODE]
            << ", profiling_options:" << init_options[ge::OPTION_EXEC_PROFILING_OPTIONS];

  // mix precision configuration
  LOG(INFO) << "[GePlugin] precision_mode : " << init_options[ge::PRECISION_MODE];

  // auto tune configuration
  LOG(INFO) << "[GePlugin] auto_tune_mode : " << init_options[ge::AUTO_TUNE_MODE];

  // debug configuration
  LOG(INFO) << "[GePlugin] op_debug_level : " << init_options[ge::OP_DEBUG_LEVEL];

  // scope fusion configuration
  LOG(INFO) << "[GePlugin] enable_scope_fusion_passes : " << init_options[ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES];

  // Open TsdClient first, then call GEInitialize
  LOG(INFO) << "[GePlugin] Open TsdClient and Init tdt host.";
  int32_t ret = tdt::TdtHostInit(static_cast<uint32_t>(device_id_));
  if (ret != 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GePlugin] Tdt host init failed, tdt error code : " << ret;
  }
  TDT_StatusT tdt_status = TsdOpen(static_cast<uint32_t>(device_id_), static_cast<uint32_t>(rankSizeNum));
  if (tdt_status != TDT_OK) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GePlugin] Open TsdClient failed, tdt error code : " << tdt_status
               << ", error message : " << TDT_GET_ERROR_STR(tdt_status);
  }
  LOG(INFO) << "[GePlugin] Open TsdClient success and tdt host init success.";

  // ge Initialize
  ge::Status status = ge::GEInitialize(init_options);
  if (status != ge::SUCCESS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GePlugin] Initialize ge failed, ret : " << ToString(status);
  }
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
  if (ret != 0) { LOG(ERROR) << "[GePlugin] Close tdt failed, tdt_ret : " << ret; }
  TDT_StatusT tdt_status = TsdClose(device_id_);
  if (tdt_status != TDT_OK) {
    LOG(ERROR) << "[GePlugin] Close TsdClient failed, tdt_ret : " << tdt_status;
  } else {
    LOG(INFO) << "[GePlugin] Close TsdClient success.";
  }
  isInit_ = false;
}

bool GePlugin::IsGlobal() {
  std::lock_guard<std::mutex> lock(mutex_);
  return isGlobal_;
}

void PluginInit(std::map<std::string, std::string> &init_options) {
  GePlugin::GetInstance()->Init(init_options, true);
  LOG(INFO) << "npu plugin init success";
}

void PluginFinalize() {
  GePlugin::GetInstance()->Finalize();
  LOG(INFO) << "npu plugin finalize success";
}
