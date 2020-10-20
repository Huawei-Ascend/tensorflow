"""
Config the non npu compilation scope for NPU in mix compute mode.
"""
import contextlib
from tensorflow.core.framework import attr_value_pb2
from tensorflow.python.framework import ops
from tensorflow.python.util import compat
from npu_bridge.estimator.npu.npu_config import NpuExecutePlacement

@contextlib.contextmanager
def without_npu_compile_scope():
  '''
  Enable the non npu compilation of operators within the scope.
  '''
  attrs = {
    "_without_npu_compile" : attr_value_pb2.AttrValue(b=True)
  }

  with ops.get_default_graph()._attr_scope(attrs):
    yield

@contextlib.contextmanager
def npu_variable_scope(placement=NpuExecutePlacement.ALL):
  '''
  Enable the node in the scope adding _variable_placement attr.
  '''
  if placement not in NpuExecutePlacement:
    raise ValueError("placement vaule must be in NpuExecutePlacement's vaule")

  attrs = {
    "_variable_placement" : attr_value_pb2.AttrValue(s=compat.as_bytes(placement.value))
  }
  with ops.get_default_graph()._attr_scope(attrs):
    yield