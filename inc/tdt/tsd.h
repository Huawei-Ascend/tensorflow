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

#ifndef INC_TDT_TSD_H_
#define INC_TDT_TSD_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
* @ingroup  Tsdaemon.
*
* Identifies that HCCP or Compute_process is waiting for
* Tsdaemon to issue a shutdown command.
*/
typedef enum {
  TSD_HCCP = 0,    /**< HCCP*/
  TSD_COMPUTE = 1, /**< Compute_process*/
  TSD_WAITTYPE_MAX /**< Max*/
} TsdWaitType;

/**
* @ingroup TsdWaitForShutdown
* @brief Wait for the TSD process to issue the shutdown command
*
* @par Function
* Wait for the TSD process to issue the shutdown command
*
* @param NA
* @param deviceID [IN] type #unsigned int. Physical device ID
* @param waitType [IN] type #TsdWaitType. HCCP or CP
* @retval 0 Success
* @retval OtherValues 0 Fail
*
* @par Dependency
* @li libtsdppc.so: Library to which the interface belongs.
* @li tsd.h: Header file where the interface declaration is located.
*/
int32_t TsdWaitForShutdown(const uint32_t deviceId, const TsdWaitType waitType);

/**
* @ingroup PpcClientSendHeartbeat
* @brief Ppc client send heartbeat msg to ppc server
*
* @par Function
* Ppc client send heartbeat msg to ppc server
*
* @param NA
* @param deviceID [IN] type #unsigned int. Physical device ID
* @param waitType [IN] type #TsdWaitType. HCCP or CP
* @retval 0 Success
* @retval OtherValues 0 Fail
*
* @par Dependency
* @li libtsdppc.so: Library to which the interface belongs.
* @li tsd.h: Header file where the interface declaration is located.
*/
int32_t TsdHeartbeatSend(const uint32_t deviceId, const TsdWaitType waitType);

/**
* @ingroup PpcClientSendAbnormalMsg
* @brief Ppc client send abnormal msg to ppc server
*
* @par Function
* Ppc client send abnormal msg to ppc server
*
* @param NA
* @param deviceID [IN] type #unsigned int. Physical device ID
* @param waitType [IN] type #TsdWaitType. HCCP or CP
* @retval 0 Success
* @retval OtherValues 0 Fail
*
* @par Dependency
* @li libtsdppc.so: Library to which the interface belongs.
* @li tsd.h: Header file where the interface declaration is located.
*/
int32_t TsdDestory(const uint32_t deviceId, const TsdWaitType waitType);

#ifdef __cplusplus
}
#endif  // __cplusplus
#endif  // INC_TDT_TSD_H_
