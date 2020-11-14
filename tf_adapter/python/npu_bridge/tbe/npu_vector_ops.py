# Copyright 2016 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
"""Ops for aicore cube."""
from tensorflow import Tensor
from tensorflow.python.eager import context
from npu_bridge.helper import helper
gen_npu_ops = helper.get_gen_ops()

def lamb_apply_optimizer_assign(input0, input1, input2, input3, mul0_x, mul1_x,
                                mul2_x, mul3_x, add2_y, steps, do_use_weight, weight_decay_rate, name=None):
    if context.executing_eagerly():
      raise RuntimeError("tf.lamb_apply_optimizer_assign() is not compatible with "
                        "eager execution.")
    update, nextv, nextm = gen_npu_ops.lamb_apply_optimizer_assign(input0, input1, input2, input3, mul0_x, mul1_x, mul2_x,
                                                     mul3_x, add2_y, steps, do_use_weight, weight_decay_rate, name)
    return update, nextv, nextm

def lamb_apply_weight_assign(input0, input1, input2, input3, input4, name=None):
    if context.executing_eagerly():
      raise RuntimeError("tf.lamb_apply_weight_assign() is not compatible with "
                        "eager execution.")
    result = gen_npu_ops.lamb_apply_weight_assign(input0, input1, input2, input3, input4, name)
    return result