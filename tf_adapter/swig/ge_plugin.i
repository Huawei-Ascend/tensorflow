%module tf_adapter
%include "std_string.i"
%include "std_map.i"

namespace std{
    %template(map_string_string) map<string, string>;
}

%{
#include "tf_adapter/util/npu_plugin.h"
%}

extern const char* const AUTO_TUNE_MODE;
extern const char* const OP_DEBUG_LEVEL;
extern const char* const OPTION_EXEC_ENABLE_SCOPE_FUSION_PASSES;
extern const char* const OPTION_EXEC_PROFILING_MODE;
extern const char* const OPTION_EXEC_PROFILING_OPTIONS;
extern const char* const OPTION_GRAPH_RUN_MODE;
extern const char* const OPTION_EXEC_HCCL_FLAG;
extern const char* const OPTION_EXEC_PROFILING_FPPONIT_OPTIONS;
extern const char* const OPTION_EXEC_PROFILING_BPPONIT_OPTIONS;


extern void PluginInit(std::map<std::string, std::string>& init_options);

extern void PluginFinalize();
