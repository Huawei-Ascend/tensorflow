# Custom function defs
load("@com_google_protobuf//:protobuf.bzl", "proto_gen")
def _proto_cc_hdrs(srcs, use_grpc_plugin = False):
    return [s[:-len(".proto")] + ".pb.h" for s in srcs]

def _proto_cc_srcs(srcs, use_grpc_plugin = False):
    return [s[:-len(".proto")] + ".pb.cc" for s in srcs]

def cc_proto_library(
        name,
        srcs = [],
        deps = [],
        cc_libs = [],
        include = None,
        protoc = "@com_google_protobuf//:protoc",
        **kargs):
    includes = []
    if include != None:
        includes = [include]
    gen_srcs = _proto_cc_srcs(srcs)
    gen_hdrs = _proto_cc_hdrs(srcs)
    outs = gen_srcs + gen_hdrs
    proto_gen(
        name = name + "_genproto",
        srcs = srcs,
        deps = [s + "_genproto" for s in deps],
        includes = includes,
        protoc = protoc,
        gen_cc = 1,
        outs = outs,
    )

    header_only_name = name + "_headers_only"
    impl_name = name + "_impl"

    native.cc_library(
        name = impl_name,
        srcs = gen_srcs,
        hdrs = gen_hdrs,
        deps = cc_libs + deps,
        includes = includes,
        **kargs
    )
    native.cc_library(
        name = header_only_name,
        hdrs = gen_hdrs,
        **kargs
    )