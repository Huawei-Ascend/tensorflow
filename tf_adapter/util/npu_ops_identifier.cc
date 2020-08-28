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

#include "tf_adapter/util/npu_ops_identifier.h"
#include <fstream>

#include "nlohmann/json.hpp"
#include "tensorflow/core/platform/logging.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/generate_report.h"
using json = nlohmann::json;

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

const static std::string kOpsInfoJson = "/framework/built-in/tensorflow/npu_supported_ops.json";
const static std::string kGray = "isGray";
const static std::string kHeavy = "isHeavy";
// This method returns different instance Pointers in mixed mode and in the full sink model
NpuOpsIdentifier *NpuOpsIdentifier::GetInstance(bool is_mix) {
  if (is_mix) {
    static json mixJson;
    static NpuOpsIdentifier instance(true, mixJson);
    return &instance;
  } else {
    static json allJson;
    static NpuOpsIdentifier instance(false, allJson);
    return &instance;
  }
}
// Constructor
NpuOpsIdentifier::NpuOpsIdentifier(bool is_mix, json &ops_info) : is_mix_(is_mix), ops_info_(ops_info) {
  const std::string mode = is_mix ? "MIX" : "ALL";
  const char *path_env = std::getenv("ASCEND_OPP_PATH");
  std::string opsPath;
  if (path_env != nullptr && strlen(path_env) < ADAPTER_ENV_MAX_LENTH) {
    opsPath = path_env;
  } else {
    opsPath = "/usr/local/Ascend/opp";
    LOG(INFO) << "environment variable ASCEND_OPP_PATH is not set, use default value[" << opsPath << "]";
  }
  std::string opsJsonPath = opsPath + kOpsInfoJson;
  LOG(INFO) << "[" << mode << "] Parsing json from " << opsJsonPath;
  int32_t opsCnt = NpuOpsIdentifier::ParseOps(opsJsonPath, ops_info_);
  LOG(INFO) << opsCnt << " ops parsed";
  VLOG(1) << ops_info_.dump(2);  // 1 is vlog level, 2 is ops info index
}
// Parse and store the ops configuration json file, return num of parsed ops
int32_t NpuOpsIdentifier::ParseOps(const std::string &f, json &root) {
  std::ifstream jsonConfigFileStream(f, std::ifstream::in);
  int32_t opsCnt = 0;
  if (jsonConfigFileStream.is_open()) {
    try {
      jsonConfigFileStream >> root;
      for (auto i = root.begin(); i != root.end(); ++i) { opsCnt++; }
    } catch (json::exception &e) {
      LOG(INFO) << e.what();
      jsonConfigFileStream.close();
      return 0;
    }
    jsonConfigFileStream.close();
  } else {
    LOG(INFO) << "Open " << f << " failed";
    return 0;
  }
  return opsCnt;
}
// Determine if the node is supported by NPU. Note that it will behave
// differently in mixed mode and full sink mode
bool NpuOpsIdentifier::IsNpuSupported(const char *op_name, const std::string &node_name) {
  return NpuOpsIdentifier::IsNpuSupported(std::string(op_name), node_name);
}
bool NpuOpsIdentifier::IsNpuSupported(const std::string &op_name, const std::string &node_name) {
  bool declared = ops_info_[op_name].is_object();
  if (!declared) {
    tensorflow::GenerateReport::Details infos;
    static const std::string message = "This op is not exsit on npu.";
    infos.code = tensorflow::GenerateReport::TypeNoDefine;
    infos.message = message;
    tensorflow::GenerateReport::GetInstance()->AddUnSupportedInfo(node_name, op_name, infos);
    return false;
  }
  if (is_mix_ && ops_info_[op_name][kGray].is_boolean()) {
    tensorflow::GenerateReport::Details infos;
    static const std::string message = "This op will not be excuted on npu in mix_compile_mode.";
    infos.code = tensorflow::GenerateReport::TypeGray;
    infos.message = message;
    tensorflow::GenerateReport::GetInstance()->AddUnSupportedInfo(node_name, op_name, infos);
    return !ops_info_[op_name][kGray];
  }
  return true;
}
// Determine if the node is performance-sensitive on NPU, this should
// normally be done after calling IsNpuSupported to confirm that the node
// is supported by NPU. To be on the safe side, it internally performs a
// check on whether it is supported by NPU, if not, prints an error log,
// and returns `false`
bool NpuOpsIdentifier::IsPerformanceSensitive(const char *op) {
  return NpuOpsIdentifier::IsPerformanceSensitive(std::string(op));
}
bool NpuOpsIdentifier::IsPerformanceSensitive(const std::string &op) {
  if (ops_info_.find(op) != ops_info_.end()) {
    if (ops_info_[op].is_object()) {
      if (ops_info_[op][kHeavy].is_boolean()) { return ops_info_[op][kHeavy]; }
    }
  }
  return false;
}

#undef LIKELY
#undef UNLIKELY
