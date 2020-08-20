/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file hvd_adapter_pub.h
 * @brief Horovod Adapter API
 */

#ifndef HVD_ADAPTER_
#define HVD_ADAPTER_

#include <hccl/base.h>

namespace hccl {
using HvdCbDataProcessFunc = hcclResult_t (*)(void *fnData);

/**
 * @brief Add function to process fndata.
 * @param fn A hvdCbDataProcessFunc type function.
 * @return void
 */
extern void HvdCbDataProcessFuncAdd(HvdCbDataProcessFunc fn);  // 注册处理回host侧数据的函数
}
#endif // HVD_ADAPTER_
