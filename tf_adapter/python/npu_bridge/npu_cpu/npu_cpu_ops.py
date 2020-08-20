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
