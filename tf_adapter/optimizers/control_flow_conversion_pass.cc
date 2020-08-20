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

#include "tf_adapter/optimizers/control_flow_conversion_pass.h"

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/common_runtime/lower_case_op.h"
#include "tensorflow/core/common_runtime/lower_function_call_op.h"
#include "tensorflow/core/common_runtime/lower_if_op.h"
#include "tensorflow/core/common_runtime/lower_while_op.h"
#include "tensorflow/core/framework/node_def_builder.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/graph/node_builder.h"
#include "tensorflow/core/public/session_options.h"
#include "tf_adapter/util/npu_attrs.h"

namespace tensorflow {
namespace {
const string kLowerUsingSwitchMergeAttr = "_lower_using_switch_merge";
}  // namespace

Status ControlFlowConvertionPass::Run(
    const GraphOptimizationPassOptions& options) {
  if (options.partition_graphs != nullptr) {
    return errors::Internal(
        "Lowering If/While ops should happen before partitioning.");
  }
  if (options.graph == nullptr || options.session_options == nullptr) {
    return Status::OK();
  }

  Graph* graph = options.graph->get();
  if (graph == nullptr) {
    return errors::Internal(
        "Lowering While op requires a graph to be available.");
  }

  std::map<std::string, std::string> pass_options = NpuAttrs::GetPassOptions(options);
  std::string job = pass_options["job"];
  if (job == "ps" || job == "default") {
    LOG(INFO) << "job is " << job << " Skip the optimizer : ControlFlowConvertionPass.";
    return Status::OK();
  }

  FunctionLibraryDefinition* flib_def = options.flib_def;
  if (flib_def == nullptr) {
    return errors::Internal(
        "Lowering If op requires a FunctionLibraryDefinition to be available.");
  }

  bool use_off_line = pass_options["use_off_line"] == "1";
  bool lower_functional_ops = pass_options["lower_functional_ops"] == "1";
  if (!use_off_line || lower_functional_ops) {
    LOG(INFO) << "Skip the optimizer";
    return Status::OK();
  }

  // Delete _lower_using_switch_merge before LowerFunctionalOpsPass
  for (int i = 2; i < graph->num_node_ids(); ++i) {
    Node* n = graph->FindNodeId(i);
    if (n->IsIfNode()) {
      n->ClearAttr(kLowerUsingSwitchMergeAttr);
    } else if (n->type_string() == "Case") {
      n->ClearAttr(kLowerUsingSwitchMergeAttr);
    } else if (n->IsWhileNode()) {
      n->ClearAttr(kLowerUsingSwitchMergeAttr);
    }
  }

  std::vector<string> function_names = flib_def->ListFunctionNames();
  for (string func_name : function_names) {
    const FunctionDef* fdef = flib_def->Find(func_name);
    if (fdef != nullptr) {
      for (NodeDef ndef : fdef->node_def()) {
        if (ndef.op() == "If" || ndef.op() == "Case" || ndef.op() == "While") {
          ndef.mutable_attr()->erase(kLowerUsingSwitchMergeAttr);
        }
      }
    }
  }

  return Status::OK();
}

REGISTER_OPTIMIZATION(OptimizationPassRegistry::PRE_PLACEMENT, -1,
                      ControlFlowConvertionPass);
}  // namespace tensorflow
