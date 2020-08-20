# Build tf_adapter workspace(Calling by tf_adapter)
def extend_base():
    return "../../../../../"

def tf_adapter_workspace():
    native.new_local_repository(
        name = "tf_adapter_cloud_host_libs",
        path = extend_base() + "out/cloud/host/obj/lib",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "tf_adapter_mini_host_libs",
        path = extend_base() + "out/mini/host/obj/lib",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "tf_adapter_onetrack_host_libs",
        path = extend_base() + "out/onetrack/host/obj/lib",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "ge_proto",
        path = extend_base() + "inc/common",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "tf_adapter_extend_hdrs",
        path = extend_base() + "inc",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name="installed_tensorflow",
        path=extend_base() + "third_party/tensorflow/compile_deps/tf-1.15.0",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name="tf_adapter_dvpp_hdrs",
        path=extend_base() + "soft_dp",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name="installed_tensorflow_libs",
        path="{TF_INSTALL_LIB_PATH}",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "local_nlohmann_json_lib",
        path = extend_base() + "third_party/json/include/",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "sec_lib",
        path = extend_base() + "libc_sec/include/",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "python_include",
        path = "/opt/buildtools/python3.7/include/python3.7m/",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )
