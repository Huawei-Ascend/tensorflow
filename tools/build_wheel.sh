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

#PYTHON_BIN_PATH=$(which python3.7)
PYTHON_BIN_PATH=${HI_PYTHON_PATH}
if [[ -z "${PYTHON_BIN_PATH}" ]]; then
    echo -e "\033[7mNo python3 installed\033[0m"
    exit 1
fi
PYTHON_INCLUDE_PATH=${HI_PYTHON_INC}
if [[ -z "${PYTHON_INCLUDE_PATH}" ]]; then
    echo -e "\033[7mNot found python3 include path\033[0m"
    exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")/../"
TOP_DIR="$(cd ../../../../../ && pwd)"
INSTALL_DIR="${TOP_DIR}/out/${PRODUCT}/host"
OUTPUT_DIR="${INSTALL_DIR}/obj/tf_adapter"
SOFT_DP_DIR="${INSTALL_DIR}/obj/lib"
mkdir -p "${OUTPUT_DIR}/genfiles"
mkdir -p "${OUTPUT_DIR}/wheel"

TF_INSTALL_LIB_PATH=$("${PYTHON_BIN_PATH}" -c 'import tensorflow as tf; print(tf.sysconfig.get_lib())')
echo "Found installed tensorflow ${TF_INSTALL_LIB_PATH}"
TF_PACKAGE_PATH="$(cd ../../../compile_deps/tf-1.15.0 && pwd)"

cp workspace.bzl.tpl workspace.bzl
sed -i "s|{TF_INSTALL_LIB_PATH}|"${TF_INSTALL_LIB_PATH}"|g" workspace.bzl
sed -i "s|{PYTHON_INCLUDE_PATH}|"${PYTHON_INCLUDE_PATH}"|g" workspace.bzl
cp WORKSPACE.tpl WORKSPACE
sed -i "s|TF_PACKAGE_PATH|"${TF_PACKAGE_PATH}"|g" WORKSPACE

## use gcc-7.3.0
#bazel --output_base=${OUTPUT_DIR}/genfiles build ${CUSTOM_COMMANDS} --config=opt //tf_adapter:_tf_adapter.so
/opt/buildtools/bazel-0.24.1/bin/bazel --output_base=${OUTPUT_DIR}/genfiles build ${CUSTOM_COMMANDS} --config=opt //tf_adapter:_tf_adapter.so

cp -r tf_adapter/python/. "${OUTPUT_DIR}/wheel"
cp -f bazel-bin/tf_adapter/_tf_adapter.so "${OUTPUT_DIR}/wheel/npu_bridge"
cp ${SOFT_DP_DIR}/libSoftDp.so "${OUTPUT_DIR}/wheel/npu_bridge"

cd "${OUTPUT_DIR}/wheel"
"${PYTHON_BIN_PATH}" setup.py bdist_wheel >/dev/null

cp -f dist/*.whl "$1"
