/*************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description:
 *
 * See LICENSE.txt for license information
 ************************************************************************/

#include "tensorflow/core/framework/op_kernel.h"

namespace tensorflow {
class HcomAllReduceOpKernel : public OpKernel {
public:
    explicit HcomAllReduceOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomAllReduceOpKernel() {}
    void Compute(OpKernelContext* context) override 
    {
        LOG(INFO) << "HcomAllReduceOp Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomAllReduce").Device(DEVICE_CPU),
                        HcomAllReduceOpKernel);

class HcomAllGatherOpKernel : public OpKernel {
public:
    explicit HcomAllGatherOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomAllGatherOpKernel() {}
    void Compute(OpKernelContext* context) override 
    {
        LOG(INFO) << "HcomAllGatherOp Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomAllGather").Device(DEVICE_CPU),
                        HcomAllGatherOpKernel);

class HcomBroadcastOpKernel : public OpKernel {
public:
    explicit HcomBroadcastOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomBroadcastOpKernel() {}
    void Compute(OpKernelContext* context) override 
    {
        LOG(INFO) << "HcomBroadcastOp Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomBroadcast").Device(DEVICE_CPU),
                        HcomBroadcastOpKernel);

class HcomReduceScatterOpKernel : public OpKernel {
public:
    explicit HcomReduceScatterOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomReduceScatterOpKernel() {}
    void Compute(OpKernelContext* context) override 
    {
        LOG(INFO) << "HcomReduceScatterOp Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomReduceScatter").Device(DEVICE_CPU),
                        HcomReduceScatterOpKernel);

class HcomSendOpKernel : public OpKernel {
public:
    explicit HcomSendOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomSendOpKernel() {}
    void Compute(OpKernelContext* context) override 
    {
        LOG(INFO) << "HcomSendOpKernel Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomSend").Device(DEVICE_CPU),
                        HcomSendOpKernel);

class HcomReceiveOpKernel : public OpKernel {
public:
    explicit HcomReceiveOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~HcomReceiveOpKernel() {}
    void Compute(OpKernelContext* context) override 
    {
        LOG(INFO) << "HcomReceiveOpKernel Compute.";
    }
};

REGISTER_KERNEL_BUILDER(Name("HcomReceive").Device(DEVICE_CPU),
                        HcomReceiveOpKernel);
} // namespace tensorflow
