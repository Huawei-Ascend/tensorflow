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