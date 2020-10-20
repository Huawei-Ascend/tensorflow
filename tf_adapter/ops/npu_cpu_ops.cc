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
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/tensor.pb.h"

namespace tensorflow {
using shape_inference::InferenceContext;
using shape_inference::ShapeHandle;

REGISTER_OP("EmbeddingRankId")
    .Input("addr_table: uint64")
    .Input("index: T")
    .Output("rank_id: uint64")
    .Attr("T: {int64,int32,uint64}")
    .Attr("row_memory: int = 320")
    .Attr("mode: string = 'mod' ")
    .SetAllowsUninitializedInput()
    .SetShapeFn([](shape_inference::InferenceContext *c) {
      auto out_shape = c->MakeShape({c->Dim(c->input(1), 0), c->Dim(c->input(0), 1)});
      c->set_output(0, out_shape);
      return Status::OK();
    })
    .Doc(R"doc(
    Traverse the index calculation server and its position in the server.
    Arguments
        addr_table:    Tensors of addr_table.
        index:    Tensors of index.
    Output
        rank_id:    Tensors with the same shape as index.dim(0)*3.
    )doc");
}  // namespace tensorflow
