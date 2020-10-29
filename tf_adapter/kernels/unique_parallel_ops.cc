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

#include <vector>
#include <atomic>
#include <unordered_map>
#include "tf_adapter/common/common.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/register_types.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/framework/bounds_check.h"
#include "tensorflow/core/lib/core/status.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"

#define EIGEN_USE_THREADS
#define EIGEN_CXX11_TENSOR_TENSOR_DEVICE_THREAD_POOL_H

namespace tensorflow {
template <typename T, typename TIndex>
class UniqueParallelOp : public OpKernel {
 public:
  explicit UniqueParallelOp(OpKernelConstruction *context) 
    : OpKernel(context) {}
  ~UniqueParallelOp() {}
  void Compute(OpKernelContext *context) override {
    const Tensor &input_tensor = context->input(0);
    OP_REQUIRES(context,
      input_tensor.NumElements() <= std::numeric_limits<int32>::max(),
        errors::InvalidArgument("unique does not support input tensors larger than ",
          std::numeric_limits<int32>::max(), " elements"));
    OP_REQUIRES(context, TensorShapeUtils::IsVector(input_tensor.shape()),
      errors::InvalidArgument("unique expects a 1D vector."));
    OP_REQUIRES(context, (input_tensor.dtype() == DT_INT32 || input_tensor.dtype() == DT_INT64),
      errors::InvalidArgument("input tensor should be int32 or int64, but got ",
        DataTypeString(input_tensor.dtype())));
    auto input_vec = input_tensor.vec<T>();
    int64 total = static_cast<int64>(input_vec.size());
    Tensor* index_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(1,
      TensorShape({input_vec.size()}), &index_tensor));
    auto index_vec = index_tensor->vec<TIndex>();

    const int64 CPU_NUMS = 16;
    std::atomic<TIndex> count_num(0);
    Tensor output_temp(input_tensor.dtype(), TensorShape({total}));
    auto output_temp_flat = output_temp.flat<T>();
    tensorflow::thread::ThreadPool thread_work(context->env(), "unique_parallel", CPU_NUMS);
    std::function<void(int64, int)> shards = [&](int64 total, int cur) {
      std::unordered_map<T, TIndex> unique_map;
      for (TIndex i = 0, j = 0; i < total; i++) {
        unsigned long long temp_input = input_vec(i);
        unsigned long long temp_cpu = CPU_NUMS - 1;
        if ((temp_input & temp_cpu) == cur) {
          if (unique_map.find(input_vec(i)) == unique_map.end()) {
            j = count_num++;
            unique_map[input_vec(i)] = j;
            output_temp_flat(j) = input_vec(i);
          }
          index_vec(i) = unique_map[input_vec(i)];
        }
      }
    };
    ParallelFor(thread_work, total, CPU_NUMS, shards);
    Tensor *output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0,
      TensorShape({count_num}), &output_tensor));
    *output_tensor = output_temp.Slice(0, count_num);
  }
 private:
  void ParallelFor(tensorflow::thread::ThreadPool& thread_work,
    int64 total, const int cpu_nums, std::function<void(int64, int)>& fn) {
    CHECK_GE(total, 0);
    CHECK_EQ(total, (int64)(Eigen::Index)total);
    if (total <= 1 || cpu_nums == 1) {
      fn(total, 0);
      return;
    }
    Eigen::Barrier barrier(static_cast<unsigned int>(cpu_nums));
    std::function<void(int64, int)> handle_range;
    handle_range = [=, &barrier, &fn](int64 total, int cur) {
      fn(total, cur);
      barrier.Notify();
    };
    for (int i = 0; i < cpu_nums; i++) {
      thread_work.Schedule([=, &handle_range]() { handle_range(total, i); });
    }
    barrier.Wait();
  }
};

/*lint -e665*/
#define REGISTER_UNIQUE_PARALLEL(type)                           \
  REGISTER_KERNEL_BUILDER(Name("Unique")                         \
                              .Device(DEVICE_CPU)                \
                              .TypeConstraint<type>("T")         \
                              .TypeConstraint<int32>("out_idx")  \
                              .Label("parallel"),                \
                              UniqueParallelOp<type, int32>);    \
  REGISTER_KERNEL_BUILDER(Name("Unique")                         \
                              .Device(DEVICE_CPU)                \
                              .TypeConstraint<type>("T")         \
                              .TypeConstraint<int64>("out_idx")  \
                              .Label("parallel"),                \
                              UniqueParallelOp<type, int64>);
TF_CALL_INTEGRAL_TYPES(REGISTER_UNIQUE_PARALLEL);
#undef REGISTER_UNIQUE_PARALLEL
/*lint +e665*/
}