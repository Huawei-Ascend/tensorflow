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


def deformable_conv2d(  # pylint: disable=redefined-builtin
        x,
        filter,
        offsets,
        bias=None,
        strides=None,
        pads=None,
        data_format='NHWC',
        dilations=(1, 1, 1, 1),
        groups=1,
        deformable_groups=1,
        name=None):
    """Computes a 2-D deformable convolution given 4-D `x`, `filters` and
        `offsets` tensors.

    Inputs:
      x: A 4D `Tensor` of input `image`. With the `data_format` `NHWC`, the
        data is stored in the order of: [batch, in_height, in_width,
        in_channels]. Must be one of the following types: `float16`, `float32`.
      filters: A 4D `Tensor` of learnable filters. Must have the same type as
        `x`. The data is stored in the order of: `[filter_height, filter_width,
        in_channels / groups, out_channels]`.
      offsets: A 4D `Tensor` of x y coordinates offset and mask. With the
        `data_format` `NHWC`, the data is stored in the order of: `[batch,
        out_height, out_width, deformable_groups * filter_height * filter_width
        * 3]`. Must be one of the following types: `float16`, `float32`.
      bias: A optional 1D `Tensor` of additive biased to the filter outputs.
        The data is stored in the order of: `[out_channels]`. Must be one of
        the following types: `float16`, `float32`.

    Attributes:
      strides: Required. An list of `4` `ints`. The stride of the sliding
        window for each dimension of `image`. The dimension order is
        interpreted according to the value of `data_format`. The `N` and `C`
        dimensions must be set to 1.
      pads: Required. An list of `4` `ints`. The number of pixels to add to
        each `(pad_top, pad_bottom, pad_left, pad_right)` side of the `image`.
      data_format: Optional. A `string` from: `NHWC`, `NCHW`. Specify the
        data format of the input and output data. Defaults to `NHWC`.
      dilations: Optional. An list of `4` `ints`. The dilation factor for each
        dimension of `image`. The dimension order is interpreted according to
        the value of `data_format`. The `N` and `C` dimensions must be set to
        1. Defaults to `(1, 1, 1, 1)`.
      groups: Optional. An `int`. The number of blocked connections from
        `in_channels` to `out_channels`. `In_channels` and `out_channels` must
        both be divisible by `groups`. Defaults to 1.
      deformable_groups: Optional. An `int`. The number of deformable group
        partitions. `In_channels` must be divisible by `deformable_groups`.
        Defaults to 1.
      name: Optional. A name for the operation.

    Returns:
      A 4D `Tensor` of output feature map. Has the same type as `x`. With the
        `data_format` `NHWC`, the data is stored in the order of: `[batch,
        out_height, out_width, out_channels]`.

        out_height = (in_height + pad_top + pad_bottom) -
                      (dilation_h * filter_height - 1) + 1))
                     / stride_h + 1
        out_width  = (in_width + pad_left + pad_right) -
                      (dilation_w * filter_width - 1) + 1))
                     / stride_w + 1
    """

    if context.executing_eagerly():
        raise RuntimeError("tf.deformable_conv2d() is not compatible with "
                           "eager execution.")

    if isinstance(bias, Tensor):
        op_res = gen_npu_ops.deformable_conv2d_with_bias(
            x=x,
            filter=filter,
            offsets=offsets,
            bias=bias,
            strides=strides,
            pads=pads,
            data_format=data_format,
            dilations=dilations,
            groups=groups,
            deformable_groups=deformable_groups,
            name=name)
    else:
        op_res = gen_npu_ops.deformable_conv2d(
            x=x,
            filter=filter,
            offsets=offsets,
            strides=strides,
            pads=pads,
            data_format=data_format,
            dilations=dilations,
            groups=groups,
            deformable_groups=deformable_groups,
            name=name)
    return op_res
