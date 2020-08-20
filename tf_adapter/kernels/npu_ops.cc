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

#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/tensor_types.h"
#include "tensorflow/core/platform/macros.h"

namespace tensorflow {
namespace {
class NPUTestOP : public OpKernel {
 public:
  explicit NPUTestOP(OpKernelConstruction *ctx) : OpKernel(ctx) {
  }
  ~NPUTestOP() {
  }
  void Compute(OpKernelContext *ctx) override {
  }
  bool IsExpensive() override { return false; }
};

REGISTER_KERNEL_BUILDER(Name("NPUTest")
.
Device(DEVICE_CPU),
    NPUTestOP
);
}  // namespace
}  // namespace tensorflow