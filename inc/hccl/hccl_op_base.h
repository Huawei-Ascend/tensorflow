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
 * @file hccl_op_base.h
 * @brief HCCL OP_BASE API
 */

#ifndef HCCL_OPBASE_H_
#define HCCL_OPBASE_H_

#include <hccl/base.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Initialize HCCL in OP_BASE mode.
 *
 * @param rank_table A string identifying the rank table file path, include file name.
 * @param identify A string identifying the identify for the rank.
 * @param comm A pointer identifying the initialized communication resource.
 * @return hcclResult_t
 * @see hcclCommDestroy()
 */
extern hcclResult_t hcclCommInitClusterInfo(const char *rankTable, const char *identify, hcclComm_t *comm);

/**
 * @brief Get hccl unique id in OP_BASE mode.
 *
 * @param id A pointer identifying the hccl unique id.
 * @return hcclResult_t
 */
extern hcclResult_t hcclGetUniqueId(hcclUniqueId* id);

/**
 * @brief Initialize HCCL with unique id in OP_BASE mode.
 *
 * @param comm A pointer identifying the initialized communication resource.
 * @param nranks A integer identifying the rank size of the cluster.
 * @param commId A struct identifying the hccl unique id.
 * @param myrank A integer identifying the identify for the rank.
 * @return hcclResult_t
 * @see hcclCommDestroy()
 */
extern hcclResult_t hcclCommInitUniqueId(hcclComm_t* comm, u32 nranks, hcclUniqueId commId, u32 myrank);

/**
 * @brief AllReduce operator in OP_BASE mode.
 *
 * @param inputPtr A pointer identifying the input data address of the operator.
 * @param outputPtr A pointer identifying the output data address of the operator.
 * @param count An integer(u64) identifying the number of the output data.
* @param dataType The data type of the operator, must be one of the following types: int8, int32, float16, float32.
 * @param op The reduction type of the operator, must be one of the following types: sum, min, max, prod.
 * @param comm A pointer identifying the communication resource based on.
 * @param stream A pointer identifying the stream information.
 * @return hcclResult_t 
 */
extern hcclResult_t hcclAllReduce(void *inputPtr, void *outputPtr, u64 count, hcclDataType_t dataType,
    hcclRedOp_t op, hcclComm_t comm, rtStream_t stream);

/**
 * @brief Broadcast operator in OP_BASE mode.
 *
 * @param ptr A pointer identifying the data address of the operator.
 * @param count An integer(u64) identifying the number of the data.
 * @param dataType The data type of the operator, must be one of the following types: int8, int32, float16, float32.
 * @param root An integer(u32) identifying the the root rank in the operator.
 * @param comm A pointer identifying the communication resource based on
 * @param stream A pointer identifying the stream information.
 * @return hcclResult_t 
 */
extern hcclResult_t hcclBroadcast(void *ptr, u64 count, hcclDataType_t dataType, u32 root, hcclComm_t comm, 
    rtStream_t stream);

/**
 * @brief ReduceScatter operator in OP_BASE mode.
 *
 * @param inputPtr A pointer identifying the input data address of the operator.
 * @param outputPtr A pointer identifying the output data address of the operator.
 * @param count An integer(u64) identifying the number of the output data.
* @param dataType The data type of the operator, must be one of the following types: int8, int32, float16, float32.
 * @param op The reduction type of the operator, must be one of the following types: sum, min, max, prod.
 * @param comm A pointer identifying the communication resource based on.
 * @param stream A pointer identifying the stream information.
 * @return hcclResult_t 
 */
extern hcclResult_t hcclReduceScatter(void* inputPtr, void* outputPtr, u64 count, hcclDataType_t dataType,
    hcclRedOp_t op, hcclComm_t comm, rtStream_t stream);

/**
 * @brief AllGather operator in OP_BASE mode.
 *
 * @param inputPtr A pointer identifying the input data address of the operator.
 * @param outputPtr A pointer identifying the output data address of the operator.
 * @param count An integer(u64) identifying the number of the input data.
* @param dataType The data type of the operator, must be one of the following types: int8, int32, float16, float32.
 * @param comm A pointer identifying the communication resource based on.
 * @param stream A pointer identifying the stream information.
 * @return hcclResult_t 
 */
extern hcclResult_t hcclAllGather(void* inputPtr, void* outputPtr, u64 count, hcclDataType_t dataType,
    hcclComm_t comm, rtStream_t stream);

/**
 * @brief Destroy HCCL comm
 *
 * @param comm A pointer identifying the communication resource targetting
 * @return hcclResult_t
 * @see hcclCommInitClusterInfo()
 */
extern hcclResult_t hcclCommDestroy(hcclComm_t comm);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // HCCL_OPBASE_H_
