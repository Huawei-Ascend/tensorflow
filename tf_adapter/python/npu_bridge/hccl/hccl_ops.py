## @file hccl_ops.py
# HCCL 算子API

from tensorflow.contrib.util import loader
from tensorflow.python.framework import load_library
from tensorflow.python.framework import ops
from tensorflow.python.platform import resource_loader
from npu_bridge.helper import helper

gen_hccl_ops = helper.get_gen_ops();


## 提供group内的集合通信allreduce功能
#  @param tensor tensorflow的tensor类型，allreduce操作的输入；
#  @param reduction string类型，reduce的操作类型，可以为”max”,”min”,”prod”和”sum”;
#  @param fusion int类型，算子融合标识。0: 不融合；1: 按照梯度切分设置融合，默认融； 2: 按照相同fusion_id融合。
#  @param fusion_id int类型，算子融合索引标识，相同fusion_id的算子将会融合。
#  @param group string类型，group名称，可以为用户自定义group或者"hccl_world_group";
#  @return 对输入tensor执行完allreduce操作之后的结果tensor
def allreduce(tensor, reduction, fusion=1, fusion_id=-1, group="hccl_world_group"):
    result = gen_hccl_ops.hcom_all_reduce(
        input=tensor,
        reduction=reduction,
        fusion=fusion,
        fusion_id=fusion_id,
        group=group)
    return result


@ops.RegisterGradient('HcomAllReduce')
def _allreduce_grad(op, grad):
    return allreduce(grad, "sum", fusion=0)


## 提供group内的集合通信allgather功能
#  @param tensor tensorflow的tensor类型，allgather操作的输入；
#  @param rank_size int类型，group内device的数量;
#  @param group string类型，group名称，可以为用户自定义group或者"hccl_world_group";
#  @return 对输入tensor执行完allgather操作之后的结果tensor
def allgather(tensor, rank_size, group="hccl_world_group"):
    result = gen_hccl_ops.hcom_all_gather(
        input=tensor,
        group=group,
        rank_size=rank_size)
    return result


## 提供group内的集合通信broadcast功能
#  @param tensor tensorflow的tensor类型，broadcast操作的输入；
#  @param root_rank int类型，作为root节点的rank_id，该id是group内的rank id;
#  @param fusion int类型，算子融合标识。0: 不融合；1:按照相同fusion_id融合;其他值非法。
#  @param fusion_id int类型，算子融合索引标识，相同fusion_id的算子将会融合。
#  @param group string类型，group名称，可以为用户自定义group或者"hccl_world_group";
#  @return 对输入tensor执行完broadcast操作之后的结果tensor
def broadcast(tensor, root_rank, fusion=0, fusion_id=-1, group="hccl_world_group"):
    result = gen_hccl_ops.hcom_broadcast(
        input=tensor,
        fusion=fusion,
        fusion_id=fusion_id,
        group=group,
        root_rank=root_rank)
    return result


## 提供group内的集合通信reduce_scatter功能
#  @param tensor tensorflow的tensor类型，reduce_scatter操作的输入；
#  @param reduction string类型，reduce的操作类型，可以为”max”,”min”,”prod”和”sum”;
#  @param rank_size int类型，group内device的数量;
#  @param group string类型，group名称，可以为用户自定义group或者"hccl_world_group";
#  @return 对输入tensor执行完reduce_scatter操作之后的结果tensor
def reduce_scatter(tensor, reduction, rank_size, group="hccl_world_group"):
    result = gen_hccl_ops.hcom_reduce_scatter(
        input=tensor,
        reduction=reduction,
        group=group,
        rank_size=rank_size)
    return result


## 提供group内的集合通信send功能
#  @param tensor tensorflow的tensor类型，send操作的输入；
#  @param sr_tag int类型，消息标签，相同sr_tag的send/recv对可以收发数据;
#  @param dest_rank int类型，数据的目标节点，该rank是group中的rank id;
#  @param group string类型，group名称，可以为用户自定义group或者"hccl_world_group";
def send(tensor, sr_tag, dest_rank, group="hccl_world_group"):
    result = gen_hccl_ops.hcom_send(
        input=tensor,
        group=group,
        sr_tag=sr_tag,
        dest_rank=dest_rank)
    return result


## 提供group内的集合通信receive功能
#  @param shape 接收tensor的shape；
#  @param data_type 接收tensor的数据类型；
#  @param sr_tag int类型，消息标签，相同sr_tag的send/recv对可以收发数据;
#  @param dest_rank int类型，数据的目标节点，该rank是group中的rank id;
#  @param group string类型，group名称，可以为用户自定义group或者"hccl_world_group";
def receive(shape, data_type, sr_tag, src_rank, group="hccl_world_group"):
    result = gen_hccl_ops.hcom_receive(
        shape=shape,
        T=data_type,
        group=group,
        sr_tag=sr_tag,
        src_rank=src_rank)
    return result

## 提供remote read功能
#  @param remote 远端内存信息，shape(index_num, 3)：[u64 remoteId, u64 remoteAddr, u64 dataLength]
#  @param data_type 接收tensor的数据类型
#  @return 本端接收内存 shape(index_num, dataLength/sizeof(data_type))
def remote_read(tensorRemote, data_type):
    result = gen_hccl_ops.hcom_remote_read(
        remote=tensorRemote,
        dtype=data_type)
    return result

## 提供remote write功能
#  @param remote 写入远端内存信息，shape(index_num, 3)：[u64 remoteId, u64 remoteAddr, u64 dataLength]
#  @param local 本端发送内存
def remote_write(tensorRemote, tensorLocal, data_type):
    result = gen_hccl_ops.hcom_remote_write(
        remote=tensorRemote,
        local=tensorLocal)
    return result