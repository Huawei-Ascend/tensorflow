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
#include "tensorflow/core/framework/tensor_types.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/platform/macros.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"

namespace tensorflow {
namespace {

class MaxPoolingGradGradWithArgmaxOp : public OpKernel {
 public:
  explicit MaxPoolingGradGradWithArgmaxOp(OpKernelConstruction *ctx) : OpKernel(ctx) {
    LOG(INFO) << "MaxPoolingGradGradWithArgmaxOp built";
  }
  ~MaxPoolingGradGradWithArgmaxOp() { LOG(INFO) << "MaxPoolingGradGradWithArgmaxOp has been destructed"; }
  void Compute(OpKernelContext *ctx) override {
    LOG(INFO) << "[ATTENTION] MaxPoolingGradGradWithArgmaxOp can not run on cpu, \
                  only running on npu, please open use_off_line ";
  }
  bool IsExpensive() override { return false; }
};

REGISTER_KERNEL_BUILDER(Name("MaxPoolGradGradWithArgmax").Device(DEVICE_CPU), MaxPoolingGradGradWithArgmaxOp);

}  // namespace

}  // namespace tensorflow
