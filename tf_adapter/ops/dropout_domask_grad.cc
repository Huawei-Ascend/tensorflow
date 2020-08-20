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

#include "tensorflow/core/framework/function.h"
#include "tensorflow/core/lib/core/errors.h"

namespace tensorflow {

namespace {
using FDH = FunctionDefHelper;

// Cwise binary ops
Status GradForBinaryCwise(FunctionDef *g, std::vector<FDH::Node> body) {
  // clang-format off
  std::vector<FDH::Node> nodes = {
      {{"sx"}, "Shape", {"x"}},
      {{"sy"}, "Shape", {"y"}},
  };
  nodes.insert(nodes.end(), body.begin(), body.end());
  std::vector<FDH::Node> reshapes = {
      {{"rx", "ry"}, "BroadcastGradientArgs", {"sx", "sy"}},
      {{"sum_gx"}, "Sum", {"gx", "rx"}},
      {{"dx"}, "Reshape", {"sum_gx", "sx"}},
      {{"sum_gy"}, "Sum", {"gy", "ry"}},
      {{"dy"}, "Reshape", {"sum_gy", "sy"}},
  };
  nodes.insert(nodes.end(), reshapes.begin(), reshapes.end());

  // clang-format on
  for (auto &n : nodes) {
    // "BroadcastGradientArgs" doesn't need any attrs.
    if (n.attr.empty() && n.op != "BroadcastGradientArgs") {
      n.attr = {{"T", "$T"}};
    }
  }
  *g = FDH::Define(
      // Arg defs
      {"x: T", "y: T", "dz: T"},
      // Ret val defs
      {"dx: T", "dy: T"},
      // Attr defs
      {{"T: {half, float, double}"}},
      // Nodes
      nodes);
  return Status::OK();
}

Status DropOutDoMaskGrad(const AttrSlice &attrs, FunctionDef *g) {
  // clang-format off
  return GradForBinaryCwise(g, {
      {{"dx"}, "Identity", {"dy"}},
  });
  // clang-format on
}
REGISTER_OP_GRADIENT("DropOutDoMask", DropOutDoMaskGrad);
} // namespace
} // namespace tensorflow