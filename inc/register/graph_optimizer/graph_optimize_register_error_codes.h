/**
 * @file graph_optimize_register_error_codes.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * @brief fe_error_codes
 *
 * @author Huawei
 *
 * @version 1.0
 *
 */

#ifndef INC_REGISTER_GRAPH_OPTIMIZE_REGISTER_ERROR_CODES_H_
#define INC_REGISTER_GRAPH_OPTIMIZE_REGISTER_ERROR_CODES_H_

#include <map>
#include <string>

/** Assigned SYS ID */
const uint8_t SYSID_FE = 3;

/** Common module ID */
const uint8_t FE_MODID_COMMON = 50;

namespace fe {

/**  FE error code definiton Macro
*  Build error code
*/
#define FE_DEF_ERRORNO(sysid, modid, name, value, desc)                       \
  static constexpr fe::Status name =                                          \
      (((((uint32_t)(0xFF & ((uint8_t)(sysid)))) << 24) |                     \
       (((uint32_t)(0xFF & ((uint8_t)(modid)))) << 16)) |                     \
       (0xFFFF & ((uint16_t)(value))));

using Status = uint32_t;

#define FE_DEF_ERRORNO_COMMON(name, value, desc) \
  FE_DEF_ERRORNO(SYSID_FE, FE_MODID_COMMON, name, value, desc)

using Status = uint32_t;

FE_DEF_ERRORNO(0, 0, SUCCESS, 0, "success");
FE_DEF_ERRORNO(0xFF, 0xFF, FAILED, 0xFFFF, "failed");
FE_DEF_ERRORNO_COMMON(NOT_CHANGED, 201, "The nodes of the graph not changed.");
FE_DEF_ERRORNO_COMMON(PARAM_INVALID, 1, "Parameter's invalid!");

}  // namespace fe
#endif  // INC_REGISTER_GRAPH_OPTIMIZE_REGISTER_ERROR_CODES_H_
