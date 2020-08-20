/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved. foss@huawei.com

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

#include "tf_adapter/common/common.h"
#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/framework/partial_tensor_shape.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/kernels/data/captured_function.h"
#include "tensorflow/core/framework/dataset.h"
#include "tensorflow/core/lib/random/random.h"

namespace tensorflow {
namespace data {
namespace {
class GEOPDatasetOp : public DatasetOpKernel {
 public:
  explicit GEOPDatasetOp(OpKernelConstruction* ctx)
    : DatasetOpKernel(ctx),
      f_handle_(kInvalidHandle) {
    FunctionMetadata::Params params;
    OP_REQUIRES_OK(ctx,
      FunctionMetadata::Create(ctx, "f", params, &func_metadata_));
  }
  ~GEOPDatasetOp() {
    if (f_handle_ != kInvalidHandle && lib_ != nullptr) {
      LOG(INFO) << "Release function handle:" << f_handle_ << " owned by node instance:" << name();
      Status s = lib_->ReleaseHandle(f_handle_);
      if (s != Status::OK()) {
        LOG(INFO) << "Release function handle:" << f_handle_
                  << " owned by node instance:" << name()
                  << " failed. original err msg: " << s.error_message();
        return;
      }
    }
  }
  void MakeDataset(OpKernelContext* ctx, DatasetBase** output) override {
    CHECK_NOT_NULL(ctx);
    CHECK_NOT_NULL(output);
    *output = new (std::nothrow) Dataset(ctx, this);
    OP_REQUIRES(ctx, *output != nullptr,
      errors::Internal("Failed new dataset of GEOPDatasetOp"));
  }

 private:
  class Dataset : public DatasetBase {
   public:
    explicit Dataset(OpKernelContext* ctx,
                     GEOPDatasetOp* op_kernel)
        : DatasetBase(DatasetContext(ctx)),
          op_kernel_(op_kernel),
          tf_session_(ctx->session_handle()) {}

    ~Dataset() override {}

    GEOPDatasetOp* kernel() const {
      return op_kernel_;
    }

    std::string tf_session() const {
      return tf_session_;
    }

    std::unique_ptr<IteratorBase> MakeIteratorInternal(
        const string& prefix) const override {
      return std::unique_ptr<IteratorBase>(
        new (std::nothrow) Iterator({this, strings::StrCat(prefix, "::GEOP")}));
    }

    const DataTypeVector& output_dtypes() const override {
      const static DataTypeVector empty_types;
      return empty_types;
    }

    const std::vector<PartialTensorShape>& output_shapes() const override {
      const static std::vector<PartialTensorShape> empty_shapes;
      return empty_shapes;
    }

    string DebugString() const override { return "GEOPDatasetOp::Dataset"; }

   protected:
    Status AsGraphDefInternal(SerializationContext* ctx,
                              DatasetGraphDefBuilder* b,
                              Node** output) const override {
      return Status::OK();
    }

   private:
    class Iterator : public DatasetIterator<Dataset> {
     public:
      explicit Iterator(const Params& params)
          : DatasetIterator<Dataset>(params) {}
      ~Iterator() {}
      Status Initialize(IteratorContext* ctx) override {
        LOG(INFO) << "Start to initialize iterator of GEOPDatasetOp";
        REQUIRES_NOT_NULL(ctx);
        REQUIRES_NOT_NULL(dataset());
        REQUIRES_NOT_NULL(dataset()->kernel());
        mutex_lock l(mu_);
        if (dataset()->kernel()->f_handle_ == kInvalidHandle) {
          FunctionLibraryRuntime* lib = ctx->flr();
          REQUIRES_NOT_NULL(lib);
          FunctionLibraryRuntime::InstantiateOptions inst_opts;
          auto metadata = dataset()->kernel()->func_metadata_;
          REQUIRES_NOT_NULL(metadata);
          AddSessionInfo(*metadata->lib_def(), metadata->func().name(), dataset()->tf_session());
          inst_opts.lib_def = metadata->lib_def();
          inst_opts.create_kernels_eagerly = true;
          if (!metadata->use_inter_op_parallelism()) {
            inst_opts.executor_type = "SINGLE_THREADED_EXECUTOR";
          }
          inst_opts.is_multi_device_function = false;
          REQUIRES_NOT_NULL(lib->device());
          inst_opts.target = lib->device()->name();
          TF_RETURN_IF_ERROR(lib->Instantiate(
            metadata->func().name(), AttrSlice(&metadata->func().attr()),
            inst_opts, &dataset()->kernel()->f_handle_));
            dataset()->kernel()->lib_ = lib;
        }
        FunctionLibraryRuntime* lib = ctx->flr();
        REQUIRES_NOT_NULL(lib);
        REQUIRES_NOT_NULL(lib->device());
        REQUIRES_NOT_NULL(lib->device()->resource_manager());
        std::vector<Tensor> args;
        std::vector<Tensor> out_tensors;
        FunctionLibraryRuntime::Options f_opts;
        ScopedStepContainer step_container(
          f_opts.step_id, [lib](const string& name) {
            lib->device()->resource_manager()->Cleanup(name).IgnoreError();
          });
        f_opts.step_container = &step_container;
        f_opts.runner = ctx->runner();
        CancellationManager cancellation_manager;
        f_opts.cancellation_manager = &cancellation_manager;
        Notification n;
        Status s;
        lib->Run(f_opts, dataset()->kernel()->f_handle_, std::move(args),
          &out_tensors, [&n, &s](Status func_status) {
            s.Update(func_status);
            n.Notify();
          });
        n.WaitForNotification();
        if (!s.ok()) {
          LOG(ERROR) << s;
        }
        return s;
      }

      void AddSessionInfo(const FunctionLibraryDefinition &flib_def,
                            std::string func_name, std::string session) {
        FunctionDef* fdef = const_cast<FunctionDef *>(flib_def.Find(func_name));
        if (fdef != nullptr) {
          for (NodeDef& ndef : *fdef->mutable_node_def()) {
            std::string node_name = ndef.name();
            if (str_util::StartsWith(node_name, "GeOp_")) {
              AddNodeAttr("_session", session, &ndef);
              LOG(INFO) << "Node " << node_name << " add session info "
                        << session << " success.";
            }
          }
        }
      }

      Status GetNextInternal(IteratorContext* ctx,
                             std::vector<Tensor>* out_tensors,
                             bool* end_of_sequence) override {
        *end_of_sequence = true;
        return Status::OK();
      }

     protected:
      Status SaveInternal(IteratorStateWriter* writer) override {
        return Status::OK();
      }

      Status RestoreInternal(IteratorContext* ctx,
                             IteratorStateReader* reader) override {
        return Status::OK();
      }

     private:
      mutex mu_;
    };
    GEOPDatasetOp* op_kernel_;
    std::string tf_session_;
  };
  std::shared_ptr<FunctionMetadata> func_metadata_ = nullptr;
  FunctionLibraryRuntime::Handle f_handle_;
  FunctionLibraryRuntime* lib_;
};

REGISTER_KERNEL_BUILDER(Name("GEOPDataset").Device(DEVICE_CPU), GEOPDatasetOp);
}  // namespace
}  // namespace data
}  // namespace tensorflow
