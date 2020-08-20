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

#ifndef TENSORFLOW_CONTRIB_OFFLINE_TRAIN_UTIL_INFERSHAPE_H_
#define TENSORFLOW_CONTRIB_OFFLINE_TRAIN_UTIL_INFERSHAPE_H_

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/common_runtime/shape_refiner.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/tensor_reference.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/graph/graph_constructor.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/common_runtime/optimization_registry.h"
#include "tensorflow/core/public/session_options.h"
#include <sys/time.h>

namespace tensorflow {
const std::string KEY_SHAPE = "geop_shape";

class InferShapeUtil {
 public:
  static Status InferShape(std::vector<Tensor> vecTensor,
                           const FunctionLibraryDefinition *flib_def,
                           const FunctionDef *func_def, Graph *graph);

  static Status getSubGraphFromFunctionDef(const FunctionDef &func_def, Graph *graph);

  static int64 GetCurrentTimestap();
  static bool IsInitializedGraph(Node *node);

  static const int INFER_SHAPE_FIRST_TIME = 0;
  static const int INFER_SHAPE_OTHER_TIME = 1;

 private:
  static Status setArgShapeFromTensorShape(std::vector<Tensor> vecTensor,
                                           Graph *graph, const OpDef &sig,
                                           ShapeRefiner &shapeRef);

  static Status getInputShapesOfNode(
      ShapeRefiner &shapeRef, Node *pNode,
      std::vector<tensorflow::shape_inference::ShapeHandle> &inputShapeVec);

  static void setShapeOfEnterOP(ShapeRefiner &shapeRef, Node *pNode);

  static void setShapeOfMergeOP(ShapeRefiner &shapeRef, Node *pNode);

  static void setShapeOfBroadcastGradientArgsOP(ShapeRefiner &shapeRef,
                                                Node *pNode);

  static void setShapeOfReshapeOP(ShapeRefiner &shapeRef, Node *pNode);

  static void inferShapeOfGraph(const Graph *graph, ShapeRefiner &shapeRef,
                                int iTime);

  static Status addShapeToAttr(ShapeRefiner &shapeRef, Node *pNode);

  static void printGraphShape(ShapeRefiner &shapeRef, Graph *graph);
};
}  // namespace tensorflow
#endif  // TENSORFLOW_CONTRIB_OFFLINE_TRAIN_UTIL_INFERSHAPE_H_
