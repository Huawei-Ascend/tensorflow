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

#include "tensorflow/core/framework/op_kernel.h"

namespace tensorflow {
template <typename T>
class DeformableConv2DOp : public OpKernel {
 public:
  explicit DeformableConv2DOp(OpKernelConstruction *context)
    : OpKernel(context) {
    LOG(INFO) << "new DeformableConv2DOp";
  }
  ~DeformableConv2DOp() override { LOG(INFO) << "del DeformableConv2DOp"; }
  void Compute(OpKernelContext *context) override {
    LOG(INFO) << "DeformableConv2DOp Compute, num_inputs: "
              << context->num_inputs();
  }
  bool IsExpensive() override { return false; }
};

#define REGISTER_KERNEL(type)                                \
REGISTER_KERNEL_BUILDER(Name("DeformableConv2D")             \
                            .Device(DEVICE_CPU)              \
                            .TypeConstraint<type>("T"),      \
                        DeformableConv2DOp<type>)
REGISTER_KERNEL(float);
REGISTER_KERNEL(Eigen::half);
#undef REGISTER_KERNEL

template <typename T>
class DeformableConv2DWithBiasOp : public OpKernel {
 public:
  explicit DeformableConv2DWithBiasOp(OpKernelConstruction *context)
    : OpKernel(context) {
    LOG(INFO) << "new DeformableConv2DWithBiasOp";
  }
  ~DeformableConv2DWithBiasOp() override {
    LOG(INFO) << "del DeformableConv2DWithBiasOp";
  }
  void Compute(OpKernelContext *context) override {
    LOG(INFO) << "DeformableConv2DWithBiasOp Compute, num_inputs: "
              << context->num_inputs();
  }
  bool IsExpensive() override { return false; }
};

#define REGISTER_KERNEL(type)                                \
REGISTER_KERNEL_BUILDER(Name("DeformableConv2DWithBias")     \
                            .Device(DEVICE_CPU)              \
                            .TypeConstraint<type>("T"),      \
                        DeformableConv2DWithBiasOp<type>)
REGISTER_KERNEL(float);
REGISTER_KERNEL(Eigen::half);
#undef REGISTER_KERNEL
}  // namespace tensorflow
