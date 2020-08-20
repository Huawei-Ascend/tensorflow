package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "_tf_adapter.so",
    srcs = glob(["common/*.*", "kernels/*.*", "ops/*.*", "optimizers/*.*", "util/*.*"]),
    data = [] + select({
        # Public introduction of external dependencies on project.
        # External linked libraries, typically, located in out/${product}/host/obj/lib
        ":cloud_build": ["@tf_adapter_cloud_host_libs//:tf_adapter_host_libs"],
        ":mini_build": ["@tf_adapter_mini_host_libs//:tf_adapter_host_libs"],
        ":onetrack_build": ["@tf_adapter_onetrack_host_libs//:tf_adapter_host_libs"],
        "//conditions:default": [],
    }),
    copts = [
            # We must ensure that this is the first header file lookup directory,
            # so that can ensure the protobuf version is consistent with the installed tensorflow
            "-Iexternal/installed_tensorflow/include",
            "-Iexternal/org_tensorflow",
            "-Iexternal/tf_adapter_extend_hdrs",
            "-Iexternal/tf_adapter_extend_hdrs/external",
            "-Iexternal/local_nlohmann_json_lib/",
            "-Iexternal/sec_lib",
            "-Iexternal/tf_adapter_dvpp_hdrs",
            "-Iexternal/python_include",
        ] + select({
            #":dbg": ["-g",],
            "//conditions:default": [],
        }),
    linkshared = 1,
    linkopts = [] + select({
                # Public introduction of external dependencies on project.
                # External linked libraries, typically, located in out/${product}/host/obj/lib
                ":cloud_build": ["-Lexternal/tf_adapter_cloud_host_libs/ -lc_sec -lge_runner -ltsdclient -ldatatransfer -lfmk_parser"],
                ":mini_build": ["-Lexternal/tf_adapter_mini_host_libs/ -lc_sec -lge_runner -ltsdclient -ldatatransfer -lfmk_parser",],
                ":onetrack_build": ["-Lexternal/tf_adapter_onetrack_host_libs/ -lc_sec -lge_runner -ltsdclient -ldatatransfer -lfmk_parser",],
                "//conditions:default": [],
            }) + [
            # "-z defs",
            # tensorflow prebuilt libs
            # we must ensure that libtensorflow_framework.so is linked
            # before any tf_adapter_host_libs to avoid
            # the latter overridding the symbol of tensorfdlow,
            "-Lexternal/installed_tensorflow_libs/ -l:libtensorflow_framework.so.1",
            # We link _pywrap_tensorflow_internal.so, because we use the implementation of the
            # capture function, which is pretty bad but doesn't have good processing.
            "-Lexternal/installed_tensorflow_libs/python/ -l:_pywrap_tensorflow_internal.so",
            # "-Wl,--version-script",  #  This line must be directly followed by the version_script.lds file
            # "$(location //tf_adapter:exported_symbols.lds)",
        ],
    deps = [
        # Specifies the symbols to export
        # "//tf_adapter:exported_symbols.lds",
        # Installed tensorflow library and header files,
        # We use the prefix foo_ because Eigen's headers doesn't end with.h or.hpp
        "@installed_tensorflow//:foo_tensorflow_hdrs",
        # Tensorflow internal headers. In general, the introduction of such headers means
        # that you are using functions that Tensorflow does not want to expose
        "//tf_adapter:tf_inner_headers_lib",
        #
        "@installed_tensorflow_libs//:installed_tensorflow_libs",
        "@local_nlohmann_json_lib//:local_nlohmann_json_lib",
        "@ge_proto//:ge_proto_headers_only",
        "@tf_adapter_extend_hdrs//:tf_adapter_extend_hdrs",
        "@sec_lib//:sec_lib",
        "@tf_adapter_dvpp_hdrs//:tf_adapter_dvpp_hdrs",
        "@python_include//:python_include",
    ],
)

exports_files(
    [
        "exported_symbols.lds",
    ],
)

config_setting(
    name = "cloud_build",
    define_values ={
        "product": "cloud",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "mini_build",
    define_values ={
        "product": "mini",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "onetrack_build",
    define_values ={
        "product": "onetrack",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "dbg",
    define_values = {
        "dbg": "true",
    },
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tf_inner_headers_lib",
    hdrs = [
        "@org_tensorflow//tensorflow/core/kernels/data:captured_function.h",
        "@org_tensorflow//tensorflow/core/kernels/data:dataset.h",
        "@org_tensorflow//tensorflow/core/kernels/data:iterator_ops.h",
        "@org_tensorflow//tensorflow/core/kernels/data:dataset_utils.h",
        "@org_tensorflow//tensorflow/core/kernels/data:unbounded_thread_pool.h",
        "@org_tensorflow//tensorflow/core/kernels:ops_util.h",
        "@org_tensorflow//tensorflow/compiler/jit/graphcycles:graphcycles.h",
    ],
)
