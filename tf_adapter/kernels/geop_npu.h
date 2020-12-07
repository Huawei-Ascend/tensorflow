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

#ifndef TENSORFLOW_KERNELS_GEOP_NPU_H_
#define TENSORFLOW_KERNELS_GEOP_NPU_H_

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/tensor_types.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/util/env_var.h"

#include "ge/ge_api.h"
#include "ge/ge_api_types.h"
#include "graph/tensor.h"
#include "graph/utils/graph_utils.h"
#include "toolchain/tuning_tool/tune_api.h"
#include <unordered_map>

namespace tensorflow {
using MsTuningFunc = MsTuneStatus (*)(ge::Graph &, std::vector<ge::Graph> &, ge::Session *,
                      const std::map<std::string, std::map<std::string, std::string>> &);
class GeOp : public AsyncOpKernel {
 public:
  explicit GeOp(OpKernelConstruction *ctx);
  ~GeOp();
  void ComputeAsync(OpKernelContext *ctx, DoneCallback done) override;

 private:
  void Initialize(OpKernelConstruction *ctx);
  void Finalize();

  // global environment Initialize/Finalize, only invoke once for each process
  Status GlobalInitialize(OpKernelConstruction *ctx);
  void GlobalFinalize();

  // Build GraphDef from FunctionDef.
  Status BuildGraphDef(const FunctionLibraryDefinition &flib_def, const FunctionDef &func_def,
                       const std::vector<Tensor> &input_vec, GraphDef &graph_def, bool &is_initialize);

  // prepare input tensor
  Status BuildInputTensorInfo(OpKernelContext *ctx, std::vector<ge::InputTensorInfo> &inputs);

  // prepare output tensor
  Status BuildOutTensorInfo(OpKernelContext *ctx);

  // create input and output desc for NodeDef
  Status GenerateDesc(Node *&node);

  Status DomiFormatFromString(std::string format, int32_t &domi_format);

 private:
  void AddNodeAttrs(Node *node, bool &is_initialize);

  int InitRebuildFlag(uint32_t cache_graph_id);

  bool IncrementGraphIdCount(std::string &tf_session, uint32_t &graph_id);

  bool DecrementGraphIdCount(std::string &tf_session, uint32_t &graph_id);

  void ClearGraphIdCount(std::string &tf_session);

  void GetExecGraphId(OpKernelContext *ctx, uint32_t &cache_graph_id,
                      std::vector<std::string> input_shapes);

  void GetMsTuneConfig(std::map<std::string, std::string> init_options);

  void SetShapesToOutputDesc(const std::vector<std::string> &input_shapes,
                             const int &index, AttrValue &attr_shape_value);

  void BuildShapeNodeAndCacheArgNodes(Graph &graph);

  Status ChangeInputsShapeDesc();

 private:
  static const std::string INPUT_DESC;
  static const std::string OUTPUT_DESC;
  static const std::string SERIALIZE_FORMAT;
  static const std::string SERIALIZE_DATATYPE;
  static const std::string SERIALIZE_SHAPE;
  static const std::string SubGraph;

  static mutex mu_;

  bool init_flag_;
  bool build_flag_;
  bool add_graph_flag_;
  bool sess_init_flag_;
  bool compute_graph_empty_;

  NameAttrList function_;
  std::string data_format_;
  uint32_t graph_id_;
  bool is_initialized_graph_;
  bool need_iteration_;
  std::string tf_session_;
  ge::Session *ge_session_;
  std::string job_type_;
  std::map<std::vector<std::string>, uint32_t> cache_graphs_;
  std::vector<std::pair<std::vector<std::string>, uint32_t>> graph_counts_;
  std::map<std::string, std::string> sess_options_;
  std::map<std::string, std::string> init_options_;
  static std::unordered_map<std::string, uint32_t> session_and_graph_id_map_;
  uint32_t iteration_per_loop_;
  bool is_host_graph_;
  std::map<std::string, std::string> graph_options_;
  string work_path_;
  string mstune_mode_;
  std::map<int, TensorShape> outputs_shape_;
  bool is_train_graph_;
  void *handle_;
  MsTuningFunc tuning_api_;
  string auto_tune_mode_;
  std::vector<Node*> dynamic_shape_nodes_;
};
}  // namespace tensorflow
#endif  // TENSORFLOW_KERNELS_GEOP_NPU_H_
