/**
* Copyright (C) <2019>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
* Description: a plugin control GEInitialize and GEFinalize
*/
#ifndef TENSORFLOW_NPU_PLUGIN_H_
#define TENSORFLOW_NPU_PLUGIN_H_

#include <map>
#include <string>
#include "ge_plugin.h"
#include "ge/ge_api_types.h"

const char* const AUTO_TUNE_MODE = ge::AUTO_TUNE_MODE.c_str();
const char* const OP_DEBUG_LEVEL = ge::OP_DEBUG_LEVEL.c_str();
const char* const OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES = ge::OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES;
const char* const OPTION_EXEC_PROFILING_MODE = ge::OPTION_EXEC_PROFILING_MODE;
const char* const OPTION_EXEC_PROFILING_OPTIONS = ge::OPTION_EXEC_PROFILING_OPTIONS;
const char* const OPTION_GRAPH_RUN_MODE = ge::OPTION_GRAPH_RUN_MODE;

void PluginInit(std::map<std::string, std::string>& init_options);

void PluginFinalize();

#endif  // TENSORFLOW_NPU_PLUGIN_H_