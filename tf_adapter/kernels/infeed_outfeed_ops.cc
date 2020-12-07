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

#include "securec.h"
#include "tdt/tdt_host_interface.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tf_adapter/common/common.h"
#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"
#include <string>

namespace tensorflow {
namespace {
Status GetTensorShape(const string &tensor_shape, TensorShape &shape) {
  // change "[32,224,224,3]" => "32,224,224,3"
  // tensor_shape.size() - 2 is the second to last
  string str = tensor_shape.substr(1, tensor_shape.size() - 2);
  string::size_type index = 0;
  if (!str.empty()) {
    while ((index = str.find(' ', index)) != string::npos) { str.erase(index, 1); }
  }
  string split = ",";
  string::size_type pos2 = str.find(split);
  string::size_type pos1 = 0;
  while (pos2 != string::npos) {
    try {
      shape.AddDim(std::stoi(str.substr(pos1, pos2 - pos1)));
    } catch (...) { return errors::InvalidArgument("Invalid shape string: ", tensor_shape); }
    // string::size_type can store the length of any string object
    pos1 = pos2 + split.size();
    pos2 = str.find(split, pos1);
  }
  if (pos1 != str.length()) {
    try {
      shape.AddDim(std::stoi(str.substr(pos1)));
    } catch (...) { return errors::InvalidArgument("Invalid shape string: ", tensor_shape); }
  }
  return Status::OK();
}

Status ConvertDataItem2Tensor(const std::vector<tdt::DataItem> &items, std::vector<Tensor> &tensors) {
  for (auto &item : items) {
    if (item.dataType_ == tdt::TDT_END_OF_SEQUENCE) {
      LOG(INFO) << "End of processing.";
      return Status::OK();
    }
    DataType type;
    DataTypeFromString(item.tensorType_, &type);
    if (type == DT_STRING) {
      Tensor result_tensor(tensorflow::DT_STRING, TensorShape({}));
      std::shared_ptr<std::string> data_str_ptr = std::static_pointer_cast<std::string>(item.dataPtr_);
      result_tensor.scalar<string>()() =
          std::move(string(reinterpret_cast<const char *>(data_str_ptr->c_str()), item.dataLen_));
      tensors.emplace_back(std::move(result_tensor));
    } else if (DataTypeCanUseMemcpy(type)) {
      TensorShape tensorShape;
      Status s = GetTensorShape(item.tensorShape_, tensorShape);
      if (!s.ok()) { return s; }
      Tensor result_tensor = Tensor(type, tensorShape);
      std::shared_ptr<std::string> data_str_ptr = std::static_pointer_cast<std::string>(item.dataPtr_);
      errno_t ret = memcpy_s(const_cast<char *>(result_tensor.tensor_data().data()), result_tensor.tensor_data().size(),
                             data_str_ptr->c_str(), item.dataLen_);
      if (ret != EOK) { return errors::Unknown("memcpy failed"); }
      tensors.emplace_back(std::move(result_tensor));
    } else {
      return errors::InvalidArgument("Not support this type: ", type);
    }
  }
  return Status::OK();
}

class OutfeedEnqueueOp : public OpKernel {
 public:
  explicit OutfeedEnqueueOp(OpKernelConstruction *ctx) : OpKernel(ctx) {
    OP_REQUIRES_OK(ctx, ctx->GetAttr("channel_name", &channel_name_));
    LOG(INFO) << "OutfeedEnqueueOp built";
  }
  ~OutfeedEnqueueOp() override { LOG(INFO) << "OutfeedEnqueueOp has been destructed"; }
  void Compute(OpKernelContext *ctx) override { LOG(INFO) << "OutfeedEnqueueOp running"; }
  bool IsExpensive() override { return false; }

 private:
  std::string channel_name_;
};

class OutfeedDequeueOp : public OpKernel {
 public:
  explicit OutfeedDequeueOp(OpKernelConstruction *ctx) : OpKernel(ctx) {
    // ctx is not nullptr
    OP_REQUIRES_OK(ctx, ctx->GetAttr("channel_name", &channel_name_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_types", &output_types_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_shapes", &output_shapes_));
    OP_REQUIRES(ctx, tdt::TdtHostPreparePopData() == 0, errors::Internal("Prepare Pop Data failed"));
    LOG(INFO) << "OutfeedDequeueOp built";
  }
  ~OutfeedDequeueOp() override { LOG(INFO) << "OutfeedDequeueOp has been destructed"; }
  void Compute(OpKernelContext *ctx) override {
    CHECK_NOT_NULL(ctx);
    std::vector<tdt::DataItem> bundle;
    OP_REQUIRES(ctx, tdt::TdtHostPopData(channel_name_, bundle) == 0,
                errors::Internal("TdtHostPopData get data failed"));
    std::vector<Tensor> out_tensors;
    OP_REQUIRES_OK(ctx, ConvertDataItem2Tensor(bundle, out_tensors));
    OP_REQUIRES(ctx, !out_tensors.empty(), errors::OutOfRange("Outfeed tensors reach the end"));
    OP_REQUIRES(
        ctx, out_tensors.size() == output_shapes_.size(),
        errors::Internal("Outfeed tensors num mismatch", out_tensors.size(), "vs. expect", output_shapes_.size()));
    for (int i = 0; i < ctx->num_outputs(); ++i) { ctx->set_output(i, out_tensors[i]); }
  }
  bool IsExpensive() override { return false; }

 private:
  DataTypeVector output_types_;
  std::vector<PartialTensorShape> output_shapes_;
  std::string channel_name_;
};

class StopOutfeedDequeueOp : public OpKernel {
 public:
  explicit StopOutfeedDequeueOp(OpKernelConstruction *ctx) : OpKernel(ctx) {
    OP_REQUIRES_OK(ctx, ctx->GetAttr("channel_name", &channel_name_));
    LOG(INFO) << "StopOutfeedDequeueOp built";
  }
  ~StopOutfeedDequeueOp() override { LOG(INFO) << "StopOutfeedDequeueOp has been destructed"; }
  void Compute(OpKernelContext *ctx) override {
    LOG(INFO) << "StopOutfeedDequeueOp running";
    OP_REQUIRES(ctx, tdt::TdtHostStop(channel_name_) == 0, errors::Internal("TdtHostStop failed"));
  }
  bool IsExpensive() override { return false; }

 private:
  std::string channel_name_;
};

REGISTER_KERNEL_BUILDER(Name("OutfeedDequeueOp").Device(DEVICE_CPU), OutfeedDequeueOp);

REGISTER_KERNEL_BUILDER(Name("OutfeedEnqueueOp").Device(DEVICE_CPU), OutfeedEnqueueOp);

REGISTER_KERNEL_BUILDER(Name("StopOutfeedDequeueOp").Device(DEVICE_CPU), StopOutfeedDequeueOp);
}  // namespace
}  // namespace tensorflow
