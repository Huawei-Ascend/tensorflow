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

#ifndef TENSORFLOW_NPU_ATTRS_H_
#define TENSORFLOW_NPU_ATTRS_H_

#include "ge/ge_api_types.h"
#include "tensorflow/core/common_runtime/optimization_registry.h"
#include "tensorflow/core/framework/attr_value.pb.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/public/session_options.h"
#include <map>
#include <string>

// single load all npu mode
namespace tensorflow {
Status GetEnvDeviceID(uint32_t &device_id);
class NpuAttrs {
 public:
  // This method returns instance Pointers
  static std::map<std::string, std::string> GetInitOptions(OpKernelConstruction *ctx);
  static std::map<std::string, std::string> GetDefaultInitOptions();
  static std::map<std::string, std::string> GetSessOptions(OpKernelConstruction *ctx);
  static std::map<std::string, std::string> GetPassOptions(const GraphOptimizationPassOptions &options);
  static std::map<std::string, std::string> GetPassOptions(OpKernelConstruction *ctx);
  static std::map<std::string, std::string> GetPassOptions(AttrSlice attrs);
  static std::map<std::string, std::string> GetAllAttrOptions(AttrSlice attrs);
  static std::map<std::string, std::string> GetDefaultPassOptions();
  static Status SetNpuOptimizerAttr(const GraphOptimizationPassOptions &options, Node *node);
};
}  // namespace tensorflow

#endif
