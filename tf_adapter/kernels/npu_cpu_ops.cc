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
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/resource_op_kernel.h"
#include "tf_adapter/util/cache_interface.h"

namespace tensorflow {
class EmbeddingRankIdOpKernel : public OpKernel {
 public:
  explicit EmbeddingRankIdOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~EmbeddingRankIdOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "EmbeddingRankIdOp Compute."; }
};

class LruCacheOp : public ResourceOpKernel<CacheInterface> {
 public:
  explicit LruCacheOp(OpKernelConstruction* context) : ResourceOpKernel(context) {}
  void Compute(OpKernelContext* context) override { LOG(INFO) << "LruCacheOp Compute"; }
 private:
  Status CreateResource(CacheInterface** context) override
                        EXCLUSIVE_LOCKS_REQUIRED(mu_) {
    return Status::OK();
  }
};

class CacheAddOp : public OpKernel {
 public:
  explicit CacheAddOp(OpKernelConstruction *context) : OpKernel(context) {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "CacheAddOp Compute"; }
};

class CacheRemoteIndexToLocalOp : public OpKernel {
 public:
  explicit CacheRemoteIndexToLocalOp(OpKernelConstruction *context) : OpKernel(context) {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "CacheRemoteIndexToLocalOp Compute"; }
};

REGISTER_KERNEL_BUILDER(Name("EmbeddingRankId").Device(DEVICE_CPU), EmbeddingRankIdOpKernel);
REGISTER_KERNEL_BUILDER(Name("LruCache").Device(DEVICE_CPU), LruCacheOp);
REGISTER_KERNEL_BUILDER(Name("CacheAdd").Device(DEVICE_CPU), CacheAddOp);
REGISTER_KERNEL_BUILDER(Name("CacheRemoteIndexToLocal").Device(DEVICE_CPU), CacheRemoteIndexToLocalOp);
}  // namespace tensorflow