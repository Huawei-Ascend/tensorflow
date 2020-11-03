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

#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/op_kernel.h"

namespace tensorflow {
REGISTER_OP("AscendQuant")
    .Attr("T: {float16, float32, float64}")
    .Attr("quant_bits: int = 8")
    .Attr("scale: float")
    .Attr("offset: float")
    .Input("x: T")
    .Output("y: T")
    .SetIsStateful()
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK(); });

REGISTER_OP("AscendWeightQuant")
    .Attr("T: {float16, float32, float64}")
    .Input("x: int8")
    .Input("offset_w: int8")
    .Output("y: T")
    .SetIsStateful()
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK(); });

REGISTER_OP("AscendDequant")
    .Attr("T: {float16, float32, float64}")
    .Attr("ksize: list(int)")
    .Attr("data_format: string = 'NHWC'")
    .Input("x: T")
    .Input("deq_scale: uint64")
    .Output("y: T")
    .SetIsStateful()
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK(); });

REGISTER_OP("AscendAntiQuant")
    .Attr("T: {float16, float32, float64}")
    .Attr("scale: float")
    .Attr("offset: float")
    .Input("x: T")
    .Output("y: T")
    .SetIsStateful()
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK(); });
}  // namespace tensorflow
