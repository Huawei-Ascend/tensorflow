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
 * @file hcom_ops_stores.h
 * @brief HCOM operators plugin API
 */

#ifndef HCOM_OPS_STORES_H_
#define HCOM_OPS_STORES_H_

#include "common/opskernel/ops_kernel_info_store.h"
#include "common/optimizer/graph_optimizer.h"
#include "framework/common/ge_inner_error_codes.h"

using OpsKernelInfoStorePtr = std::shared_ptr<ge::OpsKernelInfoStore>;
using GraphOptimizerPtr = std::shared_ptr<ge::GraphOptimizer>;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Initialize HCOM operators plugin.
 *
 * @param options Input parameter. Options must contain rank table path, deploy mode, rank id, pod name.
 * @return ge::SUCCESS success; others：fail.
 */
ge::Status Initialize(const std::map<string, string> &options);

/**
 * @brief Finalize HCOM operators plugin.
 *
 * @return ge::SUCCESS success; others: fail.
 */
ge::Status Finalize();

/**
 * @brief Get the information store of HCOM operators.
 *
 * @param opKernInfos A map identifying the information store of HCOM operators.
 */
void GetOpsKernelInfoStores(std::map<string, OpsKernelInfoStorePtr> &opKernInfos);

/**
 * @brief Get the graph optimizer of HCOM operators.
 *
 * @param graphOptimizers A map identifying the graph optimizer of HCOM operators.
 */
void GetGraphOptimizerObjs(std::map<string, GraphOptimizerPtr> &graphOptimizers);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // HCOM_OPS_STORES_H_
