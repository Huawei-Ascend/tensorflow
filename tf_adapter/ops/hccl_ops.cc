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

#include "tensorflow/core/framework/common_shape_fns.h"
#include "tensorflow/core/framework/op.h"

namespace tensorflow {
using shape_inference::InferenceContext;
using shape_inference::ShapeHandle;

REGISTER_OP("HcomAllReduce")
    .Input("input: T")
    .Output("output: T")
    .Attr("T: {int8, int16, int32, float16, float32}")
    .Attr("reduction: {'min', 'max', 'prod', 'sum'}")
    .Attr("group: string")
    .Attr("fusion: int")
    .Attr("fusion_id: int")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->input(0));
      return Status::OK();
    })
    .Doc(R"doc(
Outputs a tensor containing the reduction across all input tensors passed to ops.

The graph should be constructed so if one op runs with shared_name value `c`,
then `num_devices` ops will run with shared_name value `c`.  Failure to do so
will cause the graph execution to fail to complete.

input: the input to the reduction
output: the value of the reduction across all `num_devices` devices.
reduction: the reduction operation to perform.
group: all devices of the group participating in this reduction.
)doc");

REGISTER_OP("HcomAllGather")
    .Input("input: T")
    .Output("output: T")
    .Attr("T: {int8, int16, int32, float16, float32}")
    .Attr("group: string")
    .Attr("rank_size: int")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      // Scalar input is not supported.
      shape_inference::ShapeHandle unused;
      TF_RETURN_IF_ERROR(c->WithRankAtLeast(c->input(0), 1, &unused));

      shape_inference::ShapeHandle inSubshape;
      TF_RETURN_IF_ERROR(c->Subshape(c->input(0), 1, &inSubshape));

      int rankSize = 0;
      auto inputFirstDimValue = c->Value(c->Dim(c->input(0), 0));
      shape_inference::ShapeHandle outputFirstDimAsShape;
      TF_CHECK_OK(c->GetAttr("rank_size", &rankSize));
      Status rankSizeStatus =
          ((rankSize > 0) ? (Status::OK()) : (errors::InvalidArgument("rankSize should be greater than 0.")));
      TF_CHECK_OK(rankSizeStatus);
      std::vector<shape_inference::DimensionHandle> outputFirstDim;
      outputFirstDim.push_back(c->MakeDim(rankSize * inputFirstDimValue));
      outputFirstDimAsShape = c->MakeShape(outputFirstDim);
      shape_inference::ShapeHandle output;
      TF_RETURN_IF_ERROR(c->Concatenate(outputFirstDimAsShape, inSubshape, &output));
      c->set_output(0, output);
      return Status::OK();
    })
    .Doc(R"doc(

)doc");

REGISTER_OP("HcomBroadcast")
    .Input("input: T")
    .Output("output: T")
    .Attr("T: list(type) >= 0")
    .Attr("group: string")
    .Attr("root_rank: int")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      for (int i = 0; i < c->num_inputs(); i++) { c->set_output(i, c->input(i)); }
      return Status::OK();
    })
    .Doc(R"doc(
Sends `input` to all devices that are connected to the output.

The graph should be constructed so that all ops connected to the output have a
valid device assignment, and the op itself is assigned one of these devices.

input: The input to the broadcast.
output: The same as input.
)doc");

REGISTER_OP("HcomReduceScatter")
    .Input("input: T")
    .Output("output: T")
    .Attr("T: {int8, int16, int32, float16, float32}")
    .Attr("reduction: {'min', 'max', 'prod', 'sum'}")
    .Attr("group: string")
    .Attr("rank_size: int")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      // Scalar input is not supported.
      shape_inference::ShapeHandle unused;
      TF_RETURN_IF_ERROR(c->WithRankAtLeast(c->input(0), 1, &unused));

      shape_inference::ShapeHandle inSubshape;
      TF_RETURN_IF_ERROR(c->Subshape(c->input(0), 1, &inSubshape));

      int rankSize = 0;
      auto inputFirstDimValue = c->Value(c->Dim(c->input(0), 0));
      shape_inference::ShapeHandle outputFirstDimAsShape;
      TF_CHECK_OK(c->GetAttr("rank_size", &rankSize));
      Status rankSizeStatus =
          ((rankSize > 0) ? (Status::OK()) : (errors::InvalidArgument("rank_size should be greater than 0.")));
      TF_CHECK_OK(rankSizeStatus);
      Status outputFirstDimStatus = ((inputFirstDimValue % rankSize) == 0)
          ? (Status::OK())
          : (errors::InvalidArgument("input first dim should be N * rank_size."));
      TF_CHECK_OK(outputFirstDimStatus);
      std::vector<shape_inference::DimensionHandle> outputFirstDim;
      outputFirstDim.push_back(c->MakeDim(inputFirstDimValue / rankSize));
      outputFirstDimAsShape = c->MakeShape(outputFirstDim);
      shape_inference::ShapeHandle output;
      TF_RETURN_IF_ERROR(c->Concatenate(outputFirstDimAsShape, inSubshape, &output));
      c->set_output(0, output);
      return Status::OK();
    })
    .Doc(R"doc(

)doc");

REGISTER_OP("HcomSend")
    .Input("input: T")
    .Attr("T: {int8, int16, int32, float16, float32}")
    .Attr("group: string")
    .Attr("sr_tag: int")
    .Attr("dest_rank: int")
    .SetIsStateful()
    .SetShapeFn(shape_inference::NoOutputs)
    .Doc(R"doc(

)doc");

REGISTER_OP("HcomReceive")
    .Output("output: T")
    .Attr("T: {int8, int16, int32, float16, float32}")
    .Attr("shape: shape")
    .Attr("group: string")
    .Attr("sr_tag: int")
    .Attr("src_rank: int")
    .SetIsStateful()
    .SetShapeFn(shape_inference::ExplicitShape)
    .Doc(R"doc(

)doc");
}  // namespace tensorflow
