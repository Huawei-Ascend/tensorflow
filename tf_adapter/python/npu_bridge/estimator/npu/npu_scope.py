"""
Config the non npu compilation scope for NPU in mix compute mode.
"""
import contextlib
from tensorflow.core.framework import attr_value_pb2
from tensorflow.python.framework import ops

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
