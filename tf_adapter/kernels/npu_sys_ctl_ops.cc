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

#ifndef TENSORFLOW_NPU_SYS_CTL_OPS_H_
#define TENSORFLOW_NPU_SYS_CTL_OPS_H_

#include <dlfcn.h>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>

#include "framework/common/fmk_error_codes.h"
#include "framework/common/ge_inner_error_codes.h"
#include "framework/omg/parser/parser_api.h"
#include "ge/ge_api.h"
#include "ge/ge_api_types.h"
#include "hccl/hcom.h"
#include "tdt/tdt_host_interface.h"
#include "tdt/tsd_client.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/platform/mutex.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/kernels/geop_npu.h"
#include "tf_adapter/util/ge_plugin.h"
#include "tf_adapter/util/npu_attrs.h"

using namespace tdt;

namespace tensorflow {
inline string ToString(ge::Status status) { return ::ge::StatusFactory::Instance()->GetErrDesc(status); }

static int64 GetCurrentTimestamp() {
  struct timeval tv;
  int ret = gettimeofday(&tv, nullptr);
  if (ret != 0) {
    LOG(ERROR) << "Func gettimeofday failed, ret:" << ret;
    return 0;
  }
  int64 timestamp = tv.tv_usec + tv.tv_sec * 1000000;
  return timestamp;
}
static mutex g_mu(LINKER_INITIALIZED);
static int g_npuInitNum = 0;

static const int64 kMicrosToMillis = 1000;

class NPUInit : public OpKernel {
 public:
  explicit NPUInit(OpKernelConstruction *ctx);
  void Compute(OpKernelContext *ctx) override;
  ~NPUInit() override;

 private:
  std::map<std::string, std::string> init_options_;
};

NPUInit::NPUInit(OpKernelConstruction *ctx) : OpKernel(ctx) {
  LOG(INFO) << "NPUInit";
  mutex_lock lock{g_mu};
  g_npuInitNum++;
  string sess_config;
  Status s = ctx->GetAttr("_NpuOptimizer", &sess_config);
  if (s.ok()) {
    init_options_ = NpuAttrs::GetInitOptions(ctx);
  } else {
    LOG(INFO) << "[NPUInit] NPUInit can not get _NpuOptimizer attr, use default init options";
    init_options_ = NpuAttrs::GetDefaultInitOptions();
  }
}
void NPUInit::Compute(OpKernelContext *ctx) {
  if (GePlugin::GetInstance()->IsGlobal()) {
    LOG(INFO) << "[NPUInit] GePlugin global, skip GePlugin init";
    return;
  }
  GePlugin::GetInstance()->Init(init_options_);
  LOG(INFO) << "[NPUInit] GePlugin init success";
}

NPUInit::~NPUInit() {
  LOG(INFO) << "[~NPUInit] NPUInit destructed";
  int64 unInitStartTime = GetCurrentTimestamp();
  {
    mutex_lock lock{g_mu};
    if (g_npuInitNum > 0) { g_npuInitNum--; }
    if (g_npuInitNum != 0) {
      int64 unInitEndTime = GetCurrentTimestamp();
      LOG(INFO) << "[~NPUInit] NPU Shutdown success. [" << ((unInitEndTime - unInitStartTime) / kMicrosToMillis)
                << " ms]";
      return;
    }
  }
  if (!GePlugin::GetInstance()->IsGlobal()) {
    GePlugin::GetInstance()->Finalize();
    LOG(INFO) << "[~NPUInit] GePlugin Finalize success";
  } else {
    LOG(INFO) << "[~NPUInit] GePlugin global, skip GePlugin Finalize";
  }

  int64 unInitEndTime = GetCurrentTimestamp();
  LOG(INFO) << "[~NPUInit] NPU Shutdown success. [" << ((unInitEndTime - unInitStartTime) / kMicrosToMillis) << " ms]";
}

class NPUShutdown : public OpKernel {
 public:
  explicit NPUShutdown(OpKernelConstruction *ctx) : OpKernel(ctx){};
  void Compute(OpKernelContext *ctx) override;
  ~NPUShutdown() override = default;
};
void NPUShutdown::Compute(OpKernelContext *ctx) {
  LOG(INFO) << "[NPUShutdown] NPUShutdown Compute";
  {
    mutex_lock lock{g_mu};
    g_npuInitNum = 0;
  }
  if (!GePlugin::GetInstance()->IsGlobal()) {
    GePlugin::GetInstance()->Finalize();
    LOG(INFO) << "[~NPUShutdown] GePlugin Finalize success";
  } else {
    LOG(INFO) << "[~NPUShutdown] GePlugin global, skip GePlugin Finalize";
  }
}

REGISTER_KERNEL_BUILDER(Name("NPUInit").Device(DEVICE_CPU), NPUInit);
REGISTER_KERNEL_BUILDER(Name("NPUShutdown").Device(DEVICE_CPU), NPUShutdown);
}  // namespace tensorflow
#endif  // TENSORFLOW_NPU_SYS_CTL_OPS_H_
