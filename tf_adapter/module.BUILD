package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hdrs",
    srcs = glob(["**/*.h", "**/*.hpp"]),
)

cc_library(
    name = "foo_tensorflow_hdrs",
    hdrs = glob(["include/**/*"], exclude=["include/**/*.so"]),
)

cc_library(
    name = "tf_adapter_extend_hdrs",
    srcs = glob([
        "hccl/**/*.h", "hccl/**/*.hpp",
        "tdt/**/*.h", "tdt/**/*.hpp",
        "runtime/**/*.h", "runtime/**/*.hpp",
        "external/**/*.h", "soft_dp/**/*.h",
        "mmpa/**/*.h", "toolchain/**/*.h"
    ]),
)

cc_library(
    name = "graphengine_extend_hdrs",
    srcs = glob([
        "framework/**/*.h", "framework/**/*.hpp",
        "external/**/*.h"
    ]),
)

cc_library(
    name = "metadef_extend_hdrs",
    srcs = glob([
        "graph/**/*.h", "graph/**/*.hpp",
        "external/**/*.h"
    ]),
)

cc_library(
    name = "tf_adapter_host_libs",
    srcs = glob([
        "libc_sec.so",
        "libge_runner.so",
        "libdatatransfer.so",
        "libfmk_parser.so",
        "libindextransform.so",
        "libmmpa"
    ]),
)

cc_library(
    name = "installed_tensorflow_libs",
    srcs = glob([
    ]),
)

cc_library(
    name = "local_nlohmann_json_lib",
    hdrs = glob([
        "nlohmann/**/*.hpp",
    ]),
    visibility = ["//visibility:public"],
    alwayslink = 1,
)

cc_library(
    name = "sec_lib",
    srcs = glob([
        "**/*.h",
    ]),
)

cc_library(
    name = "python_include",
    srcs = glob([
        "**/*.h",
    ]),
)
