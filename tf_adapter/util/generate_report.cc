/* Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved. foss@huawei.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

#include "nlohmann/json.hpp"
#include "tensorflow/core/platform/env.h"
#include "tf_adapter/util/generate_report.h"

namespace tensorflow {
using Json = nlohmann::json;
const static uint32_t kInterval = 2;
const static std::string kUnsupportedInfoPath = "checkresult.tf.json";

// json file keys
const static std::string kKeyName = "name";
const static std::string kKeyOp = "op";
const static std::string kKeyCode = "code";
const static std::string kKeyType = "type";
const static std::string kKeyReason = "not_support_reason";
const static std::string kKeyIsSupport = "is_support";
const static std::string kKeyMessage = "message";

GenerateReport::GenerateReport() {
  char *need_save = std::getenv("ENABLE_NETWORK_ANALYSIS");
  if (need_save != nullptr && strcmp("1", need_save) == 0) {
    save_report_ = true;
  }
}

GenerateReport *GenerateReport::GetInstance() {
  static GenerateReport generate_report;
  return &generate_report;
}

Status GenerateReport::AddUnSupportedInfo(Node *node, Details &infos) {
  return GenerateReport::AddUnSupportedInfo(node->name(), node->type_string(), infos);
}
Status GenerateReport::AddUnSupportedInfo(const std::string &name, const std::string &type, Details &infos) {
  if (!save_report_) {
    return Status::OK();
  }

  if (check_info_map_.find(name) != check_info_map_.end()) {
    return Status::OK();
  } else {
    UnSupportedInfo unsupported_info;
    unsupported_info.name = name;
    unsupported_info.type = type;
    unsupported_info.info_details = infos;
    check_info_map_[name] = unsupported_info;
  }
  return Status::OK();
}

Status GenerateReport::DeleteUnSupportedInfo(Node *node) {
  if (!save_report_) {
    return Status::OK();
  }
  auto info_iter = check_info_map_.find(node->name());
  if (info_iter == check_info_map_.end()) {
    return Status::OK();
  } else {
    check_info_map_.erase(info_iter);
  }
  return Status::OK();
}

Status GenerateReport::SaveUnsupportedInfo() {
  if (!save_report_) {
    return Status::OK();
  }
  Json graph_info;
  for (auto info : check_info_map_) {
    Json reason = {{kKeyCode, info.second.info_details.code}, {kKeyMessage, info.second.info_details.message}};
    Json op = {{kKeyName, info.second.name},
               {kKeyType, info.second.type},
               {kKeyIsSupport, info.second.is_support},
               {kKeyReason, reason}};
    graph_info[kKeyOp].push_back(op);
  }
  std::string info_str;
  try {
    info_str = graph_info.dump(kInterval, ' ', false, Json::error_handler_t::ignore);
  } catch (std::exception &e) {
    return errors::Internal("Failed to convert json to string ,reason:", e.what());
  } catch (...) {
    return errors::Internal("Failed to convert json to string.");
  }
  return tensorflow::WriteStringToFile(Env::Default(), kUnsupportedInfoPath, info_str);
}
}  // namespace tensorflow
