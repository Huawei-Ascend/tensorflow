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

#ifndef MSPROF_ENGINE_PROF_ACL_API_H_
#define MSPROF_ENGINE_PROF_ACL_API_H_

#define MSVP_PROF_API __attribute__((visibility("default")))

#include <cstdint>
#include <string>

/**
 * @name  ProrErrorCode
 * @brief error code of prof_acl_apis
 */
enum ProfErrorCode {
    PROF_ERROR_NONE = 0,            // ok
    PROF_ERROR_PARAM_INVALID,       // param invalid, for example nullptr
    PROF_ERROR_REPEAT_INIT,         // profiling has already been inited
    PROF_ERROR_CONFIG_INVALID,      // config invalid, for example invalid json string
    PROF_ERROR_DIR_NO_ACCESS,       // dir is not accessable
    PROF_ERROR_FAILURE,             // failed to init or start profiling
    PROF_ERROR_NOT_INITED,          // profiling has not been inited
    PROF_ERROR_DEVICE_INVALID,      // device id invalid
    PROF_ERROR_UNSUPPORTED,         // unsupported data type or ai core metrics
    PROF_ERROR_REPEAT_START,        // profiilng has already been started
    PROF_ERROR_NOT_STARTED,         // profiling has not been started
};

/**
 * @brief transfer profiling config in acl.json to sample config
 * @param aclCfg       [IN]  profiling json string from acl.json as {"switch":"on", "result_path":"/home",...}
 * @param sampleCfg    [OUT] json string for GE as {"startCfg":[{"deviceID":"all","jobID":"1234",...}]}
 * @return ProfErrorCode
 */
MSVP_PROF_API int32_t ProfAclCfgToSampleCfg(const std::string &aclCfg, std::string &sampleCfg);

#endif  // MSPROF_ENGINE_PROF_ACL_API_H_
