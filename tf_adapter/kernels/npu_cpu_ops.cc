/*************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description:
 *
 * See LICENSE.txt for license information
 ************************************************************************/
#include "tensorflow/core/framework/bounds_check.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/register_types.h"
#include "tensorflow/core/lib/random/random.h"
#include "tensorflow/core/lib/random/philox_random.h"
#include "tensorflow/core/lib/random/simple_philox.h"

namespace tensorflow {
class EmbeddingRankIdOpKernel : public OpKernel {
public:
   explicit EmbeddingRankIdOpKernel(OpKernelConstruction* context) : OpKernel(context) {}
    ~EmbeddingRankIdOpKernel() {}
    void Compute(OpKernelContext* context) override
    {
        LOG(INFO) << "EmbeddingRankIdOp Compute.";
    }
};
REGISTER_KERNEL_BUILDER(Name("EmbeddingRankId").Device(DEVICE_CPU),
                        EmbeddingRankIdOpKernel);
}