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

#ifndef TENSORFLOW_OM_PARTITION_SUBGRAPHS_PASS_H_
#define TENSORFLOW_OM_PARTITION_SUBGRAPHS_PASS_H_

#include "tensorflow/core/common_runtime/optimization_registry.h"
#include "tensorflow/core/framework/function.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/lib/core/status.h"

namespace tensorflow {
namespace OMSplitter {
Status MarkForPartition(const GraphOptimizationPassOptions &options, int &clusterNum, bool mix_compile_mode,
                        int graph_num, FunctionLibraryDefinition *func_lib,
                        std::map<std::string, std::string> pass_options);

// Transformation that finds subgraphs whose nodes are marked with
// 'groupAttribute', splits those subgraphs into functions, and replaces
// the originals with GEOps.
// 'groupAttribute' must be a string valued-attribute that names the new
// functions to introduce.
Status OMPartitionSubgraphsInFunctions(string groupAttribute, const GraphOptimizationPassOptions &options,
                                       string graph_format);

bool IsNpuSupportingNode(const NodeDef &node_def, bool mix_compile_mode, FunctionLibraryDefinition *func_lib);
bool IsNpuSupportingNode(Node *node, bool mix_compile_mode, FunctionLibraryDefinition *func_lib);
}  // namespace OMSplitter

class OMPartitionSubgraphsPass : public GraphOptimizationPass {
 public:
  OMPartitionSubgraphsPass() = default;
  ~OMPartitionSubgraphsPass() override = default;
  Status Run(const GraphOptimizationPassOptions &options) override;

 private:
  Status ProcessGraph(std::unique_ptr<Graph> *graph, FunctionLibraryDefinition *func_lib,
                      const OptimizationPassRegistry::Grouping pass_group_value);
};
}  // namespace tensorflow
#endif  // TENSORFLOW_OM_PARTITION_SUBGRAPHS_PASS_H_
