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

#include "tensorflow/core/framework/bounds_check.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/register_types.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/platform/logging.h"
#include "tf_adapter/common/common.h"

namespace tensorflow {
template<typename T>
class GeluOp : public tensorflow::OpKernel {
 public:
  explicit GeluOp(tensorflow::OpKernelConstruction *context) : OpKernel(context) {}
  ~GeluOp() override = default;
  void Compute(tensorflow::OpKernelContext *context) override {
    // Grab the input tensor
    CHECK_NOT_NULL(context);
    const Tensor &input_tensor = context->input(0);
    auto input = input_tensor.flat<T>();

    // Create an output tensor
    Tensor *output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(), &output_tensor));
    // handle any data type for input and output
    auto output_flat = output_tensor->flat<T>();

    // Set the value of each element
    const int N = input.size();
    auto sqrt2overPI = T(sqrt(2 / M_PI));
    // investigate if we can optimize using other TF ops or using MKLDNN or Eigen
    for (int i = 0; i < N; i++) {
      auto x = input(i);

      auto cdf = T(0.5) * (T(1.0) + tanh(sqrt2overPI * (x + T(0.044715) * pow(x, T(3)))));
      auto y = x * cdf;

      output_flat(i) = y;
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("Gelu").Device(tensorflow::DEVICE_CPU).TypeConstraint<float>("T"), GeluOp<float>);

REGISTER_KERNEL_BUILDER(Name("Gelu").Device(tensorflow::DEVICE_CPU).TypeConstraint<double>("T"), GeluOp<double>);

REGISTER_KERNEL_BUILDER(Name("Gelu").Device(tensorflow::DEVICE_CPU).TypeConstraint<Eigen::half>("T"),
                        GeluOp<Eigen::half>);

template<typename T>
class GeluGradOp : public tensorflow::OpKernel {
 public:
  explicit GeluGradOp(tensorflow::OpKernelConstruction *context) : OpKernel(context) {}
  ~GeluGradOp() override = default;
  void Compute(tensorflow::OpKernelContext *context) override {
    // Grab the grad input tensor
    CHECK_NOT_NULL(context);
    const Tensor &grad_input_tensor = context->input(0);
    auto grad_input = grad_input_tensor.flat<T>();

    // Grab the input tensor
    const Tensor &input_tensor = context->input(1);
    auto input = input_tensor.flat<T>();

    // Grab the gelu output tensor
    const Tensor &output_tensor = context->input(2);
    auto output = output_tensor.flat<T>();

    OP_REQUIRES(context, grad_input.size() == input.size(),
                errors::InvalidArgument("grad_input size is not equal input size"));
    OP_REQUIRES(context, grad_input.size() == output.size(),
                errors::InvalidArgument("grad_input size is not equal output size"));

    // Create an output tensor
    Tensor *grad_output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0, grad_input_tensor.shape(), &grad_output_tensor));
    // handle any data type for input and output
    auto grad_output_flat = grad_output_tensor->flat<T>();

    // Set the value of each element
    const int N = input.size();
    auto sqrt2overPI = T(sqrt(2 / M_PI));
    // investigate if we can optimize using other TF ops or using MKLDNN or Eigen
    for (int i = 0; i < N; i++) {
      auto dLdy = grad_input(i);
      auto x = input(i);
      auto gelu_x = (x != T(0)) ? (output(i) / x) : T(0.5);

      auto tanhterm = gelu_x * T(2) - T(1);
      auto dydx =
          gelu_x + T(0.5) * x * (T(1) - pow(tanhterm, T(2))) * sqrt2overPI * (T(1) + T(3) * T(0.04715) * pow(x, T(2)));
      auto dLdx = dLdy * dydx;

      grad_output_flat(i) = dLdx;
    }
  }
};

REGISTER_KERNEL_BUILDER(Name("GeluGrad").Device(tensorflow::DEVICE_CPU).TypeConstraint<float>("T"), GeluGradOp<float>);

REGISTER_KERNEL_BUILDER(Name("GeluGrad").Device(tensorflow::DEVICE_CPU).TypeConstraint<double>("T"),
                        GeluGradOp<double>);

REGISTER_KERNEL_BUILDER(Name("GeluGrad").Device(tensorflow::DEVICE_CPU).TypeConstraint<Eigen::half>("T"),
                        GeluGradOp<Eigen::half>);
}  // namespace tensorflow
