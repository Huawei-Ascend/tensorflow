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

#ifndef TENSORFLOW_GRAPPLER_OPTIMIZERS_GRADIENT_FUSION_OPTIMIZER_H_
#define TENSORFLOW_GRAPPLER_OPTIMIZERS_GRADIENT_FUSION_OPTIMIZER_H_

#include "tensorflow/core/grappler/optimizers/custom_graph_optimizer_registry.h"
#include "tensorflow/core/grappler/utils.h"

namespace tensorflow {
namespace grappler {
class GradFusionOptimizer : public CustomGraphOptimizer {
 public:
  GradFusionOptimizer() {
    fusionOpInfo_.clear();
    fusionOpPool_.clear();
  }

  ~GradFusionOptimizer() override = default;

  string name() const override { return "GradFusionOptimizer"; }

  Status Init(const tensorflow::RewriterConfig_CustomGraphOptimizer *config) override { return Status::OK(); }

  bool UsesFunctionLibrary() const override { return false; }

  Status Optimize(Cluster *cluster, const GrapplerItem &item, GraphDef *optimizedGraph) override;

  void Feedback(Cluster *cluster, const GrapplerItem &item, const GraphDef &optimizedGraph, double result) override {}

 private:
  bool IsHcomOp(const NodeDef &nodeDef);
  Status GetInputTensorSize(NodeDef &nodeDef, int64_t &inputTensorSize);
  Status FusionOp(std::vector<NodeDef> fusionHcclOps, GraphDef *graphDef);
  Status SetFusionNodeAttr(NodeDef *fusionNode, NodeDef &originNode);
  Status SetHcomBroadcastAttr(NodeDef *fusionNode, NodeDef &originNode);
  int64 GetFusionTensorSize();

  std::unique_ptr<NodeMap> nodeMap_;
  std::map<string, std::vector<std::pair<int, string>>> fusionOpInfo_;
  std::map<string, NodeDef> nameToNode_;
  std::map<string, std::set<string>> fusionOpPool_;
};
}  // end namespace grappler
}  // end namespace tensorflow

#endif  // TENSORFLOW_GRAPPLER_OPTIMIZERS_GRADIENT_FUSION_OPTIMIZER_H_