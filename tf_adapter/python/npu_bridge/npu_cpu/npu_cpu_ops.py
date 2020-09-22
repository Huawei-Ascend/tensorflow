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
from tensorflow.contrib.util import loader
from tensorflow.python.framework import load_library
from tensorflow.python.framework import ops
from tensorflow.python.platform import resource_loader
from npu_bridge.helper import helper

gen_npu_cpu_ops = helper.get_gen_ops();


## 提供embeddingrankid功能
#  @param addr_tensor tensorflow的tensor类型，embeddingrankid操作的输入；
#  @param index tensorflow的tensor类型，embeddingrankid操作的输入；
#  @param row_memory int类型，一行数据存储的大小 默认为320。
#  @param mode string类型，embeddingrankid的操作类型，可以为”mod”,”order”;数据存储的方式。
#  @return 对输入addr_tensor，index_tensor执行完embeddingrankid操作之后的结果tensor
def embeddingrankid(addr_tensor, index, row_memory=320, mode='mod'):
    result = gen_npu_cpu_ops.embedding_rank_id(
        addr_table=addr_tensor,
        index=index,
        row_memory=row_memory,
        mode=mode)
    return result
