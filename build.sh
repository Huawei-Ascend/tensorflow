#!/bin/bash
# Copyright 2019-2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

set -e
BASE_PATH=$(cd "$(dirname $0)"; pwd)
RELEASE_PATH="${BASE_PATH}/output"
export BUILD_PATH="${BASE_PATH}/build"
INSTALL_PATH="${BUILD_PATH}/install"
CMAKE_PATH="${BUILD_PATH}/tfadapter"

# print usage message
usage() {
  echo "Usage:"
  echo "    bash build.sh [-h] [-j[n]] [-v] [-g]"
  echo ""
  echo "Options:"
  echo "    -h Print usage"
  echo "    -j[n] Set the number of threads used to build CANN, default is 8"
  echo "    -v Verbose"
  echo "    -g GCC compiler prefix, used to specify the compiler toolchain"
  echo "to be continued ..."
}

logging() {
  echo "[INFO] $@"
}

# parse and set optionss
checkopts() {
  VERBOSE=""
  THREAD_NUM=8
  GCC_PREFIX=""
  # Process the options
  while getopts 'hj:vg:' opt
  do
    case "${opt}" in
      h) usage
         exit 0 ;;
      j) THREAD_NUM=$OPTARG ;;
      v) VERBOSE="VERBOSE=1" ;;
      g) GCC_PREFIX=$OPTARG ;;
      *) logging "Undefined option: ${opt}"
         usage
         exit 1 ;;
    esac
  done
}

# mkdir directory
mk_dir() {
  local create_dir="$1"
  mkdir -pv "${create_dir}"
  logging "Created ${create_dir}"
}

# create build path
build_tfadapter() {
  logging "Create build directory and build tfadapter"
  cd "${BASE_PATH}" && ./configure
  CMAKE_ARGS="-DENABLE_OPEN_SRC=True -DBUILD_PATH=$BUILD_PATH"
  if [[ "$GCC_PREFIX" != "" ]]; then
    CMAKE_ARGS="$CMAKE_ARGS -DGCC_PREFIX=$GCC_PREFIX"
  fi
  logging "CMake Args: ${CMAKE_ARGS}"

  mk_dir "${CMAKE_PATH}"
  cd "${CMAKE_PATH}" && cmake ${CMAKE_ARGS} ../..
  make ${VERBOSE} -j${THREAD_NUM}
  logging "tfadapter build success!"
}

release_tfadapter() {
  logging "Create output directory"
  mk_dir "${RELEASE_PATH}"
  RELEASE_TARGET="tfadapter.tar"
  cd ${CMAKE_PATH}/dist/python/dist && mkdir -p tfplugin/bin && mv npu_bridge-*.whl tfplugin/bin && tar cfz "${RELEASE_TARGET}" * && mv "${RELEASE_TARGET}" "${RELEASE_PATH}"
}

main() {
  checkopts "$@"
  # tfadapter build start
  logging "---------------- tfadapter build start ----------------"
  ${GCC_PREFIX}g++ -v
  build_tfadapter
  release_tfadapter
  logging "---------------- tfadapter build finished ----------------"
}

main "$@"