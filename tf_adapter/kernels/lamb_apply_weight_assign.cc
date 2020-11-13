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
class LambApplyWeightAssignOp : public OpKernel {
 public:
  explicit LambApplyWeightAssignOp(OpKernelConstruction *context) : OpKernel(context) {}
  ~LambApplyWeightAssignOp() override = default;
  void Compute(OpKernelContext *context) override {
    LOG(INFO) << "LambApplyWeightAssignOp Compute, num_inputs: " << context->num_inputs();
  }
  bool IsExpensive() override { return false; }
};

REGISTER_KERNEL_BUILDER(Name("LambApplyWeightAssign").Device(DEVICE_CPU), LambApplyWeightAssignOp);
}  // namespace tensorflow
