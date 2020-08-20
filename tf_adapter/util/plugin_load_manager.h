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

#ifndef TENSORFLOW_PLUGIN_LOAD_MANAGER_H_
#define TENSORFLOW_PLUGIN_LOAD_MANAGER_H_

#include "tensorflow/core/lib/core/status.h"
#include<string>

namespace tensorflow {
class PluginLoadManager {
public:
  static void* DlOpen(const std::string &path);
  static void* DlSym(void* handel, const std::string &fun_name);
  static std::string GetTFPluginRealPath();
};
} // namespace tensorflow
#endif // TENSORFLOW_PLUGIN_LOAD_MANAGER_H_
