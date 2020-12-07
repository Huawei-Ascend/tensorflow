# Build tf_adapter workspace(Calling by tf_adapter)
def extend_base():
    return "../"

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
        name = "tf_adapter_extend_hdrs",
        path = extend_base() + "inc",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "graphengine_extend_hdrs",
        path = extend_base() + "graphengine/inc",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name = "metadef_extend_hdrs",
        path = extend_base() + "metadef/inc",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )

    native.new_local_repository(
        name="installed_tensorflow",
        path=extend_base() + "third_party/tensorflow/compile_deps/tf-1.15.0",
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
        path = "{PYTHON_INCLUDE_PATH}",
        build_file = "@tf_adapter//tf_adapter:module.BUILD",
    )
