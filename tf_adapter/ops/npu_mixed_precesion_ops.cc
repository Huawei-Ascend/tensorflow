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
// Mixed-precisions training
REGISTER_OP("NpuAllocFloatStatus")
    .Output("float_status: float")
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      c->set_output(0, c->Vector(1));
      return Status::OK();
    })
    .Doc(R"doc(
    Allocate the float status tensor for getting float status from scalar buffer.

    Arguments
        inputs: No inputs.

    Output
        output: One float element tensor.
    )doc");

REGISTER_OP("NpuGetFloatStatus")
    .Input("input_float: float")
    .Output("float_status: float")
    .SetShapeFn(shape_inference::UnchangedShape)
    .Doc(R"doc(
    Allocate the float status tensor for getting float status from scalar buffer.

    Arguments
        inputs: The allocated input float status tensor.

    Output
        output: The one float status element tensor.
    )doc");

REGISTER_OP("NpuClearFloatStatus")
    .Input("float_status: float")
    .Output("cleared_float_status: float")
    .SetShapeFn(shape_inference::UnchangedShape)
    .Doc(R"doc(
    Clear the float status in the scalar buffer.

    Arguments
        inputs: The float status tensor.

    Output
        output: The float element tensor set to zero.
    )doc");
}  // namespace tensorflow
