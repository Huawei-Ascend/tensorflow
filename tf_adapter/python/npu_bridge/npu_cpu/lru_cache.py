from tensorflow.contrib.util import loader
from tensorflow.python.framework import load_library
from tensorflow.python.framework import ops
from tensorflow.python.platform import resource_loader
from npu_bridge.helper import helper

gen_npu_cpu_ops = helper.get_gen_ops()

class LruCache(object):
  def __init(self, cache_size=100000, load_factor=1):
    self._cache_size=cache_size
    self._load_factor=load_factor
    self._cache=gen_npu_cpu_ops.lru_cache(
        cache_size=self._cache_size,
        load_factor=self._load_factor
    )
  
  ##提供CacheAdd功能
  # @param cache resource类型，保存lruCache资源
  # @param ids int类型，输入索引
  # @return result 对ids执行完swap in/swap out操作后输出的索引张量
  def cache_add(self, ids):
    result=gen_npu_cpu_ops.cache_add(
        cache=self._cache,
        ids=ids
    )
  
  ##提供CacheRemoteIndexToLocal功能
  # @param cache resource类型，保存lruCache资源
  # @param ids int类型，输入索引
  # return result 输入ids对应cache中的索引
  def cache_remote_index_to_local(self, ids):
    result=gen_npu_cpu_ops.cache_remote_index_to_local(
        cache=self._cache,
        ids=ids
    )