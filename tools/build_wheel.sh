#!/bin/bash
# Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.
# Description: Build npu_bridge pip-packages
# ==============================================================================

set -e
set -o pipefail

dst=$1
PRODUCT=$2

if [[ -z "${dst}" ]]; then
    echo -e "\033[7mNo destination dir provided\033[0m"
    exit 1
fi
if [[ "${PRODUCT}" = "cloud" ]]; then
    echo "Build npu_bridge in cloud."
    CUSTOM_COMMANDS="--define product=cloud"
elif [[ "${PRODUCT}" = "mini" ]]; then
    echo "Build npu_bridge in mini."
    CUSTOM_COMMANDS="--define product=mini"
elif [[ "${PRODUCT}" = "onetrack" ]]; then
    echo "Build npu_bridge in onetrack."
    CUSTOM_COMMANDS="--define product=onetrack"
else
    echo "TF_Adapter not support this product."
    exit 1
fi

PYTHON_BIN_PATH=$(which python3.7)
if [[ -z "${PYTHON_BIN_PATH}" ]]; then
    echo -e "\033[7mNo python3 installed\033[0m"
    exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")/../"
TOP_DIR="$(cd ../../../../../ && pwd)"
INSTALL_DIR="${TOP_DIR}/out/${PRODUCT}/host"
OUTPUT_DIR="${INSTALL_DIR}/obj/tf_adapter"
SOFT_DP_DIR="${INSTALL_DIR}/obj/lib"
mkdir -p "${OUTPUT_DIR}/genfiles"
mkdir -p "${OUTPUT_DIR}/wheel"

TF_INSTALL_LIB_PATH=$("${PYTHON_BIN_PATH:-python3}" -c 'import tensorflow as tf; print(tf.sysconfig.get_lib())')
echo "Found installed tensorflow ${TF_INSTALL_LIB_PATH}"

TF_PACKAGE_PATH="$(cd ../../../compile_deps/tf-1.15.0 && pwd)"
#if [ -n "$(uname -a|grep Ubuntu|grep x86_64)" ];then
#    TF_INSTALL_LIB_PATH="$(cd ../../../compile_deps/tf-1.15.0/lib/ubuntu_x86_64 && pwd)"
#elif [ -n "$(uname -a|grep Ubuntu|grep aarch64)" ];then
#    TF_INSTALL_LIB_PATH="$(cd ../../../compile_deps/tf-1.15.0/lib/ubuntu_aarch64 && pwd)"
#elif [ -n "$(uname -a|grep euleros|grep x86_64)" ];then
#    TF_INSTALL_LIB_PATH="$(cd ../../../compile_deps/tf-1.15.0/lib/euleros_x86_64 && pwd)"
#elif [ -n "$(uname -a|grep euleros|grep aarch64)" ];then
#    TF_INSTALL_LIB_PATH="$(cd ../../../compile_deps/tf-1.15.0/lib/euleros_aarch64 && pwd)"
#elif [ -n "$(uname -a|grep Debian|grep x86_64)" ];then
#    TF_INSTALL_LIB_PATH="$(cd ../../../compile_deps/tf-1.15.0/lib/debian_x86_64 && pwd)"
#else
#    echo "TF_Adapter not support this OS."
#    exit 1
#fi

#echo "Found installed tensorflow lib ${TF_INSTALL_LIB_PATH}"

cp workspace.bzl.tpl workspace.bzl
sed -i "s|{TF_INSTALL_LIB_PATH}|"${TF_INSTALL_LIB_PATH}"|g" workspace.bzl
cp WORKSPACE.tpl WORKSPACE
sed -i "s|TF_PACKAGE_PATH|"${TF_PACKAGE_PATH}"|g" WORKSPACE

## use gcc-7.3.0
#bazel --output_base=${OUTPUT_DIR}/genfiles build ${CUSTOM_COMMANDS} --config=opt //tf_adapter:_tf_adapter.so
/opt/buildtools/bazel-0.24.1/bin/bazel --output_base=${OUTPUT_DIR}/genfiles build ${CUSTOM_COMMANDS} --config=opt //tf_adapter:_tf_adapter.so

cp -r tf_adapter/python/. "${OUTPUT_DIR}/wheel"
cp -f bazel-bin/tf_adapter/_tf_adapter.so "${OUTPUT_DIR}/wheel/npu_bridge"
cp ${SOFT_DP_DIR}/libSoftDp.so "${OUTPUT_DIR}/wheel/npu_bridge"

cd "${OUTPUT_DIR}/wheel"
"${PYTHON_BIN_PATH:-python3.7}" setup.py bdist_wheel >/dev/null

cp -f dist/*.whl "$1"
