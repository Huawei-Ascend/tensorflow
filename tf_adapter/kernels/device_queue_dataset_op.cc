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

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/framework/dataset.h"
#include "tensorflow/core/framework/node_def_builder.h"
#include "tensorflow/core/lib/core/blocking_counter.h"
#include "tensorflow/core/lib/io/buffered_inputstream.h"
#include "tf_adapter/common/common.h"

namespace tensorflow {
namespace data {
namespace {
class DeviceQueueDatasetOp : public DatasetOpKernel {
 public:
  using DatasetOpKernel::DatasetOpKernel;

  explicit DeviceQueueDatasetOp(OpKernelConstruction *ctx) : DatasetOpKernel(ctx) {
    CHECK_NOT_NULL(ctx);
    OP_REQUIRES_OK(ctx, ctx->GetAttr("channel_name", &defStr_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_types", &outputTypes_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_shapes", &outputShapes_));
  }
  ~DeviceQueueDatasetOp() override = default;
  void MakeDataset(OpKernelContext *ctx, DatasetBase **output) override {
    CHECK_NOT_NULL(ctx);
    CHECK_NOT_NULL(output);
    *output = new (std::nothrow) Dataset(ctx, defStr_, outputTypes_, outputShapes_);
    OP_REQUIRES(ctx, *output != nullptr, errors::InvalidArgument("DeviceQueueDatasetOp: new dataset failed"));
  }

 private:
  class Dataset : public DatasetBase {
   public:
    explicit Dataset(OpKernelContext *ctx, string &sourcedata, const DataTypeVector &outputTypes,
                     const std::vector<PartialTensorShape> &outputShapes)
        : DatasetBase(DatasetContext(ctx)), sourcedata_(sourcedata), outputTypes_(outputTypes),
          outputShapes_(outputShapes) {}

    ~Dataset() override = default;

    std::unique_ptr<IteratorBase> MakeIteratorInternal(const string &prefix) const override {
      return std::unique_ptr<IteratorBase>(new Iterator({this, strings::StrCat(prefix, "::DeviceQueue")}));
    }

    const DataTypeVector &output_dtypes() const override { return outputTypes_; }

    const std::vector<PartialTensorShape> &output_shapes() const override { return outputShapes_; }

    string DebugString() const override { return "DeviceQueueDatasetOp::Dataset"; }

   protected:
    Status AsGraphDefInternal(SerializationContext *ctx, DatasetGraphDefBuilder *b, Node **output) const override {
      REQUIRES_NOT_NULL(ctx);
      REQUIRES_NOT_NULL(b);
      REQUIRES_NOT_NULL(output);
      Node *sourcedata = nullptr;
      TF_RETURN_IF_ERROR(b->AddScalar(sourcedata_, &sourcedata));
      TF_RETURN_IF_ERROR(b->AddDataset(this, {sourcedata}, output));
      return Status::OK();
    }

   private:
    class Iterator : public DatasetIterator<Dataset> {
     public:
      explicit Iterator(const Params &params) : DatasetIterator<Dataset>(params) {}

      ~Iterator() override = default;

      Status GetNextInternal(IteratorContext *ctx, std::vector<Tensor> *out_tensors, bool *end_of_sequence) override {
        *end_of_sequence = false;
        return Status::OK();
      };

     private:
      std::mutex mu_;
      std::unique_ptr<IteratorBase> inputImpl_;
    };
    std::string sourcedata_;
    const DataTypeVector outputTypes_;
    const std::vector<PartialTensorShape> outputShapes_;
  };
  string defStr_;
  DataTypeVector outputTypes_;
  std::vector<PartialTensorShape> outputShapes_;
};

REGISTER_KERNEL_BUILDER(Name("DeviceQueueDataset").Device(DEVICE_CPU), DeviceQueueDatasetOp);
}  // namespace
}  // namespace data
}  // namespace tensorflow
