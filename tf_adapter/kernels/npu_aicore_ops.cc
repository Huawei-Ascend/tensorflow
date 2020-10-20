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
#include "tf_adapter/common/common.h"

namespace tensorflow {
template<typename T>
class FastGeluOp : public tensorflow::OpKernel {
 public:
  explicit FastGeluOp(tensorflow::OpKernelConstruction *context)
      : OpKernel(context) {}
  ~FastGeluOp() {}
  void Compute(tensorflow::OpKernelContext *context) override {
    // Grab the input tensor
    CHECK_NOT_NULL(context);
    const Tensor &input_tensor = context->input(0);

    // Create an output tensor
    Tensor *output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
  }
};

REGISTER_KERNEL_BUILDER(
    Name("FastGelu")
.
Device(tensorflow::DEVICE_CPU)
.TypeConstraint<float>("T"),
FastGeluOp<float>);

REGISTER_KERNEL_BUILDER(
    Name("FastGelu")
.
Device(tensorflow::DEVICE_CPU)
.TypeConstraint<double>("T"),
FastGeluOp<double>);

REGISTER_KERNEL_BUILDER(
    Name("FastGelu")
.
Device(tensorflow::DEVICE_CPU)
.TypeConstraint<Eigen::half>("T"),
FastGeluOp<Eigen::half>);

template<typename T>
class FastGeluGradOp : public tensorflow::OpKernel {
 public:
  explicit FastGeluGradOp(tensorflow::OpKernelConstruction *context)
      : OpKernel(context) {}
  ~FastGeluGradOp() {}
  void Compute(tensorflow::OpKernelContext *context) override {
    // Grab the grad input tensor
    CHECK_NOT_NULL(context);
    const Tensor &grad_input_tensor = context->input(0);
    auto grad_input = grad_input_tensor.flat<T>();

    // Grab the input tensor
    const Tensor &input_tensor = context->input(1);
    auto input = input_tensor.flat<T>();

    OP_REQUIRES(
        context, grad_input.size() == input.size(),
        errors::InvalidArgument("grad_input size is not equal input size"));

    // Create an output tensor
    Tensor *grad_output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, grad_input_tensor.shape(),
                                                     &grad_output_tensor));
  }
};

REGISTER_KERNEL_BUILDER(
    Name("FastGeluGrad")
.
Device(tensorflow::DEVICE_CPU)
.TypeConstraint<float>("T"),
FastGeluGradOp<float>);

REGISTER_KERNEL_BUILDER(
    Name("FastGeluGrad")
.
Device(tensorflow::DEVICE_CPU)
.TypeConstraint<double>("T"),
FastGeluGradOp<double>);

REGISTER_KERNEL_BUILDER(
    Name("FastGeluGrad")
.
Device(tensorflow::DEVICE_CPU)
.TypeConstraint<Eigen::half>("T"),
FastGeluGradOp<Eigen::half>);
}  // namespace tensorflow


