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

#include "tf_adapter/optimizers/om_set_var_format_pass.h"

#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/public/session_options.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/npu_attrs.h"

namespace tensorflow {
static const int g_iInputNum = 1;  // the second input
const std::string KEY_NEW_ATTR_NAME = "_var_format";
const std::string KEY_FZ_ATTR_VALUE = "FZ";
const std::string KEY_4D_ATTR_VALUE = "4D";

const std::string KEY_CONV2D_OP_VALUE = "Conv2D";
const std::string KEY_MATMUL_OP_VALUE = "MatMul";
const std::string KEY_CONV2D_BACKPROP_INPUT_VALUE = "Conv2DBackpropInput";
const std::string KEY_VARIABLE_V2_VALUE = "VariableV2";
const std::string KEY_VAR_HANDLE_OP_VALUE = "VarHandleOp";
const std::string KEY_IDENTITY_OP_VALUE = "Identity";
const std::string KEY_READ_VARIABLE_OP_VALUE = "ReadVariableOp";
const std::string KEY_RESOURCE_APPLY_MOMENTUM_OP_VALUE = "ResourceApplyMomentum";
const std::string KEY_APPLY_MOMENTUM_OP_VALUE = "ApplyMomentum";

static void AddNodeVarFormat(Node *node, const string &var_format) {
  if (var_format == KEY_4D_ATTR_VALUE) {
    const AttrValue *attr_value = node->attrs().Find(KEY_NEW_ATTR_NAME);
    if (attr_value == nullptr) { node->AddAttr(KEY_NEW_ATTR_NAME, var_format); }
    return;
  }

  node->AddAttr(KEY_NEW_ATTR_NAME, var_format);
}

Status SetVarFormatPass::AssignApplyMomentumInNodesFormat(Node *node, const string &var_format) {
  if (node == nullptr) { return Status::OK(); }
  for (const Edge *in_edge : node->in_edges()) {
    REQUIRES_NOT_NULL(in_edge);
    Node *src_node = in_edge->src();
    REQUIRES_NOT_NULL(src_node);
    bool is_momentum_op = (in_edge->dst_input() == 1)
        && ((src_node->type_string() == KEY_VAR_HANDLE_OP_VALUE) || (src_node->type_string() == KEY_VARIABLE_V2_VALUE));
    if (is_momentum_op) {
      AddNodeVarFormat(src_node, var_format);

      for (const Edge *var_out : src_node->out_edges()) {
        REQUIRES_NOT_NULL(var_out);
        Node *var_out_node = var_out->dst();
        REQUIRES_NOT_NULL(var_out_node);
        AddNodeVarFormat(var_out_node, var_format);
      }
      break;
    }
  }
  return Status::OK();
}

Status SetVarFormatPass::GetFormat(Node *node, string &format) {
  for (const Edge *out : node->out_edges()) {
    REQUIRES_NOT_NULL(out);
    Node *dst_node = out->dst();
    REQUIRES_NOT_NULL(dst_node);
    bool is_fz_node = (out->dst_input() == 1)
        && ((dst_node->type_string() == KEY_CONV2D_OP_VALUE) || (dst_node->type_string() == KEY_MATMUL_OP_VALUE)
            || (dst_node->type_string() == KEY_CONV2D_BACKPROP_INPUT_VALUE));
    if (is_fz_node) {
      format = KEY_FZ_ATTR_VALUE;
      return Status::OK();
    }
  }
  format = KEY_4D_ATTR_VALUE;
  return Status::OK();
}

Status SetVarFormatPass::AssignFormatToVarOutNodes(Node *node) {
  string var_format = KEY_4D_ATTR_VALUE;
  for (const Edge *out : node->out_edges()) {
    REQUIRES_NOT_NULL(out);
    Node *dst_node = out->dst();
    REQUIRES_NOT_NULL(dst_node);
    bool is_read_var_node =
        (dst_node->type_string() == KEY_IDENTITY_OP_VALUE) || (dst_node->type_string() == KEY_READ_VARIABLE_OP_VALUE);
    if (is_read_var_node) {
      Status status = GetFormat(dst_node, var_format);
      if (!status.ok()) { return status; }
    }
  }

  Node *apply_momentum = nullptr;
  AddNodeVarFormat(node, var_format);
  for (const Edge *out : node->out_edges()) {
    REQUIRES_NOT_NULL(out);
    Node *dst_node = out->dst();
    REQUIRES_NOT_NULL(dst_node);
    AddNodeVarFormat(dst_node, var_format);
    bool is_apply_momentum_node = dst_node->type_string() == KEY_APPLY_MOMENTUM_OP_VALUE
        || dst_node->type_string() == KEY_RESOURCE_APPLY_MOMENTUM_OP_VALUE;
    if (is_apply_momentum_node) { apply_momentum = dst_node; }
  }

  TF_RETURN_IF_ERROR(AssignApplyMomentumInNodesFormat(apply_momentum, var_format));

  return Status::OK();
}

Status SetVarFormatPass::Run(const GraphOptimizationPassOptions &options) {
  Graph *graph_in = (options.graph)->get();
  if (graph_in == nullptr || options.session_options == nullptr) { return Status::OK(); }

  std::map<std::string, std::string> pass_options = NpuAttrs::GetPassOptions(options);
  std::string job = pass_options["job"];
  if (job == "ps" || job == "default") {
    LOG(INFO) << "job is " << job << " Skip the optimizer : SetVarFormatPass.";
    return Status::OK();
  }

  for (Node *node : graph_in->op_nodes()) {
    if ((node != nullptr)
        && ((node->type_string() == KEY_VAR_HANDLE_OP_VALUE) || (node->type_string() == KEY_VARIABLE_V2_VALUE))) {
      AssignFormatToVarOutNodes(node);
    }
  }
  return Status::OK();
}

REGISTER_OPTIMIZATION(OptimizationPassRegistry::PRE_PLACEMENT, 1, SetVarFormatPass);
}  // namespace tensorflow
