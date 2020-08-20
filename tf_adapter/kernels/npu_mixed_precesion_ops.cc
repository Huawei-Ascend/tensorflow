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

#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/register_types.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/framework/bounds_check.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/platform/logging.h"

namespace tensorflow {
// Mixed-precesions training
class NpuAllocFloatStatusOp : public tensorflow::OpKernel {
 public:
  explicit NpuAllocFloatStatusOp(tensorflow::OpKernelConstruction *context)
      : OpKernel(context) {}
  ~NpuAllocFloatStatusOp() {}
  void Compute(tensorflow::OpKernelContext *context) override {
    // Create an output tensor
    Tensor *output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, TensorShape({1}),
                                                     &output_tensor));
    // Set the float_status tensor to be 0
    auto flat = output_tensor->flat<float>();
    flat(0) = 0.0;
  }
};

REGISTER_KERNEL_BUILDER(
    Name("NpuAllocFloatStatus")
.
Device(tensorflow::DEVICE_CPU),
    NpuAllocFloatStatusOp
);

class NpuGetFloatStatusOp : public tensorflow::OpKernel {
 public:
  explicit NpuGetFloatStatusOp(tensorflow::OpKernelConstruction *context)
      : OpKernel(context) {}
  ~NpuGetFloatStatusOp() {}
  void Compute(tensorflow::OpKernelContext *context) override {
    // Grab the input tensor
    const Tensor &input_tensor = context->input(0);

    // Create an output tensor
    Tensor *output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
    // Set the float_status tensor to be 0
    auto flat = output_tensor->flat<float>();
    // For testing
    flat(0) = 0.0;
  }
};

REGISTER_KERNEL_BUILDER(
    Name("NpuGetFloatStatus")
.
Device(tensorflow::DEVICE_CPU),
    NpuGetFloatStatusOp
);

class NpuClearFloatStatusOp : public tensorflow::OpKernel {
 public:
  explicit NpuClearFloatStatusOp(tensorflow::OpKernelConstruction *context)
      : OpKernel(context) {}
  ~NpuClearFloatStatusOp() {}
  void Compute(tensorflow::OpKernelContext *context) override {
    // Grab the input tensor
    const Tensor &input_tensor = context->input(0);
    auto input = input_tensor.flat<float>();

    // Create an output tensor
    Tensor *output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));

    // Clear the status
    auto flat = output_tensor->flat<float>();
    // For testing
    for (int i = 0; i < input.size(); i++) {
      flat(i) = 0.0;
    }
  }
};

REGISTER_KERNEL_BUILDER(
    Name("NpuClearFloatStatus")
.
Device(tensorflow::DEVICE_CPU),
    NpuClearFloatStatusOp
);
}  // namespace tensorflow

