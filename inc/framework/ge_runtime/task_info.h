/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INC_FRAMEWORK_GE_RUNTIME_TASK_INFO_H_
#define INC_FRAMEWORK_GE_RUNTIME_TASK_INFO_H_

#include <stdint.h>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cce/taskdown_api.h"

namespace ge {
namespace model_runner {
enum TaskInfoType {
  CCE = 0,
  TBE,
  AICPU,
  LABEL_SET,
  LABEL_SWITCH,
  LABEL_GOTO,
  EVENT_RECORD,
  EVENT_WAIT,
  FUSION_START,
  FUSION_END,
  HCCL,
  PROFILER_TRACE,
  MEMCPY_ASYNC,
  STREAM_SWITCH,
  STREAM_ACTIVE,
  // Insert new task type here
  REVSERVED = 23
};

class TaskInfo {
 public:
  virtual ~TaskInfo() {}
  uint32_t stream_id() const { return stream_id_; }
  TaskInfoType type() const { return type_; }
  std::string op_name() const { return op_name_; }
  bool dump_flag() const { return dump_flag_; }

 protected:
  TaskInfo(const std::string &op_name, uint32_t stream_id, TaskInfoType type, bool dump_flag)
      : op_name_(op_name), stream_id_(stream_id), type_(type), dump_flag_(dump_flag) {}

 private:
  std::string op_name_;
  uint32_t stream_id_;
  TaskInfoType type_;
  bool dump_flag_;
};

class CceTaskInfo : public TaskInfo {
 public:
  CceTaskInfo(const std::string &op_name, uint32_t stream_id, const cce::ccOpContext &ctx, const std::string &stub_func,
              uint32_t block_dim, const std::vector<uint8_t> &args, uint32_t args_size,
              const std::vector<uint8_t> &sm_desc, const std::vector<uint8_t> &flow_table,
              const std::vector<uint8_t> &args_offset, bool is_flowtable)
      : TaskInfo(op_name, stream_id, TaskInfoType::CCE, false),
        ctx_(ctx),
        stub_func_(stub_func),
        block_dim_(block_dim),
        args_(args),
        args_size_(args_size),
        sm_desc_(sm_desc),
        flow_table_(flow_table),
        args_offset_(args_offset),
        is_flowtable_(is_flowtable) {}
  ~CceTaskInfo() override {}

  cce::ccOpContext cc_context() const { return ctx_; }
  std::string stub_func() const { return stub_func_; }
  uint32_t block_dim() const { return block_dim_; }
  const std::vector<uint8_t> &args() const { return args_; }
  uint32_t args_size() const { return args_size_; }
  const std::vector<uint8_t> &sm_desc() const { return sm_desc_; }
  const std::vector<uint8_t> &flow_table() const { return flow_table_; }
  const std::vector<uint8_t> &args_offset() const { return args_offset_; }
  bool is_flowtable() const { return is_flowtable_; }

 private:
  cce::ccOpContext ctx_;
  std::string stub_func_;
  uint32_t block_dim_;
  std::vector<uint8_t> args_;
  uint32_t args_size_;
  std::vector<uint8_t> sm_desc_;
  std::vector<uint8_t> flow_table_;
  std::vector<uint8_t> args_offset_;
  bool is_flowtable_;
};

class TbeTaskInfo : public TaskInfo {
 public:
  TbeTaskInfo(const std::string &op_name, uint32_t stream_id, const std::string &stub_func, uint32_t block_dim,
              const std::vector<uint8_t> &args, uint32_t args_size, const std::vector<uint8_t> &sm_desc, void *binary,
              uint32_t binary_size, const std::vector<uint8_t> &meta_data, const std::vector<void *> &input_data_addrs,
              const std::vector<void *> &output_data_addrs, const std::vector<void *> &workspace_addrs, bool dump_flag)
      : TaskInfo(op_name, stream_id, TaskInfoType::TBE, dump_flag),
        stub_func_(stub_func),
        block_dim_(block_dim),
        args_(args),
        args_size_(args_size),
        sm_desc_(sm_desc),
        binary_(binary),
        binary_size_(binary_size),
        meta_data_(meta_data),
        input_data_addrs_(input_data_addrs),
        output_data_addrs_(output_data_addrs),
        workspace_addrs_(workspace_addrs) {}
  ~TbeTaskInfo() override {}

  const std::string &stub_func() const { return stub_func_; }
  uint32_t block_dim() const { return block_dim_; }
  const std::vector<uint8_t> &args() const { return args_; }
  uint32_t args_size() const { return args_size_; }
  const std::vector<uint8_t> &sm_desc() const { return sm_desc_; }
  void *binary() const { return binary_; }
  uint32_t binary_size() const { return binary_size_; }
  const std::vector<uint8_t> &meta_data() const { return meta_data_; }
  const std::vector<void *> &input_data_addrs() const { return input_data_addrs_; }
  const std::vector<void *> &output_data_addrs() const { return output_data_addrs_; }
  const std::vector<void *> &workspace_addrs() const { return workspace_addrs_; }

  void SetBinary(void *binary, uint32_t binary_size) {
    binary_ = binary;
    binary_size_ = binary_size;
  }

 private:
  std::string stub_func_;
  uint32_t block_dim_;
  std::vector<uint8_t> args_;
  uint32_t args_size_;
  std::vector<uint8_t> sm_desc_;
  void *binary_;
  uint32_t binary_size_;
  std::vector<uint8_t> meta_data_;
  std::vector<void *> input_data_addrs_;
  std::vector<void *> output_data_addrs_;
  std::vector<void *> workspace_addrs_;
};

class AicpuTaskInfo : public TaskInfo {
 public:
  AicpuTaskInfo(const std::string &op_name, uint32_t stream_id, const string &so_name, const std::string &kernel_name,
                const std::string &node_def, const std::vector<void *> &input_data_addrs,
                const std::vector<void *> &output_data_addrs, bool dump_flag)
      : TaskInfo(op_name, stream_id, TaskInfoType::AICPU, dump_flag),
        so_name_(so_name),
        kernel_name_(kernel_name),
        node_def_(node_def),
        input_data_addrs_(input_data_addrs),
        output_data_addrs_(output_data_addrs) {}
  ~AicpuTaskInfo() override {}

  const std::string &so_name() const { return so_name_; }
  const std::string &kernel_name() const { return kernel_name_; }
  const std::string &node_def() const { return node_def_; }
  const std::vector<void *> &input_data_addrs() const { return input_data_addrs_; }
  const std::vector<void *> &output_data_addrs() const { return output_data_addrs_; }

 private:
  std::string so_name_;
  std::string kernel_name_;
  std::string node_def_;
  std::vector<void *> input_data_addrs_;
  std::vector<void *> output_data_addrs_;
};

class LabelSetTaskInfo : public TaskInfo {
 public:
  LabelSetTaskInfo(const std::string &op_name, uint32_t stream_id, uint32_t label_id)
      : TaskInfo(op_name, stream_id, TaskInfoType::LABEL_SET, false), label_id_(label_id) {}
  ~LabelSetTaskInfo() override {}
  uint32_t label_id() const { return label_id_; }

 private:
  uint32_t label_id_;
};

class LabelGotoTaskInfo : public TaskInfo {
 public:
  LabelGotoTaskInfo(const std::string &op_name, uint32_t stream_id, uint32_t label_id)
      : TaskInfo(op_name, stream_id, TaskInfoType::LABEL_GOTO, false), label_id_(label_id) {}
  ~LabelGotoTaskInfo() override {}
  uint32_t label_id() const { return label_id_; }

 private:
  uint32_t label_id_;
};

class LabelSwitchTaskInfo : public TaskInfo {
 public:
  LabelSwitchTaskInfo(const std::string &op_name, uint32_t stream_id, uint32_t label_size,
                      const std::vector<uint32_t> &label_list, void *cond)
      : TaskInfo(op_name, stream_id, TaskInfoType::LABEL_SWITCH, false),
        label_size_(label_size),
        label_list_(label_list),
        cond_(cond) {}
  ~LabelSwitchTaskInfo() override {}
  uint32_t label_size() { return label_size_; };
  const std::vector<uint32_t> &label_list() { return label_list_; };
  void *cond() { return cond_; };

 private:
  uint32_t label_size_;
  std::vector<uint32_t> label_list_;
  void *cond_;
};

class EventTaskInfo : public TaskInfo {
 public:
  uint32_t event_id() const { return event_id_; }

 protected:
  EventTaskInfo(const std::string &op_name, uint32_t stream_id, TaskInfoType type, uint32_t event_id)
      : TaskInfo(op_name, stream_id, type, false), event_id_(event_id) {}
  virtual ~EventTaskInfo() override {}

  uint32_t event_id_;
};

class EventRecordTaskInfo : public EventTaskInfo {
 public:
  EventRecordTaskInfo(const std::string &op_name, uint32_t stream_id, uint32_t event_id)
      : EventTaskInfo(op_name, stream_id, TaskInfoType::EVENT_RECORD, event_id) {}
  ~EventRecordTaskInfo() override {}
};

class EventWaitTaskInfo : public EventTaskInfo {
 public:
  EventWaitTaskInfo(const std::string &op_name, uint32_t stream_id, uint32_t event_id)
      : EventTaskInfo(op_name, stream_id, TaskInfoType::EVENT_WAIT, event_id) {}
  ~EventWaitTaskInfo() override {}
};

class FusionStartTaskInfo : public TaskInfo {
 public:
  explicit FusionStartTaskInfo(const std::string &op_name, uint32_t stream_id)
      : TaskInfo(op_name, stream_id, TaskInfoType::FUSION_START, false) {}
  ~FusionStartTaskInfo() override {}
};

class FusionEndTaskInfo : public TaskInfo {
 public:
  explicit FusionEndTaskInfo(const std::string &op_name, uint32_t stream_id)
      : TaskInfo(op_name, stream_id, TaskInfoType::FUSION_END, false) {}
  ~FusionEndTaskInfo() override {}
};

class HcclTaskInfo : public TaskInfo {
 public:
  HcclTaskInfo(const std::string &op_name, uint32_t stream_id, const std::string hccl_type, void *input_data_addr,
               void *output_data_addr, void *workspace_addr, int64_t workspace_size, int64_t hccl_stream_num,
               const std::vector<uint8_t> &private_def, void *ops_kernel_store, int32_t count, int64_t root_id,
               int64_t op_type, int64_t data_type, const std::string &group,
               std::function<bool(void *, void *)> hcom_bind_model, std::function<bool(void *)> hcom_unbind_model,
               std::function<bool(std::shared_ptr<HcclTaskInfo>, void *)> hcom_distribute_task, bool dump_flag)
      : TaskInfo(op_name, stream_id, TaskInfoType::HCCL, dump_flag),
        hccl_type_(hccl_type),
        input_data_addr_(input_data_addr),
        output_data_addr_(output_data_addr),
        workspace_addr_(workspace_addr),
        workspace_size_(workspace_size),
        hccl_stream_num_(hccl_stream_num),
        private_def_(private_def),
        ops_kernel_store_(ops_kernel_store),
        count_(count),
        root_id_(root_id),
        op_type_(op_type),
        data_type_(data_type),
        group_(group),
        hcom_bind_model_(hcom_bind_model),
        hcom_unbind_model_(hcom_unbind_model),
        hcom_distribute_task_(hcom_distribute_task) {}
  ~HcclTaskInfo() override {}

  const std::string &hccl_type() const { return hccl_type_; }
  void *input_data_addr() const { return input_data_addr_; }
  void *output_data_addr() const { return output_data_addr_; }
  void *workspace_addr() const { return workspace_addr_; }
  int64_t workspace_size() const { return workspace_size_; }
  int64_t hccl_stream_num() const { return hccl_stream_num_; }
  const std::vector<uint8_t> &private_def() const { return private_def_; }
  void *ops_kernel_store() const { return ops_kernel_store_; }
  int32_t count() const { return count_; }
  int64_t root_id() const { return root_id_; }
  int64_t op_type() const { return op_type_; }
  int64_t data_type() const { return data_type_; }
  const std::string group() const { return group_; }
  std::function<bool(void *, void *)> hcom_bind_model() const { return hcom_bind_model_; }
  std::function<bool(void *)> hcom_unbind_model() const { return hcom_unbind_model_; }
  std::function<bool(std::shared_ptr<HcclTaskInfo>, void *)> hcom_distribute_task() const {
    return hcom_distribute_task_;
  }

 private:
  std::string hccl_type_;
  void *input_data_addr_;
  void *output_data_addr_;
  void *workspace_addr_;
  int64_t workspace_size_;
  int64_t hccl_stream_num_;
  std::vector<uint8_t> private_def_;
  void *ops_kernel_store_;
  int32_t count_;
  int64_t root_id_;
  int64_t op_type_;
  int64_t data_type_;
  std::string group_;
  std::function<bool(void *, void *)> hcom_bind_model_;
  std::function<bool(void *)> hcom_unbind_model_;
  std::function<bool(std::shared_ptr<HcclTaskInfo>, void *)> hcom_distribute_task_;
};

class ProfilerTraceTaskInfo : public TaskInfo {
 public:
  ProfilerTraceTaskInfo(const std::string &op_name, uint32_t stream_id, uint64_t log_id, bool notify, uint32_t flat)
      : TaskInfo(op_name, stream_id, TaskInfoType::PROFILER_TRACE, false),
        log_id_(log_id),
        notify_(notify),
        flat_(flat) {}
  ~ProfilerTraceTaskInfo() override {}

  uint64_t log_id() const { return log_id_; }
  bool notify() const { return notify_; }
  uint32_t flat() const { return flat_; }

 private:
  uint64_t log_id_;
  bool notify_;
  uint32_t flat_;
};

class MemcpyAsyncTaskInfo : public TaskInfo {
 public:
  MemcpyAsyncTaskInfo(const std::string &op_name, uint32_t stream_id, void *dst, uint64_t dst_max, void *src,
                      uint64_t count, uint32_t kind, bool dump_flag)
      : TaskInfo(op_name, stream_id, TaskInfoType::MEMCPY_ASYNC, dump_flag),
        dst_(dst),
        dst_max_(dst_max),
        src_(src),
        count_(count),
        kind_(kind) {}
  ~MemcpyAsyncTaskInfo() override {}

  void *dst() const { return dst_; }
  uint64_t dst_max() const { return dst_max_; }
  void *src() const { return src_; }
  uint64_t count() const { return count_; }
  uint32_t kind() const { return kind_; }

 private:
  void *dst_;
  uint64_t dst_max_;
  void *src_;
  uint64_t count_;
  int32_t kind_;
};

class StreamSwitchTaskInfo : public TaskInfo {
 public:
  StreamSwitchTaskInfo(const std::string &op_name, uint32_t stream_id, int64_t true_stream_id, void *input_addr,
                       void *value_addr, int64_t cond, int64_t data_type)
      : TaskInfo(op_name, stream_id, TaskInfoType::STREAM_SWITCH, false),
        true_stream_id_(true_stream_id),
        input_addr_(input_addr),
        value_addr_(value_addr),
        cond_(cond),
        data_type_(data_type) {}
  ~StreamSwitchTaskInfo() override {}

  int64_t true_stream_id() const { return true_stream_id_; }
  void *input_addr() const { return input_addr_; }
  void *value_addr() const { return value_addr_; }
  int64_t cond() const { return cond_; }
  int64_t data_type() const { return data_type_; }

 private:
  int64_t true_stream_id_;
  void *input_addr_;
  void *value_addr_;
  int64_t cond_;
  int64_t data_type_;
};

class StreamActiveTaskInfo : public TaskInfo {
 public:
  StreamActiveTaskInfo(const std::string &op_name, uint32_t stream_id, uint32_t active_stream_id)
      : TaskInfo(op_name, stream_id, TaskInfoType::STREAM_ACTIVE, false), active_stream_id_(active_stream_id) {}
  ~StreamActiveTaskInfo() override {}

  uint32_t active_stream_id() const { return active_stream_id_; }

 private:
  uint32_t active_stream_id_;
};
}  // namespace model_runner
}  // namespace ge

#endif  // INC_FRAMEWORK_GE_RUNTIME_TASK_INFO_H_
