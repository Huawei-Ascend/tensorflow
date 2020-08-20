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
class DropOutDoMaskOp : public OpKernel {
 public:
  explicit DropOutDoMaskOp(OpKernelConstruction *context) : OpKernel(context) {}
  ~DropOutDoMaskOp() {}
  void Compute(OpKernelContext *context) override {
    LOG(INFO) << "DropOutDoMaskOp Compute ";
  }
  bool IsExpensive() override { return false; }
};

class DropOutGenMaskOp : public OpKernel {
 public:
  explicit DropOutGenMaskOp(OpKernelConstruction *context) : OpKernel(context) {}
  ~DropOutGenMaskOp() {}
  void Compute(OpKernelContext *context) override {
    LOG(INFO) << "DropOutGenMaskOp Compute ";
  }
  bool IsExpensive() override { return false; }
};

REGISTER_KERNEL_BUILDER(Name("DropOutGenMask")
.
Device(DEVICE_CPU), DropOutGenMaskOp
);
REGISTER_KERNEL_BUILDER(Name("DropOutDoMask")
.
Device(DEVICE_CPU), DropOutDoMaskOp
);
}