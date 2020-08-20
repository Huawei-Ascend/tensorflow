/**
* Copyright (c) Huawei Technologies Co., Ltd. 2019. All rights reserved.
* Description: Implement the custom host queue dataset to get and send data
*/

#include <dlfcn.h>
#include <thread>
#include <vector>
#include "tf_adapter/common/common.h"
#include "tdt/tdt_host_interface.h"
#include "tensorflow/core/framework/partial_tensor_shape.h"
#include "tensorflow/core/framework/stats_aggregator.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/framework/variant_tensor_data.h"
#include "tensorflow/core/kernels/data/dataset.h"
#include "tensorflow/core/lib/core/error_codes.pb.h"
#include "tensorflow/core/lib/gtl/cleanup.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/util/env_var.h"

#include "unistd.h"

namespace tensorflow {
namespace data {
namespace {
using namespace std;
using namespace tdt;

const static int kMaxDeviceId = 7;
const static uint32_t kMaxValue = 128;
// total memory usage controlled below 2G
const uint64_t kTotalBytes = 2147483648;
std::atomic<bool> tdt_release(false);

using InitFunc = int (*)(uint32_t);
using PushDataFunc = int (*)(const string &, const vector<DataItem> &);
using DestroyFunc = int (*)();

class HostQueueDatasetOp : public DatasetOpKernel {
 public:
  explicit HostQueueDatasetOp(OpKernelConstruction* ctx)
      : DatasetOpKernel(ctx) {
    // ctx is not nullptr
    OP_REQUIRES_OK(ctx, ctx->GetAttr("channel_name", &channel_name_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_types", &output_types_));
    OP_REQUIRES_OK(ctx, ctx->GetAttr("output_shapes", &output_shapes_));
    LOG(INFO) << "Start to init tdt.";
    string lib_path = "libdatatransfer.so";
    handle_ = dlopen(lib_path.c_str(), RTLD_NOW);
    OP_REQUIRES(ctx, handle_ != nullptr,
                errors::InvalidArgument("libdatatransfer.so dlopen failed."));
    init_api_ = (InitFunc)dlsym(handle_, "TdtHostInit");
    push_api_ = (PushDataFunc)dlsym(handle_, "TdtHostPushData");
    destroy_api_ = (DestroyFunc)dlsym(handle_, "TdtHostDestroy");
    OP_REQUIRES(ctx, init_api_ != nullptr && push_api_ != nullptr &&
      destroy_api_ != nullptr, errors::InvalidArgument("dlsym tdt API failed."));
    int64 id = -1;
    OP_REQUIRES_OK(ctx, ReadInt64FromEnvVar("DEVICE_ID", -1, &id));

    OP_REQUIRES(ctx, id >= 0 && id <= kMaxDeviceId,
      errors::InvalidArgument("device_id should be in [0, 7]."));
    uint32_t u_id = (uint32_t)id;
    int32_t tdt_status = (*init_api_)(u_id);
    OP_REQUIRES(ctx, tdt_status == 0,
      errors::InvalidArgument("Tdt client init failed."));
    tdt_release = false;
  }
  ~HostQueueDatasetOp() {
    LOG(INFO) << "Start to destroy tdt.";
    if (!tdt_release) {
      int32_t tdt_status = (*destroy_api_)();
      if (tdt_status != 0) {
        LOG(ERROR) << "Tdt client close failed.";
      } else {
        LOG(INFO) << "Tdt client close success.";
        tdt_release = true;
      }
    }
    if (handle_ != nullptr) {
      dlclose(handle_);
      LOG(INFO) << "dlclose handle finish.";
    } else {
      LOG(INFO) << "handle is null.";
    }
  }
  void MakeDataset(OpKernelContext* ctx, DatasetBase** output) override {
    std::vector<DatasetBase*> inputs;
    CHECK_NOT_NULL(output);
    for (int i = 0; i < ctx->num_inputs(); ++i) {
      DatasetBase* input = nullptr;
      OP_REQUIRES_OK(ctx, GetDatasetFromVariantTensor(ctx->input(i), &input));
      inputs.push_back(input);
    }
    *output = new (nothrow) Dataset(ctx, this, inputs, channel_name_, output_types_, output_shapes_);
    OP_REQUIRES(
        ctx, *output != nullptr,
        errors::InvalidArgument(
            "Data process host queue dataset op: new dataset failed."));
  }

 private:
  class Dataset : public DatasetBase {
   public:
    Dataset(OpKernelContext* ctx, HostQueueDatasetOp* op_kernel, const std::vector<DatasetBase*>& inputs,
            const string& channelName, const DataTypeVector& outputTypes,
            const vector<PartialTensorShape>& outputShapes)
        : DatasetBase(DatasetContext(ctx)),
          op_kernel_(op_kernel),
          inputs_(inputs),
          channel_name_(channelName),
          output_types_(outputTypes),
          output_shapes_(outputShapes) {
      for (const auto& input : inputs_) {
        input->Ref();
      }
    }

    ~Dataset() override {
      for (const auto& input : inputs_) {
        input->Unref();
      }
    }

    HostQueueDatasetOp* kernel() const {
      return op_kernel_;
    }

    unique_ptr<IteratorBase> MakeIteratorInternal(
        const string& prefix) const override {
      return unique_ptr<IteratorBase>(new (nothrow) Iterator(
          {this, strings::StrCat(prefix, "::HostQueue")}));
    }

    const DataTypeVector& output_dtypes() const override {
      return output_types_;
    }
    const vector<PartialTensorShape>& output_shapes() const override {
      return output_shapes_;
    }

    string DebugString() const override {
      return "HostQueueDatasetOp::Dataset";
    }

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

      ~Iterator() override {
        {
          mutex_lock lck(mu_);
          finish_send_ = true;
        }
        // wait for tdt destory for sleeping one second
        sleep(1);
        {
          mutex_lock lck(mu_);
          cancelled_ = true;
          cond_var_.notify_all();
        }
        LOG(INFO) << "HostQueueDatasetOp's iterator is released.";
      }

      void GetDataThread(const std::shared_ptr<IteratorContext>& ctx) {
        RecordStart(ctx.get());
        auto cleanup = gtl::MakeCleanup([this, ctx] { RecordStop(ctx.get()); });
        while (true) {
          {
            mutex_lock lck(mu_);
            while (!cancelled_ && (buffer_.size() >= kMaxValue ||
                                   total_bytes_ > kTotalBytes)) {
              RecordStop(ctx.get());
              cond_var_.wait(lck);
              RecordStart(ctx.get());
            }

            if (cancelled_) {
              return;
            }
          }

          mutex_lock parent_l(parent_mu_);
          vector<Tensor> args;
          bool end_of_sequence = false;
          BufferElement buffer_element;
          buffer_element.status =
              input_impls_[1]->GetNext(ctx.get(), &args, &end_of_sequence);

          if (!buffer_element.status.ok() ||
              (buffer_element.status.ok() && end_of_sequence)) {
            if (!buffer_element.status.ok()) {
              LOG(ERROR) << "Failed to get tensor data, Status:"
                         << buffer_element.status.ToString();
            } else {
              LOG(INFO) << "Finish to get tensor data, Status:"
                        << buffer_element.status.ToString()
                        << "; end_of_sequence:" << end_of_sequence;
            }
            mutex_lock lck(mu_);
            buffer_element.host_thread_finished = true;
            buffer_.push_back(std::move(buffer_element));
            cond_var_.notify_all();
            return;
          }

          {
            mutex_lock lck(mu_);
            for (auto& tensor : args) {
              if (tensor.TotalBytes() > UINT64_MAX - total_bytes_) {
                LOG(ERROR) << "the size of tensor is too big";
                buffer_element.host_thread_finished = true;
                buffer_.push_back(std::move(buffer_element));
                cond_var_.notify_all();
                return;
              }
              total_bytes_ += tensor.TotalBytes();
            }
            buffer_element.value = args;
            buffer_.push_back(std::move(buffer_element));
            cond_var_.notify_all();
          }
        }
      }
      void SendDataThread(const std::shared_ptr<IteratorContext>& ctx) {
        vector<Tensor> args;
        while (!cancelled_) {
          {
            mutex_lock lck(mu_);
            if (buffer_.empty()) {
              RecordStop(ctx.get());
              cond_var_.wait(lck);
              RecordStart(ctx.get());
            }
          }

          {
            mutex_lock l(mu_);
            if (buffer_.front().host_thread_finished) {
              std::vector<DataItem> items;
              DataItem end_item;
              if (buffer_.front().status.ok()) {
                end_item.dataType_ = TDT_END_OF_SEQUENCE;
                LOG(INFO) << "Push data finish, end_of_sequence_ is true.";
              } else {
                end_item.dataType_ = TDT_ABNORMAL;
                LOG(ERROR) << "Get data failed.";
              }
              items.emplace_back(end_item);
              int32_t tdt_status =
                  (*(dataset()->kernel()->push_api_))(dataset()->channel_name_, items);
              if (tdt_status != 0) {
                LOG(ERROR) << "Push the end data to tdt failed.";
              }
              cancelled_ = true;
              cond_var_.notify_all();
              return;
            } else {
              args = buffer_.front().value;
            }
          }

          string value;
          uint64_t total_bytes = 0;
          std::vector<DataItem> items;
          for (auto& tensor : args) {
            DataItem data_item;
            data_item.dataType_ = TDT_TENSOR;
            data_item.tensorShape_ = tensor.shape().DebugString();
            data_item.tensorType_ = DataTypeString(tensor.dtype());

            if (DataTypeCanUseMemcpy(tensor.dtype())) {
              data_item.dataLen_ = tensor.tensor_data().size();
              data_item.dataPtr_ = std::shared_ptr<void>(
                  const_cast<char*>(tensor.tensor_data().data()),
                  [](void* elem) {});
            } else if (tensor.dtype() == DT_STRING) {
              if (tensor.dims() != 0) {
                LOG(ERROR) << "input of DT_STRING type should be scalar,"
                  " current dims:" << tensor.dims();
                mutex_lock lck(mu_);
                cancelled_ = true;
                cond_var_.notify_all();
                return;
              }
              value = tensor.scalar<string>()();
              data_item.dataLen_ = value.size();
              data_item.dataPtr_ = std::shared_ptr<void>(
                  const_cast<char*>(value.data()), [](void* elem) {});
            } else {
              LOG(ERROR) << "Unexpected data type.";
              mutex_lock lck(mu_);
              cancelled_ = true;
              cond_var_.notify_all();
              return;
            }
            items.push_back(data_item);
            // total_bytes is smaller than total_bytes_
            total_bytes += tensor.TotalBytes();
          }
          // call tdt interface
          int32_t tdt_status = (*(dataset()->kernel()->push_api_))(dataset()->channel_name_, items);
          if (tdt_status != 0 || cancelled_ || finish_send_) {
            mutex_lock lck(mu_);
            cancelled_ = true;
            LOG(INFO) << "End training.";
            cond_var_.notify_all();
            return;
          }
          mutex_lock lck(mu_);
          buffer_.pop_front();
          // total_bytes is smaller than total_bytes_
          total_bytes_ -= total_bytes;
          cond_var_.notify_all();
        }
      }

      Status EnsureReceiveThreadStarted(IteratorContext* ctx)
          EXCLUSIVE_LOCKS_REQUIRED(mu_) {
        // ctx is not nullptr
        if (!receive_thread_) {
          std::shared_ptr<IteratorContext> new_ctx(new (std::nothrow) IteratorContext(*ctx));
          REQUIRES_NOT_NULL(new_ctx);
          REQUIRES_NOT_NULL(ctx->env());
          receive_thread_.reset(ctx->env()->StartThread(
              {}, "receive_thread",
              [this, new_ctx]() { GetDataThread(new_ctx); }));
        }
        return Status::OK();
      }

      Status EnsureSendThreadStarted(IteratorContext* ctx)
          EXCLUSIVE_LOCKS_REQUIRED(mu_) {
        if (!send_thread_) {
          std::shared_ptr<IteratorContext> new_ctx(new (std::nothrow) IteratorContext(*ctx));
          REQUIRES_NOT_NULL(new_ctx);
          REQUIRES_NOT_NULL(ctx->env());
          send_thread_.reset(ctx->env()->StartThread(
              {}, "send_thread",
              [this, new_ctx]() { SendDataThread(new_ctx); }));
        }
        return Status::OK();
      }

      Status Initialize(IteratorContext* ctx) override {
        LOG(INFO) << "Start to check channel name. channelName: "
                  << dataset()->channel_name_;
        if (dataset()->channel_name_.empty()) {
          return errors::InvalidArgument(
              "HostQueueDataset channel_name is null.");
        }

        LOG(INFO) << "Start to check receive and send thread.";
        try {
          input_impls_.resize(dataset()->inputs_.size());
        } catch (...) {
          return errors::InvalidArgument(
              "HostQueueDataset resize failed.");
        }

        for (size_t i = 0; i < input_impls_.size(); ++i) {
          TF_RETURN_IF_ERROR(dataset()->inputs_[i]->MakeIterator(
              ctx, strings::StrCat(prefix(), "[", i, "]"), &input_impls_[i]));
        }
        {
          mutex_lock lck(mu_);
          TF_RETURN_IF_ERROR(EnsureReceiveThreadStarted(ctx));
          TF_RETURN_IF_ERROR(EnsureSendThreadStarted(ctx));
        }

        LOG(INFO) << "HostQueue success to Initialize. channelName: " << dataset()->channel_name_;
        return Status::OK();
      }

      Status GetNextInternal(IteratorContext* ctx, vector<Tensor>* outTensors,
                             bool* endOfSequence) override {
        *endOfSequence = false;
        LOG(INFO) << "HostQueue Get Next data.";
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
      struct BufferElement {
        bool host_thread_finished = false;
        Status status;
        vector<Tensor> value;
      };
      // This mutex is used to ensure exclusivity between multiple threads
      // reading/writing this iterator's local state.
      mutex mu_;
      // This mutex is used to ensure exclusivity between multiple threads
      // accessing the parent iterator. We keep this separate from `mu_` to
      // allow prefetching to run in parallel with GetNext calls.
      mutex parent_mu_ ACQUIRED_BEFORE(mu_);
      std::vector<std::unique_ptr<IteratorBase>> input_impls_ GUARDED_BY(mu_);
      condition_variable cond_var_;
      string prefix_end_;
      std::deque<BufferElement> buffer_ GUARDED_BY(mu_);
      std::unique_ptr<Thread> receive_thread_ GUARDED_BY(mu_);
      std::unique_ptr<Thread> send_thread_ GUARDED_BY(mu_);
      bool cancelled_ GUARDED_BY(mu_) = false;
      bool finish_send_ GUARDED_BY(mu_) = false;
      bool host_thread_finished_ GUARDED_BY(mu_) = false;
      uint64_t total_bytes_ GUARDED_BY(mu_) = 0;
    };
    HostQueueDatasetOp* op_kernel_;
    const std::vector<DatasetBase*> inputs_;
    std::string channel_name_;
    const DataTypeVector output_types_;
    const vector<PartialTensorShape> output_shapes_;
  };
  std::string channel_name_;
  DataTypeVector output_types_;
  vector<PartialTensorShape> output_shapes_;
  void* handle_;
  InitFunc init_api_;
  PushDataFunc push_api_;
  DestroyFunc destroy_api_;
};

REGISTER_KERNEL_BUILDER(Name("HostQueueDataset").Device(DEVICE_CPU),
                        HostQueueDatasetOp);
}  // namespace
}  // namespace data
}  // namespace tensorflow
