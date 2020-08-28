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

#include <deque>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>

#include "tensorflow/compiler/jit/graphcycles/graphcycles.h"
#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/common_runtime/optimization_registry.h"
#include "tensorflow/core/common_runtime/shape_refiner.h"
#include "tensorflow/core/framework/graph_def_util.h"
#include "tensorflow/core/public/session_options.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/infershape_util.h"
#include "tf_adapter/util/npu_attrs.h"

namespace tensorflow {
static const int64 kMicrosToMillis = 1000;
static std::atomic<int> graph_run_num(1);
static mutex graph_num_mutex(LINKER_INITIALIZED);

std::set<string> string_split(const string &str, const string &pattern) {
  std::set<string> resultSet;
  string::size_type pos1, pos2;
  pos2 = str.find(pattern);
  pos1 = 0;
  while (pos2 != string::npos) {
    resultSet.insert(str.substr(pos1, pos2 - pos1));
    pos1 = pos2 + pattern.size();
    pos2 = str.find(pattern, pos1);
  }
  if (pos1 != str.length()) { resultSet.insert(str.substr(pos1)); }
  return resultSet;
}

class MarkStartNodePass : public GraphOptimizationPass {
 public:
  MarkStartNodePass() = default;
  ~MarkStartNodePass() override = default;
  Status Run(const GraphOptimizationPassOptions &options) override;
  Status TraverseNode(Node *start_node);
};

Status MarkStartNodePass::Run(const GraphOptimizationPassOptions &options) {
  int graph_num;
  graph_num = graph_run_num++;

  if (options.graph == nullptr || options.flib_def == nullptr || options.session_options == nullptr) {
    return Status::OK();
  }

  std::map<std::string, std::string> pass_options = NpuAttrs::GetPassOptions(options);
  std::string job = pass_options["job"];
  if (job == "ps" || job == "default" || job == "localhost") {
    LOG(INFO) << "job is " << job << " Skip the optimizer : MarkStartNodePass.";
    return Status::OK();
  }

  std::unique_ptr<Graph> *graph = options.graph;

  for (Node *n : graph->get()->nodes()) {
    REQUIRES_NOT_NULL(n);
    if (n->attrs().Find("_NoNeedOptimize")) {
      LOG(INFO) << "Found mark of noneed optimize on node [" << n->name() << "], skip MarkStartNodePass.";
      return Status::OK();
    }

    if (n->attrs().Find("_StartNodeName")) {
      LOG(INFO) << "Found mark of startnode optimize on node [" << n->name() << "], skip MarkStartNodePass.";
      return Status::OK();
    }
  }

  int64 startTime = InferShapeUtil::GetCurrentTimestap();
  char *need_print = getenv("PRINT_MODEL");

  if (need_print != nullptr && strcmp("1", need_print) == 0) {
    GraphDef ori_graph_def;
    graph->get()->ToGraphDef(&ori_graph_def);
    string ori_model_path = "BeforeMarkStartNodeAttr_";
    string omodel_path = ori_model_path + std::to_string(graph_num) + ".pbtxt";
    Status status_out = WriteTextProto(Env::Default(), omodel_path, ori_graph_def);
  }

  for (Node *start_node : graph->get()->nodes()) {
    REQUIRES_NOT_NULL(start_node);
    std::string src_device_name = start_node->assigned_device_name();
    if (!src_device_name.empty() && src_device_name.find("/job:ps") == std::string::npos) {
      for (Node *n : start_node->out_nodes()) {
        std::string device_name = n->assigned_device_name();
        if (device_name.find("/job:ps") != std::string::npos) {
          std::string start_node_name;
          std::set<string> start_nodes_name;
          if (start_node->attrs().Find("_StartNodeName") != nullptr) {
            start_nodes_name = string_split(start_node->attrs().Find("_StartNodeName")->s(), ";");
          }
          start_nodes_name.insert(start_node->name());
          for (const auto &name : start_nodes_name) {
            start_node_name += name;
            start_node_name += ";";
          }
          start_node->AddAttr("_StartNodeName", start_node_name);

          if (n->attrs().Find("_StartNodeName") != nullptr) {
            std::set<string> nodes_name = string_split(n->attrs().Find("_StartNodeName")->s(), ";");
            for (const auto &name : nodes_name) { start_nodes_name.insert(name); }
          }
          for (const auto &name : start_nodes_name) {
            start_node_name += name;
            start_node_name += ";";
          }
          n->AddAttr("_StartNodeName", start_node_name);
          Status s = TraverseNode(n);
          if (s != Status::OK()) { return s; }
        }
      }
    }
  }

  if (need_print != nullptr && strcmp("1", need_print) == 0) {
    GraphDef omg_graph_def;
    graph->get()->ToGraphDef(&omg_graph_def);
    string tmpmodel_path = "AfterMarkStartNodeAttr_";
    string tmodel_path = tmpmodel_path + std::to_string(graph_num) + ".pbtxt";
    Status status_o = WriteTextProto(Env::Default(), tmodel_path, omg_graph_def);
  }
  int64 endTime = InferShapeUtil::GetCurrentTimestap();
  LOG(INFO) << "MarkStartNodePass_" << std::to_string(graph_num) << " success. ["
            << ((endTime - startTime) / kMicrosToMillis) << " ms]";

  return Status::OK();
}

Status MarkStartNodePass::TraverseNode(Node *start_node) {
  Status s = Status::OK();
  for (Node *n : start_node->out_nodes()) {
    REQUIRES_NOT_NULL(n);
    std::string start_node_name;
    std::set<string> start_nodes_name;
    start_nodes_name = string_split(start_node->attrs().Find("_StartNodeName")->s(), ";");

    if (n->attrs().Find("_StartNodeName") != nullptr) {
      std::set<string> nodes_name = string_split(n->attrs().Find("_StartNodeName")->s(), ";");
      for (const auto &name : nodes_name) { start_nodes_name.insert(name); }
    }
    for (const auto &name : start_nodes_name) {
      start_node_name += name;
      start_node_name += ";";
    }
    n->AddAttr("_StartNodeName", start_node_name);
    s = TraverseNode(n);
    if (s != Status::OK()) {
      LOG(INFO) << "traverse node : " << start_node->name() << " to add start node name failed.";
      return s;
    }
  }
  return Status::OK();
}

REGISTER_OPTIMIZATION(OptimizationPassRegistry::POST_REWRITE_FOR_EXEC, 0, MarkStartNodePass);
}  // namespace tensorflow
