%module tf_adapter
%include "std_string.i"
%include "std_map.i"
%include "typemaps.i"
%include "std_vector.i"

%begin %{
#define SWIG_PYTHON_CAST_MODE
%}

typedef int int32_t;
%apply int { int32_t };

#if defined(SMALL_LONG)

typedef long long int64_t;
typedef unsigned long long uint64_t;
%apply long long { int64_t };
%apply unsigned long long { uint64_t };

#else

typedef long int int64_t;
typedef unsigned long int uint64_t;
%apply long int { int64_t };
%apply unsigned long int { uint64_t };

#endif

namespace std{
    %template(map_string_string) map<string, string>;
}

%apply uint64_t &OUTPUT { uint64_t &base_addr, uint64_t &var_size, uint64_t &dev_addr, uint64_t &memory_size };

%{
#include "tf_adapter/util/npu_plugin.h"

extern int32_t RdmaInitAndRegister(const std::vector<ge::HostVarInfo> &var_info, size_t size);

extern int32_t GetVarAddrAndSize(const std::string &var_name, uint64_t &base_addr, uint64_t &var_size);

extern int32_t MallocSharedMem(const ge::TensorInfo &tensor_info, uint64_t &dev_addr, uint64_t &memory_size);
%}

%template(var_info_vec) std::vector<ge::HostVarInfo>;
%template(int64_vec) std::vector<int64_t>;

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

namespace ge{
  struct HostVarInfo {
    uint64_t base_addr;
    uint64_t var_size;
  };
  enum DataType {
    DT_FLOAT = 0,            // float type
    DT_FLOAT16 = 1,          // fp16 type
    DT_INT8 = 2,             // int8 type
    DT_INT16 = 6,            // int16 type
    DT_UINT16 = 7,           // uint16 type
    DT_UINT8 = 4,            // uint8 type
    DT_INT32 = 3,            // int32 type
    DT_INT64 = 9,            // int64 type
    DT_UINT32 = 8,           // unsigned int32
    DT_UINT64 = 10,          // unsigned int64
    DT_BOOL = 12,            // bool type
    DT_DOUBLE = 11,          // double type
    DT_STRING = 13,          // string type
    DT_DUAL_SUB_INT8 = 14,   // dual output int8 type
    DT_DUAL_SUB_UINT8 = 15,  // dual output uint8 type
    DT_COMPLEX64 = 16,       // complex64 type
    DT_COMPLEX128 = 17,      // complex128 type
    DT_QINT8 = 18,           // qint8 type
    DT_QINT16 = 19,          // qint16 type
    DT_QINT32 = 20,          // qint32 type
    DT_QUINT8 = 21,          // quint8 type
    DT_QUINT16 = 22,         // quint16 type
    DT_RESOURCE = 23,        // resource type
    DT_STRING_REF = 24,      // string ref type
    DT_DUAL = 25,            // dual output type
    DT_UNDEFINED             // Used to indicate a DataType field has not been set.
  };
  struct TensorInfo {
    std::string var_name;
    std::vector<int64_t> dims;
    DataType data_type;
  };
}

extern int32_t RdmaInitAndRegister(const std::vector<ge::HostVarInfo> &var_info, size_t size);

extern int32_t GetVarAddrAndSize(const std::string &var_name, uint64_t &base_addr, uint64_t &var_size);

extern int32_t MallocSharedMem(const ge::TensorInfo &tensor_info, uint64_t &dev_addr, uint64_t &memory_size);