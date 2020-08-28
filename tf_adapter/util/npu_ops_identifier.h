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

#ifndef TENSORFLOW_NPU_OPS_IDENTIFIER_H_
#define TENSORFLOW_NPU_OPS_IDENTIFIER_H_

#include "nlohmann/json_fwd.hpp"
#include <string>

// Sigleton class for check weather or not a tensorflow op is supported by NPU,
// and, weather or not a tensorflow op is performance sensitive on NPU.
class NpuOpsIdentifier {
 public:
  // This method returns different instance Pointers in mixed mode and in the full sink model
  static NpuOpsIdentifier *GetInstance(bool is_mix = false);
  // Determine if the node is supported by NPU. Note that it will behave
  // differently in mixed mode and full sink mode
  bool IsNpuSupported(const char *op_name, const std::string &node_name);
  bool IsNpuSupported(const std::string &op_name, const std::string &node_name);
  // Determine if the node is performance-sensitive on NPU, this should
  // normally be done after calling IsNpuSupported to confirm that the node
  // is supported by NPU. To be on the safe side, it internally performs a
  // check on whether it is supported by NPU, if not, prints an error log,
  // and returns `false`
  bool IsPerformanceSensitive(const char *op);
  bool IsPerformanceSensitive(const std::string &op);

 private:
  NpuOpsIdentifier(bool is_mix, nlohmann::json &json);
  ~NpuOpsIdentifier() = default;
  // Parse and store the ops configuration json file, return num of parsed ops
  int32_t ParseOps(const std::string &f, nlohmann::json &root);
  const bool is_mix_;
  nlohmann::json &ops_info_;
};

#endif
