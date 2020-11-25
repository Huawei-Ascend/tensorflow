/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/core/framework/common_shape_fns.h"
#include "tensorflow/core/framework/op.h"

namespace tensorflow {
using shape_inference::DimensionHandle;
using shape_inference::InferenceContext;
using shape_inference::ShapeHandle;

Status Conv2dInferShape(shape_inference::InferenceContext *c) {
  std::string dt_format;
  const std::set<std::string> kVaildFormat = {"NHWC", "NCHW"};
  if (!c->GetAttr("data_format", &dt_format).ok()) {
    dt_format = "NHWC";
  }
  if (kVaildFormat.find(dt_format) == kVaildFormat.end()) {
    return errors::InvalidArgument("Invalid data format string: ",
                                   dt_format);
  }
  size_t pos_n = dt_format.find("N");
  size_t pos_c = dt_format.find("C");
  size_t pos_h = dt_format.find("H");
  size_t pos_w = dt_format.find("W");

  const int rank = 4;
  ShapeHandle x_shape;
  TF_RETURN_IF_ERROR(c->WithRank(c->input(0), rank, &x_shape));
  DimensionHandle x_n_dim = c->Dim(x_shape, pos_n);
  DimensionHandle x_c_dim = c->Dim(x_shape, pos_c);
  DimensionHandle x_h_dim = c->Dim(x_shape, pos_h);
  DimensionHandle x_w_dim = c->Dim(x_shape, pos_w);
  if (!c->ValueKnown(x_n_dim) ||
      !c->ValueKnown(x_c_dim) ||
      !c->ValueKnown(x_h_dim) ||
      !c->ValueKnown(x_w_dim)) {
    return errors::InvalidArgument("Invalid x shape value");
  }
  int64 x_n = c->Value(x_n_dim);
  int64 x_c = c->Value(x_c_dim);
  int64 x_h = c->Value(x_h_dim);
  int64 x_w = c->Value(x_w_dim);
  // Filter format is always HWCN
  ShapeHandle filter_shape;
  TF_RETURN_IF_ERROR(c->WithRank(c->input(1), rank, &filter_shape));
  DimensionHandle filter_h_dim = c->Dim(filter_shape, 0);
  DimensionHandle filter_w_dim = c->Dim(filter_shape, 1);
  DimensionHandle filter_c_dim = c->Dim(filter_shape, 2);
  DimensionHandle filter_n_dim = c->Dim(filter_shape, 3);
  if (!c->ValueKnown(filter_h_dim) ||
      !c->ValueKnown(filter_w_dim) ||
      !c->ValueKnown(filter_c_dim) ||
      !c->ValueKnown(filter_n_dim)) {
    return errors::InvalidArgument("Invalid filter shape value");
  }
  int64 filter_n = c->Value(filter_n_dim);
  int64 filter_c = c->Value(filter_c_dim);
  int64 filter_h = c->Value(filter_h_dim);
  int64 filter_w = c->Value(filter_w_dim);
  ShapeHandle offsets_shape;
  TF_RETURN_IF_ERROR(c->WithRank(c->input(2), rank, &offsets_shape));
  DimensionHandle offsets_n_dim = c->Dim(offsets_shape, pos_n);
  DimensionHandle offsets_c_dim = c->Dim(offsets_shape, pos_c);
  DimensionHandle offsets_h_dim = c->Dim(offsets_shape, pos_h);
  DimensionHandle offsets_w_dim = c->Dim(offsets_shape, pos_w);
  if (!c->ValueKnown(offsets_n_dim) ||
      !c->ValueKnown(offsets_c_dim) ||
      !c->ValueKnown(offsets_h_dim) ||
      !c->ValueKnown(offsets_w_dim)) {
    return errors::InvalidArgument("Invalid offsets shape value");
  }
  int64 offsets_n = c->Value(offsets_n_dim);
  int64 offsets_c = c->Value(offsets_c_dim);
  int64 offsets_h = c->Value(offsets_h_dim);
  int64 offsets_w = c->Value(offsets_w_dim);
  if (offsets_n != x_n) {
    return errors::InvalidArgument(
        "Offsets batch size (", offsets_n,
        ") should be equal to x (", x_n, ")");
  }
  int32 groups;
  TF_RETURN_IF_ERROR(c->GetAttr("groups", &groups));
  if (groups < 1) {
    return errors::InvalidArgument("Groups should be >= 1");
  }
  if (x_c != filter_c * groups) {
    return errors::InvalidArgument(
        "In_channels (", x_c,
        ") should be equal to filter channels (", filter_c,
        ") * groups (", groups, ")");
  }
  if (filter_n % groups != 0) {
    return errors::InvalidArgument(
        "Out_channels (", filter_n,
        ") should be divisiable by groups (", groups, ")");
  }
  int32 dfm_groups;
  TF_RETURN_IF_ERROR(c->GetAttr("deformable_groups", &dfm_groups));
  if (dfm_groups < 1) {
    return errors::InvalidArgument("Deformable_groups should be >= 1");
  }
  if (x_c % dfm_groups != 0) {
    return errors::InvalidArgument(
        "In_channels (", x_c,
        ") should be divisiable by deformable_groups (", dfm_groups, ")");
  }
  int64 exp_offset_c = dfm_groups * filter_h * filter_w * 3;
  if (offsets_c != exp_offset_c) {
    return errors::InvalidArgument(
        "Offsets channels (", offsets_c,
        ") should be equal to ", exp_offset_c);
  }
  if (c->num_inputs() == 4) {
    ShapeHandle bias_shape;
    TF_RETURN_IF_ERROR(c->WithRank(c->input(3), 1, &bias_shape));
    DimensionHandle bias_n_dim = c->Dim(bias_shape, 0);
    if (c->ValueKnown(bias_n_dim) &&
        c->Value(bias_n_dim) != filter_n) {
      return errors::InvalidArgument(
          "Bias size (", c->Value(bias_n_dim),
          ") should be equal to out_channels (", filter_n, ")");
    }
  }

  std::vector<int32> dilations;
  TF_RETURN_IF_ERROR(c->GetAttr("dilations", &dilations));
  if (dilations.size() != 4) {
    return errors::InvalidArgument(
        "Dilations attribute should contain 4 values, but got: ",
        dilations.size());
  }
  const int32 dil_h = dilations[pos_h];
  const int32 dil_w = dilations[pos_w];
  if (dil_h < 1 || dil_w < 1) {
    return errors::InvalidArgument("Dilation rate should be >= 1");
  }
  if (dilations[pos_n] != 1 || dilations[pos_c] != 1) {
    return errors::InvalidArgument(
        "Dilations N and C dimensions must be set to 1");
  }
  std::vector<int32> strides;
  TF_RETURN_IF_ERROR(c->GetAttr("strides", &strides));
  if (strides.size() != 4) {
    return errors::InvalidArgument(
        "Strides attribute should contain 4 values, but got: ",
        strides.size());
  }
  const int32 str_h = strides[pos_h];
  const int32 str_w = strides[pos_w];
  if (str_h < 1 || str_w < 1) {
    return errors::InvalidArgument("Stride should be >= 1");
  }
  if (strides[pos_n] != 1 || strides[pos_c] != 1) {
    return errors::InvalidArgument(
        "Stride N and C dimensions must be set to 1");
  }
  std::vector<int32> pads;
  TF_RETURN_IF_ERROR(c->GetAttr("pads", &pads));
  if (pads.size() != 4) {
    return errors::InvalidArgument(
        "Pads attribute should contain 4 values, but got: ",
        pads.size());
  }
  int64 pad_t = pads[0];
  int64 pad_b = pads[1];
  int64 pad_l = pads[2];
  int64 pad_r = pads[3];
  if (pad_t < 0 || pad_b < 0 || pad_l < 0 || pad_r < 0) {
    return errors::InvalidArgument("Pad should be >= 0");
  }

  int64 dil_filter_h = dil_h * (filter_h - 1) + 1;
  int64 dil_filter_w = dil_w * (filter_w - 1) + 1;
  int64 pad_image_h = x_h + pad_t + pad_b;
  int64 pad_image_w = x_w + pad_l + pad_r;
  if ((pad_image_h - dil_filter_h) < 0 || (pad_image_w - dil_filter_w) < 0) {
    return errors::InvalidArgument(
        "Image size after padding (", pad_image_h, ", ", pad_image_w,
        ") should not be smaller than filter size after dilation (",
        dil_filter_h, ", ", dil_filter_w,")");
  }
  int64 out_h = (pad_image_h - dil_filter_h) / str_h + 1;
  int64 out_w = (pad_image_w - dil_filter_w) / str_w + 1;
  if (offsets_h != out_h) {
    return errors::InvalidArgument(
        "Offsets height (", offsets_h,
        ") should be equal to output (", out_h, ")");
  }
  if (offsets_w != out_w) {
    return errors::InvalidArgument(
        "Offsets width (", offsets_w,
        ") should be equal to output (", out_w, ")");
  }
  std::vector<DimensionHandle> out_dims(rank);
  out_dims[pos_n] = x_n_dim;
  out_dims[pos_c] = filter_n_dim;
  out_dims[pos_h] = c->MakeDim(out_h);
  out_dims[pos_w] = c->MakeDim(out_w);
  c->set_output(0, c->MakeShape(out_dims));
  return Status::OK();
}

REGISTER_OP("DeformableConv2D")
    .Input("x: T")
    .Input("filter: T")
    .Input("offsets: T")
    .Output("y: T")
    .Attr("T: {float16, float32}")
    .Attr("strides: list(int)")
    .Attr("pads: list(int)")
    .Attr("data_format: {'NHWC', 'NCHW'} = 'NHWC'")
    .Attr("dilations: list(int) = [1,1,1,1]")
    .Attr("groups: int = 1")
    .Attr("deformable_groups: int = 1")
    .SetShapeFn(Conv2dInferShape);

REGISTER_OP("DeformableConv2DWithBias")
    .Input("x: T")
    .Input("filter: T")
    .Input("offsets: T")
    .Input("bias: T")
    .Output("y: T")
    .Attr("T: {float16, float32}")
    .Attr("strides: list(int)")
    .Attr("pads: list(int)")
    .Attr("data_format: {'NHWC', 'NCHW'} = 'NHWC'")
    .Attr("dilations: list(int) = [1,1,1,1]")
    .Attr("groups: int = 1")
    .Attr("deformable_groups: int = 1")
    .SetShapeFn(Conv2dInferShape);
}  // namespace tensorflow