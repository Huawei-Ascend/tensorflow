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

#include <memory>
#include <queue>
#include <set>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/common_runtime/optimization_registry.h"
#include "tensorflow/core/framework/graph_to_functiondef.h"
#include "tensorflow/core/framework/node_def_util.h"
#include "tensorflow/core/graph/algorithm.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/graph/graph_constructor.h"
#include "tensorflow/core/graph/node_builder.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/lib/gtl/map_util.h"
#include "tensorflow/core/lib/hash/hash.h"
#include "tensorflow/core/lib/random/random.h"
#include "tensorflow/core/lib/random/simple_philox.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/public/session_options.h"
#include "tf_adapter/util/infershape_util.h"
#include "tf_adapter/util/npu_attrs.h"
#include "tf_adapter/common/common.h"
#include "tensorflow/core/util/dump_graph.h"
#include "tensorflow/core/graph/graph_constructor.h"

namespace tensorflow {
static const int64 kMicrosToMillis = 1000;
static std::atomic<int> graph_run_num(1);
static mutex graph_num_mutex(LINKER_INITIALIZED);

class AddInputPass : public GraphOptimizationPass {
 public:
  AddInputPass() = default;
  ~AddInputPass() = default;
  Status Run(const GraphOptimizationPassOptions &options) override;
};

Status AddInputPass::Run(const GraphOptimizationPassOptions& options) {
  if (options.partition_graphs == nullptr || options.flib_def == nullptr) { // in ps mode : session_options may be null
      return Status::OK();
  }

  for (auto& partition : *options.partition_graphs) {
    std::unique_ptr<Graph>* graph = &partition.second;

    int graph_num;
    {
      mutex_lock lock(graph_num_mutex);
      graph_num = graph_run_num;
      graph_run_num++;
    }
    int64 startTime = InferShapeUtil::GetCurrentTimestap();
    if (graph == nullptr) {
      continue;
    }

    bool findMarkNoNeed = false;
    for (Node *n : graph->get()->nodes()) {
      REQUIRES_NOT_NULL(n);
      if (n->attrs().Find("_NoNeedOptimize")) {
        LOG(INFO) << "Found mark of noneed optimize on node ["
                  << n->name() << "], skip AddInputPass.";
        findMarkNoNeed = true;
        break;
      }
    }
    if (findMarkNoNeed) {
      continue;
    }

    std::map<std::string, std::string> pass_options;
    pass_options = NpuAttrs::GetDefaultPassOptions();

    for (Node *n : graph->get()->nodes()) {
      REQUIRES_NOT_NULL(n);
      if (n->attrs().Find("_NpuOptimizer")) {
        pass_options = NpuAttrs::GetPassOptions(n->attrs());
        break;
      }
    }

    std::string job = pass_options["job"];
    if (job == "ps" || job == "default" || job == "localhost") {
      LOG(INFO) << "job is " << job << " Skip the optimizer : AddInputPass.";
      continue;
    }

    char *need_print = getenv("PRINT_MODEL");

    if (need_print != nullptr && strcmp("1", need_print) == 0) {
      GraphDef ori_graph_def;
      graph->get()->ToGraphDef(&ori_graph_def);
      string ori_model_path = "BeforeSubGraph_Add_Input_";
      string omodel_path = ori_model_path + std::to_string(graph_num) + ".pbtxt";
      Status status_out = WriteTextProto(Env::Default(), omodel_path, ori_graph_def);
    }

    GraphDef graph_def;
    FunctionLibraryDefinition *func_lib = options.flib_def;
    partition.second.get()->ToGraphDef(&graph_def);

    std::unique_ptr<Graph> device_graph(new Graph(OpRegistry::Global()));
    GraphConstructorOptions device_opts;
    // There are internal operations (e.g., send/recv) that we now allow.
    device_opts.allow_internal_ops = true;
    device_opts.expect_device_spec = true;
    TF_RETURN_IF_ERROR(ConvertGraphDefToGraph(device_opts, graph_def, device_graph.get()));
    partition.second.swap(device_graph);

    if (need_print != nullptr && strcmp("1", need_print) == 0) {
      GraphDef omg_graph_def;
      graph->get()->ToGraphDef(&omg_graph_def);
      string tmpmodel_path = "AfterSubGraph_Add_Input_";
      string tmodel_path = tmpmodel_path + std::to_string(graph_num) + ".pbtxt";
      Status status_o = WriteTextProto(Env::Default(), tmodel_path, omg_graph_def);
    }
    int64 endTime = InferShapeUtil::GetCurrentTimestap();
    LOG(INFO) << "AddInputPass subgraph_" << std::to_string(graph_num) << " success. ["
              << ((endTime - startTime) / kMicrosToMillis) << " ms]";
  }

  return Status::OK();
}

REGISTER_OPTIMIZATION(OptimizationPassRegistry::POST_PARTITIONING, 103,
                      AddInputPass);
}  // namespace tensorflow
