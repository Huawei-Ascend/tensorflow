/**
* Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.
* Description: Max pooling grad with argmax
*/

#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/tensor_types.h"
#include "tensorflow/core/platform/macros.h"
#include "tensorflow/core/framework/types.h"

namespace tensorflow {
namespace {

class MaxPoolingGradGradWithArgmaxOp : public OpKernel {
 public:
  explicit MaxPoolingGradGradWithArgmaxOp(OpKernelConstruction *ctx) : OpKernel(ctx) {
    LOG(INFO) << "MaxPoolingGradGradWithArgmaxOp built";
  }
  ~MaxPoolingGradGradWithArgmaxOp() {
    LOG(INFO) << "MaxPoolingGradGradWithArgmaxOp has been destructed";
  }
  void Compute(OpKernelContext *ctx) override {
    LOG(INFO) << "[ATTENTION] MaxPoolingGradGradWithArgmaxOp can not run on cpu, \
                  only running on npu, please open use_off_line ";
  }
  bool IsExpensive() override { return false; }
};

REGISTER_KERNEL_BUILDER(Name("MaxPoolGradGradWithArgmax")
.
Device(DEVICE_CPU),
    MaxPoolingGradGradWithArgmaxOp
);

}

}
