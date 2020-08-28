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

    Tensor* index_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(1, 
      TensorShape({input_vec.size()}), &index_tensor));
    auto index_vec = index_tensor->template vec<TIndex>();

    const int cpu_nums = 16;
    std::atomic<TIndex> count_num(0);
    std::vector<std::unordered_map<T, TIndex> > unique_map_vec;
    unique_map_vec.resize(cpu_nums);
    std::function<void(int64, int)> shards = [&](int64 total, int cur) {
      for (TIndex i = 0; i < total; i++){
        if ((input_vec(i) & (cpu_nums-1)) == cur) {
          if (unique_map_vec[cur].find(input_vec(i)) == unique_map_vec[cur].end()) {
            unique_map_vec[cur][input_vec(i)] = count_num++;
          }
          index_vec(i) = unique_map_vec[cur][input_vec(i)];
        }
      }
    };
    tensorflow::thread::ThreadPool thread_work(context->env(), "unique_parallel", cpu_nums);
    ParallelFor(thread_work, (int64)(input_vec.size()), cpu_nums, shards);
    
    Tensor* output_tensor = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output(0,
      TensorShape({count_num}), &output_tensor));
    auto output_vec = output_tensor->template vec<T>();
    int index = 0;
    for (size_t i = 0; i < unique_map_vec.size(); i++) {
      for (auto it : unique_map_vec[i]) {
        output_vec(index) = it.first;
        index++;
      }    
    }
  }
 private:
  void ParallelFor(tensorflow::thread::ThreadPool& thread_work, 
    int64 total, int cpu_nums, std::function<void(int64, int)>& fn) {
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

REGISTER_KERNEL_BUILDER(Name("Unique")                     
                       .TypeConstraint<int64>("T")                            
                       .TypeConstraint<int32>("out_idx")                      
                       .Device(DEVICE_CPU)                                   
                       .Label("parallel"),                                     
                       UniqueParallelOp<int64, int32>);                       
REGISTER_KERNEL_BUILDER(Name("Unique")                     
                       .TypeConstraint<int32>("T")                            
                       .TypeConstraint<int32>("out_idx")                      
                       .Device(DEVICE_CPU)                            
                       .Label("parallel"), 
                       UniqueParallelOp<int32, int32>);
REGISTER_KERNEL_BUILDER(Name("Unique")
                       .TypeConstraint<int32>("T")
                       .TypeConstraint<int64>("out_idx")
                       .Device(DEVICE_CPU)
                       .Label("parallel"), 
                       UniqueParallelOp<int32, int64>);
REGISTER_KERNEL_BUILDER(Name("Unique")
                       .TypeConstraint<int64>("T")
                       .TypeConstraint<int64>("out_idx")
                       .Device(DEVICE_CPU)
                       .Label("parallel"), 
                       UniqueParallelOp<int64, int64>);
}