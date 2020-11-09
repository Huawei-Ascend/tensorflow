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
#ifndef TENSORFLOW_CORE_FRAMEWORK_CACHE_INTERFACE_H_
#define TENSORFLOW_CORE_FRAMEWORK_CACHE_INTERFACE_H_

#include <string>
#include <vector>
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/resource_mgr.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/types.h"

namespace tensorflow {
//All implementations must be thread-safe.
class CacheInterface : public ResourceBase {
 public:
  virtual void add(std::vector<uint64> &ids_vec,
                   std::vector<uint64> &swap_in_id_temp,
                   std::vector<uint64> &swap_in_idx_temp,
                   std::vector<uint64> &swap_out_id_temp,
                   std::vector<uint64> &swap_out_idx_temp,
                   int64 &swap_in_num,
                   int64 &swap_out_num) = 0;

  virtual void remoteIndexToLocal(const std::vector<uint64> &ids_vec, Tensor &local_idx) = 0;
  //Return the num of elements in cache
  virtual int64 size() const = 0;
  //Return a debug string for *this
  string DebugString() const override { return strings::StrCat("A Cache of size: ", size()) }
 protected:
  virtual ~CacheInterface() {}
};
} //namespace tensorflow
#endif //TENSORFLOW_CACHE_INTERFACE_H_