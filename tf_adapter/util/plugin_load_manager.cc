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

#include "tf_adapter/util/plugin_load_manager.h"
#include "tensorflow/core/platform/env.h"
#include <dlfcn.h>
#include <climits>

namespace tensorflow {
void* PluginLoadManager::DlOpen(const std::string &path) {
  void* handle = dlopen(path.c_str(), RTLD_NOW);
  if (handle == nullptr) {
    LOG(WARNING) << "dlopen faild, reason:" << dlerror();
  }
  return handle;
}

void* PluginLoadManager::DlSym(void* handle, const std::string &func_name) {
  if (handle == nullptr) {
    LOG(WARNING) << "handle is null, not valid!";
    return nullptr;
  }
  void* func = dlsym(handle, func_name.c_str());
  if (func == nullptr) {
    LOG(WARNING) << "get func[" << func_name << "] faild, reason:" << dlerror();
  }
  return func;
}

std::string PluginLoadManager::GetTFPluginRealPath() {
  Dl_info dl_info;
  if (dladdr(reinterpret_cast<void *>(&PluginLoadManager::GetTFPluginRealPath), &dl_info) == 0) {
    LOG(WARNING) << "can not get tf-adapter base path!";
    return string();
  } else {
    std::string so_path = dl_info.dli_fname;
    char path[PATH_MAX] = {0};
    if (so_path.length() >= PATH_MAX) {
      LOG(WARNING) << "The shared library file path is too long!";
      return string();
    }
    if (realpath(so_path.c_str(), path) == nullptr) {
      LOG(WARNING) << "Failed to get realpath of " << so_path;
      return string();
    }
    so_path = path;
    so_path = so_path.substr(0, so_path.rfind('/') + 1);
    LOG(INFO) << "tf-plugin base path is: " << so_path;
    return so_path;
  }
}
} // namesapce tensorflow
