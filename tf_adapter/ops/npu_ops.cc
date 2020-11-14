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
using shape_inference::DimensionHandle;
using shape_inference::InferenceContext;
using shape_inference::ShapeHandle;
using shape_inference::UnchangedShape;

namespace {
REGISTER_OP("NPUTest").SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("GeOp")
    .Input("inputs: Tin")
    .Attr("Tin: list(type) >= 0")
    .Output("outputs: Tout")
    .Attr("Tout: list(type) >= 0")
    .Attr("function: func")
    .Attr("data_format: { 'NHWC', 'NCHW', 'NDHWC', 'NCDHW', 'DHWCN', 'DHWNC'} = 'NHWC'")
    .SetIsStateful();

REGISTER_OP("DPOP")
    .Input("inputs: Tin")
    .Attr("Tin: list(type) >= 0")
    .Output("outputs: Tout")
    .Attr("Tout: list(type) >= 0")
    .Attr("function: func")
    .Attr("data_format: { 'NHWC', 'NCHW'} = 'NHWC'")
    .SetIsStateful();

REGISTER_OP("NPUInit").SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("LogTimeStamp").Attr("logid: string").Attr("notify: bool").SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("NPUShutdown").SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("LARS")
    .Input("inputs_w: T")
    .Input("inputs_g: T")
    .Input("weight_decay: float")
    .Output("outputs: T")
    .Attr("T: list(type) >= 1")
    .Attr("hyperpara: float = 0.001")
    .Attr("epsilon: float = 0.00001")
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      for (int i = 0; i < ((c->num_inputs() - 1) / 2); i++) { c->set_output(i, c->input(i)); }
      return Status::OK();
    })
    .Doc(R"doc(
    Perform Lars on multi tensors. inputs_g have the same shape as `inputs_w`.

    Arguments
        inputs_w:    Tensors of weight.
        inputs_g:    Tensors of gradient.

    Output
        outputs:    Tensors with the same shape as `inputs_w`.
    )doc");

REGISTER_OP("LarsV2")
    .Input("input_weight: T")
    .Input("input_grad: T")
    .Input("weight_decay: T")
    .Input("learning_rate: T")
    .Output("output: T")
    .Attr("T: {float}")
    .Attr("hyperpara: float = 0.001")
    .Attr("epsilon: float = 0.00001")
    .Attr("use_clip: bool = false")
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->input(0));
      return Status::OK();
    })
    .Doc(R"doc(
    Perform LarsV2 on single output. input_weight have the same shape
    as `input_grad`.

    Arguments
        input_weight:    Tensor of weight.
        input_grad:      Tensor of gradient.
        weight_decay:    Tensor of weight_decay.
        learning_rate:   Tensor of learning_rate.
        use_clip:        Indicates whether to limit the coeff to acertain range.

    Output
        output:    Tensor with the same shape as `input_weight`.
    )doc");

Status OutfeedDequeueShapeFn(shape_inference::InferenceContext *c) {
  std::vector<PartialTensorShape> output_shapes;
  TF_RETURN_IF_ERROR(c->GetAttr("output_shapes", &output_shapes));
  if (static_cast<int>(output_shapes.size()) != c->num_outputs()) {
    return errors::InvalidArgument("`output_shapes` must be the same length as `output_types` (", output_shapes.size(),
                                   " vs. ", c->num_outputs());
  }
  for (size_t i = 0; i < output_shapes.size(); ++i) {
    shape_inference::ShapeHandle output_shape_handle;
    TF_RETURN_IF_ERROR(c->MakeShapeFromPartialTensorShape(output_shapes[i], &output_shape_handle));
    c->set_output(static_cast<int>(i), output_shape_handle);
  }
  return Status::OK();
}

REGISTER_OP("OutfeedEnqueueOp")
    .Input("inputs: Tin")
    .Attr("channel_name: string")
    .Attr("Tin: list(type) >= 0")
    .SetIsStateful()
    .SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("OutfeedDequeueOp")
    .Output("outputs: output_types")
    .Attr("channel_name: string")
    .Attr("output_types: list(type) >= 1")
    .Attr("output_shapes: list(shape) >= 1")
    .SetIsStateful()
    .SetShapeFn(OutfeedDequeueShapeFn);

REGISTER_OP("StopOutfeedDequeueOp").Attr("channel_name: string").SetIsStateful().SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("DropOutDoMask")
    .Input("x: T")
    .Input("mask: uint8")
    .Input("keep_prob: T")
    .Output("y: T")
    .Attr("T: {float16, float32}")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->input(0));
      return Status::OK();
    });

REGISTER_OP("DropOutGenMask")
    .Input("shape: T")
    .Attr("T: {int64, int32}")
    .Input("prob: S")
    .Attr("S: {float, half}")
    .Output("output: uint8")
    .Attr("seed: int = 0")
    .Attr("seed2: int = 0")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      ShapeHandle unused;
      TF_RETURN_IF_ERROR(c->WithRankAtMost(c->input(1), 0, &unused));  // prob must be 0-d

      ShapeHandle inputShapeHandle;
      TF_RETURN_IF_ERROR(c->MakeShapeFromShapeTensor(0, &inputShapeHandle));

      int32 rank = c->Rank(inputShapeHandle);
      if (InferenceContext::kUnknownRank == rank) {
        ShapeHandle out = c->UnknownShapeOfRank(1);
        c->set_output(0, out);
        return Status::OK();
      }

      bool unknownDimExist = false;
      for (int32 i = 0; i < rank; ++i) {
        DimensionHandle dimHandle = c->Dim(inputShapeHandle, i);
        int64 value = c->Value(dimHandle);
        if (InferenceContext::kUnknownDim == value) {
          unknownDimExist = true;
          break;
        }
      }

      if (unknownDimExist) {
        ShapeHandle out = c->UnknownShapeOfRank(1);
        c->set_output(0, out);
        return Status::OK();
      }

      int64 bitCount = 0;
      if (rank != 0) {
        DimensionHandle inputDimHandle = c->NumElements(inputShapeHandle);
        bitCount = c->Value(inputDimHandle);
      }

      // align to 128 and around up
      int64 n128Bits = bitCount / 128;
      if ((bitCount % 128) != 0) { n128Bits++; }

      // transfer 128 bit count to byte count if shape is full know
      int64 nBytes = n128Bits * 16;

      ShapeHandle out = c->Vector(nBytes);
      c->set_output(0, out);
      return Status::OK();
    });

REGISTER_OP("BasicLSTMCell")
    .Input("x: T")
    .Input("h: T")
    .Input("c: T")
    .Input("w: T")
    .Input("b: T")
    .Output("ct: T")
    .Output("ht: T")
    .Output("it: T")
    .Output("jt: T")
    .Output("ft: T")
    .Output("ot: T")
    .Output("tanhct: T")
    .Attr("T: {float16, float32}")
    .Attr("keep_prob: float = 1.0")
    .Attr("forget_bias: float = 1.0")
    .Attr("state_is_tuple: bool = true")
    .Attr("activation: string = 'tanh'")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->input(2));
      c->set_output(1, c->input(1));
      c->set_output(2, c->input(2));
      c->set_output(3, c->input(2));
      c->set_output(4, c->input(2));
      c->set_output(5, c->input(2));
      c->set_output(6, c->input(2));
      return Status::OK();
    });

REGISTER_OP("BasicLSTMCellCStateGrad")
    .Input("c: T")
    .Input("dht: T")
    .Input("dct: T")
    .Input("it: T")
    .Input("jt: T")
    .Input("ft: T")
    .Input("ot: T")
    .Input("tanhct: T")
    .Output("dgate: T")
    .Output("dct_1: T")
    .Attr("T: {float16, float32}")
    .Attr("forget_bias: float = 1.0")
    .Attr("activation: string = 'tanh'")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      auto input_it_shape = c->input(4);
      auto hidden_size = c->Dim(input_it_shape, 1);
      auto batch_size = c->Dim(input_it_shape, 0);
      DimensionHandle output_size;
      TF_RETURN_IF_ERROR(c->Multiply(hidden_size, 4, &output_size));
      auto output_shape = c->MakeShape({batch_size, output_size});
      c->set_output(0, output_shape);
      c->set_output(1, c->input(2));
      return Status::OK();
    });

REGISTER_OP("BasicLSTMCellWeightGrad")
    .Input("x: T")
    .Input("h: T")
    .Input("dgate: T")
    .Output("dw: T")
    .Output("db: T")
    .Attr("T: {float16, float32}")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      auto input_x_shape = c->input(0);
      auto input_h_shape = c->input(1);
      auto input_dgate_shape = c->input(2);
      auto four_hidden_size = c->Dim(input_dgate_shape, 1);
      auto hidden_size = c->Dim(input_h_shape, 1);
      auto input_size = c->Dim(input_x_shape, 1);
      DimensionHandle output_size;
      TF_RETURN_IF_ERROR(c->Add(hidden_size, input_size, &output_size));
      auto output_dw_shape = c->MakeShape({output_size, four_hidden_size});
      auto output_db_shape = c->MakeShape({four_hidden_size});
      c->set_output(0, output_dw_shape);
      c->set_output(1, output_db_shape);
      return Status::OK();
    });

REGISTER_OP("BasicLSTMCellInputGrad")
    .Input("dgate: T")
    .Input("w: T")
    .Output("dxt: T")
    .Output("dht: T")
    .Attr("T: {float16, float32}")
    .Attr("keep_prob: float = 1.0")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      auto input_dgate_shape = c->input(0);
      auto input_w_shape = c->input(1);
      auto four_hidden_size = c->Dim(input_dgate_shape, 1);
      auto batch_size = c->Dim(input_dgate_shape, 0);
      auto input_hidden_size = c->Dim(input_w_shape, 0);
      DimensionHandle output_hidden_size;
      TF_RETURN_IF_ERROR(c->Divide(four_hidden_size, 4, true, &output_hidden_size));
      auto output_dht_shape = c->MakeShape({batch_size, output_hidden_size});
      DimensionHandle output_input_size;
      TF_RETURN_IF_ERROR(c->Subtract(input_hidden_size, output_hidden_size, &output_input_size));
      auto output_dxt_shape = c->MakeShape({batch_size, output_input_size});
      c->set_output(0, output_dxt_shape);
      c->set_output(1, output_dht_shape);
      return Status::OK();
    });

REGISTER_OP("DecodeAndResizeJpeg")
    .Input("contents: string")
    .Input("resize: int32")
    .Output("image: uint8")
    .SetShapeFn([](InferenceContext *c) {
      const Tensor *rezise = c->input_tensor(1);
      DimensionHandle h;
      DimensionHandle w;
      if (rezise != nullptr) {
        auto rezise_vec = rezise->vec<int32>();
        h = c->MakeDim(rezise_vec(0));
        w = c->MakeDim(rezise_vec(1));
      }
      c->set_output(0, c->MakeShape({h, w, 3}));
      return Status::OK();
    });

REGISTER_OP("DecodeAndCropAndResizeJpeg")
    .Input("contents: string")
    .Input("crop_size: int32")
    .Input("resize: int32")
    .Output("image: uint8")
    .SetShapeFn([](InferenceContext *c) {
      const Tensor *rezise = c->input_tensor(2);
      DimensionHandle h;
      DimensionHandle w;
      if (rezise != nullptr) {
        auto rezise_vec = rezise->vec<int32>();
        h = c->MakeDim(rezise_vec(0));
        w = c->MakeDim(rezise_vec(1));
      }
      c->set_output(0, c->MakeShape({h, w, 3}));
      return Status::OK();
    });

REGISTER_OP("AdamApplyOneAssign")
    .Input("input0: T")
    .Input("input1: T")
    .Input("input2: T")
    .Input("input3: T")
    .Input("input4: T")
    .Input("mul0_x: T")
    .Input("mul1_x: T")
    .Input("mul2_x: T")
    .Input("mul3_x: T")
    .Input("add2_y: T")
    .Attr("T: {float16, float32}")
    .SetShapeFn(shape_inference::NoOutputs);

REGISTER_OP("LambApplyOptimizerAssign")
    .Input("input0: T")
    .Input("input1: T")
    .Input("input2: T")
    .Input("input3: T")
    .Input("mul0_x: T")
    .Input("mul1_x: T")
    .Input("mul2_x: T")
    .Input("mul3_x: T")
    .Input("add2_y: T")
    .Input("steps: T")
    .Input("do_use_weight: T")
    .Input("weight_decay_rate: T")
    .Output("update: T")
    .Output("output1: T")
    .Output("output2: T")
    .Attr("T: {float16, float32}")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->input(0));
      c->set_output(1, c->input(1));
      c->set_output(2, c->input(2));
      return Status::OK();
    });

REGISTER_OP("LambApplyWeightAssign")
    .Input("input0: T")
    .Input("input1: T")
    .Input("input2: T")
    .Input("input3: T")
    .Input("input4: T")
    .Output("output0: T")
    .Attr("T: {float16, float32}")
    .SetIsStateful()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->input(4));
      return Status::OK();
    });

REGISTER_OP("AdamApplyOneWithDecayAssign")
    .Input("input0: T")
    .Input("input1: T")
    .Input("input2: T")
    .Input("input3: T")
    .Input("input4: T")
    .Input("mul0_x: T")
    .Input("mul1_x: T")
    .Input("mul2_x: T")
    .Input("mul3_x: T")
    .Input("mul4_x: T")
    .Input("add2_y: T")
    .Attr("T: {float16, float32}")
    .SetShapeFn(shape_inference::NoOutputs);
}  // namespace
}  // namespace tensorflow
