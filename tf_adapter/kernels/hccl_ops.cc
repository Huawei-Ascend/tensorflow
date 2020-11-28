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
class HcomAllReduceOpKernel : public OpKernel {
 public:
  explicit HcomAllReduceOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomAllReduceOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomAllReduceOp Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomAllReduce").Device(DEVICE_CPU), HcomAllReduceOpKernel);

class HcomAllGatherOpKernel : public OpKernel {
 public:
  explicit HcomAllGatherOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomAllGatherOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomAllGatherOp Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomAllGather").Device(DEVICE_CPU), HcomAllGatherOpKernel);

class HcomBroadcastOpKernel : public OpKernel {
 public:
  explicit HcomBroadcastOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomBroadcastOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomBroadcastOp Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomBroadcast").Device(DEVICE_CPU), HcomBroadcastOpKernel);

class HcomReduceOpKernel : public OpKernel {
 public:
  explicit HcomReduceOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomReduceOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomReduceOp Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomReduce").Device(DEVICE_CPU), HcomReduceOpKernel);

class HcomReduceScatterOpKernel : public OpKernel {
 public:
  explicit HcomReduceScatterOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomReduceScatterOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomReduceScatterOp Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomReduceScatter").Device(DEVICE_CPU), HcomReduceScatterOpKernel);

class HcomSendOpKernel : public OpKernel {
 public:
  explicit HcomSendOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomSendOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomSendOpKernel Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomSend").Device(DEVICE_CPU), HcomSendOpKernel);

class HcomReceiveOpKernel : public OpKernel {
 public:
  explicit HcomReceiveOpKernel(OpKernelConstruction *context) : OpKernel(context) {}
  ~HcomReceiveOpKernel() {}
  void Compute(OpKernelContext *context) override { LOG(INFO) << "HcomReceiveOpKernel Compute."; }
};

REGISTER_KERNEL_BUILDER(Name("HcomReceive").Device(DEVICE_CPU), HcomReceiveOpKernel);

class HcomRemoteReadOpKernel : public OpKernel {
public:
    explicit HcomRemoteReadOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomRemoteReadOpKernel() {}
    void Compute(OpKernelContext* context) override
    {
        LOG(INFO) << "HcomRemoteReadOpKernel Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomRemoteRead").Device(DEVICE_CPU), HcomRemoteReadOpKernel);

class HcomRemoteRefReadOpKernel : public OpKernel {
public:
    explicit HcomRemoteRefReadOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomRemoteRefReadOpKernel() {}
    void Compute(OpKernelContext* context) override
    {
        LOG(INFO) << "HcomRemoteRefRead Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomRemoteRefRead").Device(DEVICE_CPU), HcomRemoteRefReadOpKernel);

class HcomRemoteWriteKernel : public OpKernel {
public:
    explicit HcomRemoteWriteKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomRemoteWriteKernel() {}
    void Compute(OpKernelContext* context) override
    {
        LOG(INFO) << "HcomRemoteWriteKernel Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomRemoteWrite").Device(DEVICE_CPU), HcomRemoteWriteKernel);

class HcomRemoteScatterWriteOpKernel : public OpKernel {
public:
    explicit HcomRemoteScatterWriteOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomRemoteScatterWriteOpKernel() {}
    void Compute(OpKernelContext* context) override
    {
        LOG(INFO) << "HcomRemoteScatterWrite Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomRemoteScatterWrite").Device(DEVICE_CPU), HcomRemoteScatterWriteOpKernel);

} // namespace tensorflow