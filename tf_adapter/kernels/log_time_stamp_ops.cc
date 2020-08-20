/**
* Copyright (C) <2019>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
* Description : implememt of log time stamp
*/

#ifndef TENSORFLOW_CORE_KERNELS_LogTimeStamp_OP_H_
#define TENSORFLOW_CORE_KERNELS_LogTimeStamp_OP_H_
#if HISI_OFFLINE

#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/tensor_types.h"
#include "tensorflow/core/platform/macros.h"

namespace tensorflow 
{
  class LogTimeStampOP : public OpKernel {
  public:
    explicit LogTimeStampOP(OpKernelConstruction* ctx) : OpKernel(ctx) {
        LOG(INFO) << "new LogTimeStampOP";
    }
    ~LogTimeStampOP() {
      LOG(INFO) << "del LogTimeStampOP";
    }
    void Compute(OpKernelContext* ctx) override {
      LOG(INFO) << "in LogTimeStampOP";
    }
    bool IsExpensive() override { return false; }
  };

  REGISTER_KERNEL_BUILDER(Name("LogTimeStamp").Device(DEVICE_CPU), 
          LogTimeStampOP);
}  // namespace tensorflow

#endif  // HISI_OFFLINE

#endif  // TENSORFLOW_CORE_KERNELS_LogTimeStamp_OP_H_
