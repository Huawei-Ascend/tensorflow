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

#ifndef TENSORFLOW_OM_SET_VAR_FORMAT_PASS_H_
#define TENSORFLOW_OM_SET_VAR_FORMAT_PASS_H_

#include "tensorflow/core/common_runtime/optimization_registry.h"
#include "tensorflow/core/framework/function.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/lib/core/status.h"

namespace tensorflow {
class SetVarFormatPass : public GraphOptimizationPass {
 public:
  Status Run(const GraphOptimizationPassOptions &options) override;
  Status AssignFormatToVarOutNodes(Node *node);
  Status GetFormat(Node *node, string &format);
  Status AssignApplyMomentumInNodesFormat(Node *node, const string &var_format);
};
}    // namespace tensorflow
#endif // TENSORFLOW_OM_SET_VAR_FORMAT_PASS_H_
