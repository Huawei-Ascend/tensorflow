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

#ifndef TENSORFLOW_NPU_PLUGIN_H_
#define TENSORFLOW_NPU_PLUGIN_H_

#include "ge/ge_api_types.h"
#include "ge_plugin.h"
#include <map>
#include <string>

const char *const AUTO_TUNE_MODE = ge::AUTO_TUNE_MODE.c_str();
const char *const OP_DEBUG_LEVEL = ge::OP_DEBUG_LEVEL.c_str();
const char *const OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES = ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES;
const char *const OPTION_EXEC_PROFILING_MODE = ge::OPTION_EXEC_PROFILING_MODE;
const char *const OPTION_EXEC_PROFILING_OPTIONS = ge::OPTION_EXEC_PROFILING_OPTIONS;
const char *const OPTION_GRAPH_RUN_MODE = ge::OPTION_GRAPH_RUN_MODE;
const char* const OPTION_EXEC_HCCL_FLAG = ge::OPTION_EXEC_HCCL_FLAG;

void PluginInit(std::map<std::string, std::string> &init_options);

void PluginFinalize();

#endif  // TENSORFLOW_NPU_PLUGIN_H_