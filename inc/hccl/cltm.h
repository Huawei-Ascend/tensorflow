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
 * @file cltm.h
 * @brief Cluster Logical Topology Management API
 * 
 */

#ifndef CLTM_INC_H_
#define CLTM_INC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief CLTM functions return value definition
 */
typedef enum tagCltmResult {
    CLTM_SUCCESS = 0,    /**< success */
    CTLM_E_PTR,          /**< empty pointer */
    CLTM_E_PARA,         /**< parameter error */
    CLTM_E_NO_RESOURCE,  /**< resource not enough error */
    CLTM_E_RESERVED      /**< reserved */
} cltmResult_t;

/**
 * @brief Generate rank table
 *
 * @param allocatedResource A string identifying the resource list allocate by the CSM.
 * @param rankTableBuf A string identifying the buffer of .
 * @param maxBufSize An integer(u32)  identifying the size of rank table buffer.
 * @param usedBufSize A pointer identifying the used size of rank table buffer.
 * @return cltmResult_t
 */
cltmResult_t cltm_generate_ranktable(const char *allocatedResource, char* rankTableBuf,
    unsigned int maxBufSize, unsigned int *usedBufSize);

#ifdef __cplusplus
}
#endif
#endif // CLTM_INC_H_
