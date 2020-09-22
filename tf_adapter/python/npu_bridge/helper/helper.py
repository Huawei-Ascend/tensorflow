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
import tensorflow
import npu_bridge
import os
try:
    npu_bridge_handle = tensorflow.load_op_library(os.path.dirname(npu_bridge.__file__) + "/_tf_adapter.so")
except Exception as e:
    print(str(e))
def get_gen_ops():
    return npu_bridge_handle

version = 'v1.15.0'