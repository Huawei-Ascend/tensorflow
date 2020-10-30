workspace(name = "tf_adapter")

local_repository(
    name="org_tensorflow",
    path="../third_party/tensorflow/tensorflow-1.15.0/",
)

# TensorFlow depends on "io_bazel_rules_closure" so we need this here.
# Needs to be kept in sync with the same target in TensorFlow's WORKSPACE file.
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "io_bazel_rules_closure",
    sha256 = "5b00383d08dd71f28503736db0500b6fb4dda47489ff5fc6bed42557c07c6ba9",
    strip_prefix = "rules_closure-308b05b2419edb5c8ee0471b67a40403df940149",
    urls = [
        "file://TF_PACKAGE_PATH/308b05b2419edb5c8ee0471b67a40403df940149.tar.gz",
        "https://storage.googleapis.com/mirror.tensorflow.org/github.com/bazelbuild/rules_closure/archive/308b05b2419edb5c8ee0471b67a40403df940149.tar.gz",
        "https://github.com/bazelbuild/rules_closure/archive/308b05b2419edb5c8ee0471b67a40403df940149.tar.gz",  # 2019-06-13
    ],
)

http_archive(
    name = "bazel_skylib",
    sha256 = "2ef429f5d7ce7111263289644d233707dba35e39696377ebab8b0bc701f7818e",
    urls = [
        "file://TF_PACKAGE_PATH/bazel-skylib.0.8.0.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/0.8.0/bazel-skylib.0.8.0.tar.gz",
    ],
)  # https://github.com/bazelbuild/bazel-skylib/releases


http_archive(
    name = "com_google_protobuf",
    sha256 = "e9883cbd0807eb35f9e837cf7ff1d12a7a0303fef4fffb340632c98a2bbd4e2d",
    strip_prefix = "protobuf-3.8.0",
    urls = [
        "file://TF_PACKAGE_PATH/v3.8.0.tar.gz",
        "https://storage.googleapis.com/mirror.tensorflow.org/github.com/protocolbuffers/protobuf/archive/310ba5ee72661c081129eb878c1bbcec936b20f0.tar.gz",
        "https://github.com/protocolbuffers/protobuf/archive/310ba5ee72661c081129eb878c1bbcec936b20f0.tar.gz",
    ],
)

# protobuf depends on @zlib, it has to be renamed to @zlib_archive because "zlib" is already
# defined using bind for grpc
http_archive(
    name = "zlib_archive",
    build_file = "@org_tensorflow//:third_party/zlib.BUILD",
    sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
    strip_prefix = "zlib-1.2.11",
    urls = [
        "file://TF_PACKAGE_PATH/zlib-1.2.11.tar.gz",
    ],
)


load('@org_tensorflow//tensorflow:workspace.bzl', 'tf_workspace')
tf_workspace()

load('//:workspace.bzl', 'tf_adapter_workspace')
tf_adapter_workspace()
