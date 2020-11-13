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

#include "tf_adapter/kernels/geop_npu.h"

#include <chrono>
#include <cstdint>
#include <dirent.h>
#include <dlfcn.h>
#include <fstream>
#include <map>
#include <memory>
#include <mmpa/mmpa_api.h>
#include <queue>
#include <securec.h>
#include <securectype.h>
#include <thread>
#include <vector>

#include "tf_adapter/common/common.h"
#include "tf_adapter/util/ge_plugin.h"
#include "tf_adapter/util/infershape_util.h"
#include "tf_adapter/util/npu_attrs.h"
#include "tf_adapter/util/generate_report.h"
#include "tf_adapter/util/npu_ops_identifier.h"
#include "tf_adapter/util/session_manager.h"

#include "tensorflow/core/common_runtime/dma_helper.h"
#include "tensorflow/core/framework/attr_value_util.h"
#include "tensorflow/core/framework/node_def_util.h"
#include "tensorflow/core/framework/tensor_shape.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/graph/node_builder.h"
#include "tensorflow/core/lib/strings/str_util.h"

#include "framework/common/ge_inner_error_codes.h"
#include "framework/common/types.h"
#include "framework/omg/omg_inner_types.h"
#include "framework/omg/parser/model_parser.h"
#include "framework/omg/parser/parser_api.h"
#include "framework/omg/parser/parser_factory.h"
#include "framework/omg/parser/parser_inner_ctx.h"
#include "ge/ge_api.h"
#include "ge/ge_api_types.h"
#include "tdt/tdt_host_interface.h"
#include "tdt/tsd_client.h"

using namespace tdt;

namespace tensorflow {
namespace {
inline string ToString(ge::Status status) { return ::ge::StatusFactory::Instance()->GetErrDesc(status); }
Status BuildOutputTensorInfo(OpKernelContext *ctx, std::vector<ge::OutputTensorInfo> &outputs) {
  // ctx is not nullptr
  int num_outputs = ctx->num_outputs();
  LOG(INFO) << "BuildOutputTensorInfo, num_outputs:" << num_outputs;
  if (num_outputs != static_cast<int>(outputs.size())) {
    LOG(ERROR) << "[GEOP] Outputs num mismatched, need:" << num_outputs << ", while GE return:" << outputs.size();
    return errors::InvalidArgument("Outputs num mismatched, need:", num_outputs, ", while GE return:", outputs.size());
  }

  // populate outputs
  for (int i = 0; i < num_outputs; ++i) {
    ge::OutputTensorInfo &output = outputs[i];
    std::vector<int64> dims;
    std::string dim_string;
    for (int64_t dim : output.dims) {
      dims.push_back(dim);
      dim_string = dim_string + " " + std::to_string(dim);
    }
    TensorShape out_shape(dims);
    Tensor *tensor = nullptr;
    TF_RETURN_IF_ERROR(ctx->allocate_output(i, out_shape, &tensor));
    REQUIRES_NOT_NULL(tensor);
    size_t total_bytes = tensor->TotalBytes();
    void *tensor_ptr = DMAHelper::base(tensor);
    if (total_bytes != static_cast<size_t>(output.length)) {
      LOG(ERROR) << "[GEOP] Outputs len mismatched, index:" << i << ", alloc output:" << total_bytes
                 << ", while GE return:" << output.length;
      return errors::InvalidArgument("Outputs num mismatched, index:", i, ", alloc output:", total_bytes,
                                     ", while GE return:", outputs[i].length);
    }
    LOG(INFO) << "BuildOutputTensorInfo, output index:" << i << ", total_bytes:" << total_bytes
              << ", shape:" << dim_string << ", tensor_ptr:" << reinterpret_cast<uintptr_t>(tensor_ptr) << ", output"
              << reinterpret_cast<uintptr_t>(output.data.get());
    if (total_bytes == 0) {
      LOG(INFO) << "BuildOutputTensorInfo, output index:" << i << ", total_bytes is 0, continue do next. ";
      continue;
    }

    if (output.data != nullptr) {
      void *dst_ptr = tensor_ptr;
      void *src_ptr = static_cast<void *>(output.data.get());
      size_t left_size = total_bytes;
      while (left_size > SECUREC_MEM_MAX_LEN) {
        auto err = memcpy_s(dst_ptr, SECUREC_MEM_MAX_LEN, src_ptr, SECUREC_MEM_MAX_LEN);
        if (err != EOK) {
          LOG(ERROR) << "[GEOP] Outputs mem copy failed, index:" << i << ", errret:" << err
                     << ", dst_ptr:" << (uintptr_t) dst_ptr << ", dst_size:" << SECUREC_MEM_MAX_LEN
                     << ", src_ptr:" << (uintptr_t) src_ptr << ", src_size:" << SECUREC_MEM_MAX_LEN;
          return errors::InvalidArgument("Outputs mem copy failed, index:", i);
        }
        left_size -= SECUREC_MEM_MAX_LEN;

        dst_ptr = static_cast<void *>(static_cast<char *>(dst_ptr) + SECUREC_MEM_MAX_LEN);
        src_ptr = static_cast<void *>(static_cast<char *>(src_ptr) + SECUREC_MEM_MAX_LEN);
      }
      REQUIRES_NOT_NULL(dst_ptr);
      REQUIRES_NOT_NULL(src_ptr);
      auto err = memcpy_s(dst_ptr, left_size, src_ptr, left_size);
      if (err != EOK) {
        LOG(ERROR) << "[GEOP] Outputs mem copy failed, index:" << i << ", errret:" << err
                   << ", dst_ptr:" << (uintptr_t)dst_ptr << ", dst_size:" << left_size
                   << ", src_ptr:" << (uintptr_t)src_ptr << ", src_size:" << left_size;
        return errors::InvalidArgument("Outputs mem copy failed, index:", i);
      }
    }
  }
  return Status::OK();
}

bool CmpValue(const std::pair<std::vector<string>, uint32_t> &p1, const std::pair<std::vector<string>, uint32_t> &p2) {
  return p1.second < p2.second;
}

bool CmpVecValue(Node *node1, Node *node2) {
  if (node1 == nullptr || node2 == nullptr) {
    LOG(ERROR) << "node1 or node2 is nullptr.";
    return false;
  }
  return node1->name() < node2->name();
}
}  // namespace

std::string CurrentTimeInStr() {
  std::time_t now = std::time(nullptr);
  std::tm *ptm = std::localtime(&now);
  if (nullptr == ptm) { return ""; }

  const int time_buffer_len = 32;
  char buffer[time_buffer_len] = {0};
  // format: 20171122042550
  std::strftime(buffer, time_buffer_len, "%Y%m%d%H%M%S", ptm);
  return std::string(buffer);
}

static const int64 kMicrosToMillis = 1000;
const int kInvalidGraphId = 0;
const int kMaxCacheNum = 10;
const int kFatalSleepTime = 3000;

GeOp::GeOp(OpKernelConstruction *ctx)
    : AsyncOpKernel(ctx), init_flag_(false), build_flag_(false), add_graph_flag_(false),
      sess_init_flag_(false), compute_graph_empty_(false), data_format_(""), graph_id_(0),
      is_initialized_graph_(false), need_iteration_(false), tf_session_(""), ge_session_(nullptr),
      job_type_(""), is_host_graph_(false), is_train_graph_(false), handle_(nullptr), tuning_api_(nullptr) {
  Initialize(ctx);
}

GeOp::~GeOp() { Finalize(); }

void GeOp::Initialize(OpKernelConstruction *ctx) {
  int64 startTime = InferShapeUtil::GetCurrentTimestap();
  LOG(INFO) << "[GEOP] Begin GeOp initialize.";
  if (init_flag_) {
    LOG(WARNING) << "[GEOP] GEOP already Initialize.";
    return;
  }

  CHECK_NOT_NULL(ctx);
  const NameAttrList *func = nullptr;
  OP_REQUIRES_OK(ctx, ctx->GetAttr("function", &func));
  function_ = *func;
  string data_format;
  OP_REQUIRES_OK(ctx, ctx->GetAttr("data_format", &data_format));
  this->data_format_ = data_format;

  Status s = ctx->GetAttr("_session", &tf_session_);
  if (s.ok()) { LOG(INFO) << "[GEOP] get session info from attr, tf session: " << tf_session_; }

  // global environment Initialize, invoke once for each process
  string sess_config = "";
  OP_REQUIRES_OK(ctx, ctx->GetAttr("_NpuOptimizer", &sess_config));
  std::map<std::string, std::string> pass_options = NpuAttrs::GetPassOptions(ctx);
  iteration_per_loop_ = std::atoi(pass_options["iterations_per_loop"].c_str());
  job_type_ = pass_options["job"];
  if (GePlugin::GetInstance()->IsGlobal()) {
    LOG(INFO) << "[GEOP] GePlugin global, skip GePlugin init";
    std::map<std::string, std::string> global_init_options = GePlugin::GetInstance()->GetInitOptions();
    GetMsTuneConfig(global_init_options);
  } else {
    std::map<std::string, std::string> init_options = NpuAttrs::GetInitOptions(ctx);
    GetMsTuneConfig(init_options);
    GePlugin::GetInstance()->Init(init_options_);
    LOG(INFO) << "[GEOP] GePlugin init success";
  }

  if (!mstune_mode_.empty() && !work_path_.empty()) {
    handle_ = mmDlopen("libmstune_train.so", MMPA_RTLD_NOW);
    OP_REQUIRES(ctx, handle_ != nullptr, errors::InvalidArgument("libmstune_train.so dlopen failed, ", mmDlerror()));
    tuning_api_ = (MsTuningFunc)mmDlsym(handle_, const_cast<char*>("MsTrainTuning"));
    OP_REQUIRES(ctx, tuning_api_ != nullptr, errors::InvalidArgument("dlsym MsGradientTuning API failed, ", mmDlerror()));
  }

  sess_options_ = NpuAttrs::GetSessOptions(ctx);

  init_flag_ = true;
  int64 endTime = InferShapeUtil::GetCurrentTimestap();
  LOG(INFO) << "[GEOP] GeOp Initialize success, cost:"
            << " [" << ((endTime - startTime) / kMicrosToMillis) << " ms]";
  return;
}

void GeOp::Finalize() {
  {
    LOG(INFO) << "[GEOP] GeOp start to finalize, tf session: " << tf_session_ << ", graph_id_: " << graph_id_;
    // global environment finalize, invoke once for each process
    {
      mutex_lock lock{mu_};
      uint32_t graph_id = -1;
      bool ret = DecrementGraphIdCount(tf_session_, graph_id);
      if (!ret || graph_id < kInvalidGraphId) {
        LOG(ERROR) << "tf session " << tf_session_ << " sub graph id failed.";
        return;
      }
      if (graph_id == kInvalidGraphId) {
        SessionManager::GetInstance().DestroyGeSession(tf_session_);
        ClearGraphIdCount(tf_session_);
      }

      if (!SessionManager::GetInstance().IsGeSessionExist()) {
        if (!GePlugin::GetInstance()->IsGlobal()) {
          GePlugin::GetInstance()->Finalize();
          LOG(INFO) << "[GEOP] GePlugin Finalize success";
        } else {
          LOG(INFO) << "[GEOP] GePlugin global, skip GePlugin Finalize";
        }
        if (!GenerateReport::GetInstance()->SaveUnsupportedInfo().ok()) {
          LOG(WARNING) << "[GEOP] Save check report failed.";
        }
        if (handle_ != nullptr) {
          (void)mmDlclose(handle_);
        }
      }
    }
  }
  init_flag_ = false;
  LOG(INFO) << "[GEOP] GeOp Finalize success, tf session: " << tf_session_ << ", graph_id_: " << graph_id_;
  return;
}

void GeOp::GetMsTuneConfig(std::map<std::string, std::string> init_options) {
  mstune_mode_ = init_options["ge.buildMode"];
  work_path_ = init_options["ge.tuningPath"];
  auto_tune_mode_ = init_options[ge::AUTO_TUNE_MODE];
  if (!mstune_mode_.empty() && !work_path_.empty()) {
    init_options[ge::AUTO_TUNE_MODE] = "";
  }
  init_options_ = init_options;
}

int GeOp::InitRebuildFlag(uint32_t cache_graph_id) {
  if (!build_flag_) {
    LOG(INFO) << "[GEOP] tf session " << tf_session_ << ", graph id: " << cache_graph_id
              << " does not build yet, no need to check rebuild";
    return 0;
  }
  if (ge_session_ == nullptr) {
    LOG(ERROR) << "[GEOP] GE session is nullptr";
    return -1;
  }
  if (!ge_session_->IsGraphNeedRebuild(cache_graph_id)) {
    LOG(INFO) << "[GEOP] tf session " << tf_session_ << ", graph id: " << cache_graph_id << " no need to rebuild";
    return 0;
  }

  LOG(INFO) << "[GEOP] The graph need rebuild, graph id " << cache_graph_id;

  // The graph need to rebuild, remove it from GE first.
  LOG(INFO) << "[GEOP] tf session: " << tf_session_ << ", graph id: " << cache_graph_id;
  auto ret = ge_session_->RemoveGraph(cache_graph_id);
  if (ret != ge::SUCCESS) {
    LOG(ERROR) << "[GEOP] Failed to remove graph " << cache_graph_id << " from ge, error code " << ret;
    return -1;
  }

  build_flag_ = false;
  compute_graph_empty_ = false;
  return 0;
}

bool GeOp::IncrementGraphIdCount(std::string &tf_session, uint32_t &graph_id) {
  if (tf_session_.empty()) {
    LOG(ERROR) << "[GEOP] Add graph id failed, tf session is empty.";
    return false;
  }
  auto it = session_and_graph_id_map_.find(tf_session_);
  if (it != session_and_graph_id_map_.end()) {
    it->second = it->second + kMaxCacheNum;
    graph_id = it->second;
    return true;
  }
  graph_id = 1;
  session_and_graph_id_map_.insert(std::make_pair(tf_session_, graph_id));
  return true;
}

bool GeOp::DecrementGraphIdCount(std::string &tf_session, uint32_t &graph_id) {
  if (tf_session_.empty()) {
    LOG(ERROR) << "[GEOP] Sub graph id failed, tf session is empty.";
    return false;
  }

  auto it = session_and_graph_id_map_.find(tf_session_);
  if (it != session_and_graph_id_map_.end()) {
    if (it->second == 1) {
      it->second = it->second - 1;
      graph_id = it->second;
      return true;
    }
    it->second = it->second - kMaxCacheNum;
    graph_id = it->second;
    return true;
  }
  LOG(ERROR) << "[GEOP] Sub graph id failed, can not find tf session " << tf_session;
  return false;
}

void GeOp::ClearGraphIdCount(std::string &tf_session) {
  auto it = session_and_graph_id_map_.find(tf_session_);
  if (it != session_and_graph_id_map_.end()) { session_and_graph_id_map_.erase(it); }
}

void GeOp::GetExecGraphId(OpKernelContext *ctx, uint32_t &cache_graph_id,
                          std::vector<std::string> input_shapes) {
  size_t num = cache_graphs_.size();
  if (cache_graphs_.find(input_shapes) != cache_graphs_.end()) {
    for (auto &graph_count : graph_counts_) {
      if (graph_count.first == input_shapes) {
        graph_count.second += 1;
        break;
      }
    }
    cache_graph_id = cache_graphs_[input_shapes];
    build_flag_ = true;
  } else {
    if (num >= kMaxCacheNum) {
      LOG(INFO) << "[GEOP] the cache vector size is : " << num << " , begin erase the least uesed";
      std::sort(graph_counts_.begin(), graph_counts_.end(), CmpValue);
      uint32_t erased_graph_id = cache_graphs_[graph_counts_[0].first];
      cache_graphs_.erase(graph_counts_[0].first);
      graph_counts_.erase(graph_counts_.begin());
      ge::Status status = ge_session_->RemoveGraph(erased_graph_id);
      if (status != ge::SUCCESS) { LOG(WARNING) << "[GEOP] GE Remove Graph failed, ret : " << ToString(status); }
      cache_graph_id = erased_graph_id;
    } else {
      cache_graph_id = graph_id_ + num;
    }
    build_flag_ = false;
    compute_graph_empty_ = false;
  }
}

void GeOp::ComputeAsync(OpKernelContext *ctx, DoneCallback done) {
  // ctx is not nullptr
  OP_REQUIRES_ASYNC(ctx, init_flag_, errors::InvalidArgument("GeOp not Initialize success."), done);
  // ge ge session
  if (!sess_init_flag_) {
    if (job_type_ != "localhost") {  // in ps mode : ctx->session_handle() is empty
      tf_session_ = "ps_worker_session";
      LOG(INFO) << "[GEOP] get tf session " << tf_session_ << " when in ps mode.";
    }

    if (tf_session_.empty()) {
      tf_session_ = ctx->session_handle();
      LOG(INFO) << "[GEOP] get tf session " << tf_session_ << " from session handle.";
    }

    {
      mutex_lock lock{mu_};
      bool res = IncrementGraphIdCount(tf_session_, graph_id_);
      if (!res || graph_id_ < kInvalidGraphId) {
        OP_REQUIRES_ASYNC(ctx, false, errors::Unavailable("Get ge session failed."), done);
        return;
      }

      LOG(INFO) << "[GEOP] Node name: " << ctx->op_kernel().name() << " , tf session: " << tf_session_;

      res = SessionManager::GetInstance().GetOrCreateGeSession(tf_session_, ge_session_, sess_options_);
      if (!res || tf_session_.empty() || ge_session_ == nullptr) {
        OP_REQUIRES_ASYNC(ctx, false, errors::Unavailable("Get ge session failed."), done);
        return;
      }
      LOG(INFO) << "[GEOP] tf session: " << tf_session_ << " get ge session success.";
      sess_init_flag_ = true;
    }
  }
  string geop_name = ctx->op_kernel().name();
  uint32_t num_inputs = static_cast<uint32_t>(ctx->num_inputs());
  LOG(INFO) << "[GEOP] Begin GeOp::ComputeAsync"
            << ", kernel_name:" << geop_name << ", num_inputs:" << num_inputs << ", num_outputs:" << ctx->num_outputs();
  int64 startTime = InferShapeUtil::GetCurrentTimestap();
  std::vector<std::string> input_shapes;
  for (int i = 0; i < ctx->num_inputs(); i++) {
    input_shapes.push_back(ctx->input(i).shape().DebugString());
  }

  // if input shapes changed, cache graphs
  uint32_t cache_graph_id;
  bool is_set_dynamic_config = !sess_options_["ge.inputShape"].empty() && !sess_options_["ge.dynamicDims"].empty();
  bool is_tuning = !mstune_mode_.empty() && !work_path_.empty();
  if (is_set_dynamic_config && is_tuning) {
    LOG(FATAL) << "dynamic input config can not use with mstuning.";
  } else if (is_set_dynamic_config && !is_tuning) {
    cache_graph_id = graph_id_;
    if (InitRebuildFlag(cache_graph_id) != 0) {
      OP_REQUIRES_ASYNC(ctx, false, errors::Internal("Failed to check rebuild flag"), done);
      return;
    }
  } else if (!is_set_dynamic_config && is_tuning) {
    cache_graph_id = graph_id_;
  } else {
    // if set dynamic input config, do not cache graphs.
    GetExecGraphId(ctx, cache_graph_id, input_shapes);
    if (InitRebuildFlag(cache_graph_id) != 0) {
      OP_REQUIRES_ASYNC(ctx, false, errors::Internal("Failed to check rebuild flag"), done);
      return;
    }
  }

  if (!build_flag_) {
    // Get Graph
    OP_REQUIRES_ASYNC(ctx, ctx->function_library() != nullptr, errors::Internal("function library is nullptr"), done);
    const FunctionLibraryDefinition *flib_def = ctx->function_library()->GetFunctionLibraryDefinition();
    OP_REQUIRES_ASYNC(ctx, flib_def != nullptr, errors::Internal("flib_def is nullptr"), done);
    const FunctionDef *fdef = flib_def->Find(function_.name());
    OP_REQUIRES_ASYNC(ctx, fdef != nullptr, errors::Internal("fdef is nullptr"), done);
    std::shared_ptr<Graph> graph = std::make_shared<Graph>(OpRegistry::Global());
    OP_REQUIRES_ASYNC(ctx, graph != nullptr, errors::Internal("create tensorflow graph failed"), done);

    std::vector<Tensor> input_vec;
    for (uint32_t i = 0; i < num_inputs; i++) {
      Tensor input(ctx->input(i));
      input_vec.push_back(input);
    }

    // Build GraphDef from FunctionDef
    GraphDef ori_graph_def;
    BuildGraphDef(ctx, done, *flib_def, *fdef, input_vec, ori_graph_def, is_initialized_graph_);
    if (ctx->status() != Status::OK()) { return; }

    /* if graph is init verify graph, return */
    if (this->is_initialized_graph_ == true) {
      Tensor initialized_tensor(ctx->expected_output_dtype(0), TensorShape({0}));
      ctx->set_output(0, initialized_tensor);
      done();
      return;
    }

    char *need_print = getenv("PRINT_MODEL");
    if (need_print != nullptr && strcmp("1", need_print) == 0) {
      string tmpmodel_path = "TF_";
      string tmodel_path = tmpmodel_path + geop_name.c_str() + ".pbtxt";
      Status status_out = WriteTextProto(Env::Default(), tmodel_path, ori_graph_def);
    }
    int64 endTime = InferShapeUtil::GetCurrentTimestap();
    LOG(INFO) << "[GEOP] In GEOP computeAsync, kernel_name:" << geop_name << " ,TFadapter cost time: ["
              << ((endTime - startTime) / kMicrosToMillis) << " ms]";
    LOG(INFO) << "[GEOP] TFadpter process graph success, GE parser begin, kernel_name:" << geop_name
              << " ,tf session: " << tf_session_ << " ,graph id :" << cache_graph_id;
    // parser,  tensorflow graph to ge graph
    std::shared_ptr<domi::ModelParser> model_parser =
        domi::ModelParserFactory::Instance()->CreateModelParser(domi::FrameworkType::TENSORFLOW);
    OP_REQUIRES_ASYNC(ctx, model_parser != nullptr, errors::Unavailable("create model parser ret failed."), done);
    ge::ComputeGraphPtr compute_graph = nullptr;
    try {
      compute_graph = std::make_shared<ge::ComputeGraph>("ge_default_" + CurrentTimeInStr());
    } catch (...) { OP_REQUIRES_ASYNC(ctx, false, errors::Internal("make shared failed"), done); }

    OP_REQUIRES_ASYNC(ctx, compute_graph != nullptr, errors::InvalidArgument("create ComputeGraph failed"), done);

    auto build_sub_graph = [this, flib_def](const google::protobuf::Message *root_proto,
                                            const std::string &graph) -> std::unique_ptr<google::protobuf::Message> {
      // const tensorflow::GraphDef *graph_def_in = reinterpret_cast<const tensorflow::GraphDef *>(root_proto);
      LOG(INFO) << "[GEOP] build_sub_graph enter, sub graph name is " << graph;
      const FunctionDef *func_def = flib_def->Find(graph);
      if (func_def == nullptr) {
        LOG(ERROR) << "[GEOP] Sub graph not found in library, sub graph name is " << graph;
        return nullptr;
      }
      // get infershape
      Graph subgraph(flib_def);
      Status status = InferShapeUtil::GetSubGraphFromFunctionDef(*flib_def, *func_def, &subgraph);
      if (status != Status::OK()) {
        LOG(ERROR) << "[GEOP] Get subgraph from functiondef fail.";
        return nullptr;
      }
      LOG(INFO) << "[GEOP] Get subgraph from functiondef success.";

      bool is_initialize = false;
      for (Node *node : subgraph.nodes()) {
        AddNodeAttrs(node, is_initialize);

        // Add Input&Output Desc into NodeDef
        if (GenerateDesc(node) != Status::OK()) {
          LOG(WARNING) << "[GEOP] name: " << node->name() << " op:" << node->type_string()
                       << " Generate desc failed in subgraph.";
        }
      }

      unique_ptr<GraphDef> sub_graph_def(new (std::nothrow) GraphDef());
      if (sub_graph_def == nullptr) {
        LOG(ERROR) << "[GEOP] Malloc memory for subgraph def fail.";
        return nullptr;
      }
      subgraph.ToGraphDef(sub_graph_def.get());

      unique_ptr<google::protobuf::Message> graph_def_out(std::move(sub_graph_def));

      char *need_print = getenv("PRINT_MODEL");
      if (need_print != nullptr && strcmp("1", need_print) == 0) {
        string tmpmodel_path = "TF_Subgraph_";
        string tmodel_path = tmpmodel_path + graph.c_str() + ".pbtxt";
        Status status_out = WriteTextProto(Env::Default(), tmodel_path, *graph_def_out.get());
      }
      LOG(INFO) << "[GEOP] build_sub_graph exit, sub graph name is " << graph;
      return graph_def_out;
    };

    ge::Status status = model_parser->ParseProtoWithSubgraph(
        reinterpret_cast<google::protobuf::Message *>(&ori_graph_def), build_sub_graph, compute_graph);
    OP_REQUIRES_ASYNC(ctx, status == ge::SUCCESS, errors::Internal("graph parse failed, domi_ret : ", ToString(status)),
                      done);
    domi::GetContext().format = ge::GetParserContext().format;

    LOG(INFO) << "[GEOP] Tensorflow graph parse to ge graph success, kernel_name:" << geop_name
              << " ,tf session: " << tf_session_ << " ,graph id: " << cache_graph_id;

    size_t nodes = compute_graph->GetAllNodesSize();
    if (nodes == 0) {
      build_flag_ = true;
      compute_graph_empty_ = true;
      int64 endTime = InferShapeUtil::GetCurrentTimestap();
      LOG(INFO) << "[GEOP] End GeOp::ComputeAsync, compute_graph is empty, kernel_name:" << geop_name
                << ", ret_status:" << ToString(ge::SUCCESS) << " , tf session: " << tf_session_
                << " ,graph id: " << cache_graph_id << " [" << ((endTime - startTime) / kMicrosToMillis) << " ms]";
      done();
      return;
    }

    // convert to ge::graph
    ge::Graph ge_graph = ge::GraphUtils::CreateGraphFromComputeGraph(compute_graph);
    if (iteration_per_loop_ > 1) {
      ge_graph.SetNeedIteration(this->need_iteration_);
    }

    if (is_host_graph_) {
      LOG(INFO) << "[GEOP] set graph option.";
      graph_options_["ge.exec.placement"] = "HOST";
    }
    if (is_tuning) {
      if (!is_train_graph_) {
        LOG(INFO) << "[GEOP] in tune mode, nontraining graphs should be cache.";
        OP_REQUIRES_ASYNC(ctx, SessionManager::GetInstance().CacheGeGraphs(ge_session_, ge_graph),
          errors::Internal("[GEOP] cache ge session failed."), done);
        build_flag_ = true;
        BuildOutTensorInfo(ctx);
        done();
        return;
      } else {
        LOG(INFO) << "[GEOP] in tune mode, training graph handled by tools.";
        uint32_t device_id = 0;
        OP_REQUIRES_OK_ASYNC(ctx, GetEnvDeviceID(device_id), done);
        LOG(INFO) << "[GEOP] in tuning func, mstune_mode_:" << mstune_mode_
                  << " auto_tune_mode_:" << auto_tune_mode_;
        std::map<string, string> tune_options = {{"work_path", work_path_},
                                                 {"job_type", mstune_mode_},
                                                 {"devices", std::to_string(device_id)},
                                                 {"auto_tune_mode", auto_tune_mode_}};
        std::map<std::string, std::map<std::string, std::string>> options;
        options["mstune"] = tune_options;
        options["initialize"] = init_options_;
        options["session"] = sess_options_;
        std::vector<ge::Graph> ge_graphs;
        OP_REQUIRES_ASYNC(ctx, SessionManager::GetInstance().GetGeGraphs(ge_session_, ge_graphs),
          errors::Internal("[GEOP] ge ge session nontraining graphs failed."), done);
        MsTuneStatus tune_ret = (*tuning_api_)(ge_graph, ge_graphs, ge_session_, options);
        OP_REQUIRES_ASYNC(ctx, tune_ret == MSTUNE_SUCCESS, errors::Internal("[GEOP] exec msTuning func failed."), done);
        LOG(INFO) << "[GEOP] msTuning success.";
        build_flag_ = true;
        BuildOutTensorInfo(ctx);
        done();
        return;
      }
    }

    // call ge session addGraph api
    status = ge_session_->AddGraph(cache_graph_id, ge_graph, graph_options_);
    if (status != ge::SUCCESS) {
      std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
      LOG(FATAL) << "[GEOP] call ge session add graph failed, kernel: " << geop_name << " ,tf session: " << tf_session_
                 << ", graph id: " << cache_graph_id;
      OP_REQUIRES_ASYNC(ctx, status == ge::SUCCESS,
                        errors::Unavailable("[GEOP] GE session add graph failed, domi_ret : ", ToString(status)), done);
    } else {
      add_graph_flag_ = true;
      LOG(INFO) << "[GEOP] Add graph to ge session success, kernel_name:" << geop_name
                << " ,tf session: " << tf_session_ << " ,graph id:" << cache_graph_id;
    }
    build_flag_ = true;
    if (!is_set_dynamic_config) {
      cache_graphs_.insert(std::make_pair(input_shapes, cache_graph_id));
      graph_counts_.push_back(std::make_pair(input_shapes, 1));
    }
  } else {
    if (compute_graph_empty_) {
      int64 endTime = InferShapeUtil::GetCurrentTimestap();
      LOG(INFO) << "[GEOP] End GeOp::ComputeAsync, compute_graph is empty, kernel_name:" << geop_name
                << ", ret_status:" << ToString(ge::SUCCESS) << " , tf session: " << tf_session_
                << " ,graph id: " << cache_graph_id << " [" << ((endTime - startTime) / kMicrosToMillis) << " ms]";
      done();
      return;
    }
  }

  if (is_tuning) {
    LOG(INFO) << "in mstune mode, graph only execute once, The remaining steps return directly.";
    BuildOutTensorInfo(ctx);
    done();
    return;
  }

  int64 run_start_time = InferShapeUtil::GetCurrentTimestap();
  auto callback = [done, ctx, run_start_time](ge::Status ge_status, std::vector<ge::OutputTensorInfo> &outputs) {
    if (ge_status == ge::SUCCESS) {
      if (BuildOutputTensorInfo(ctx, outputs) != Status::OK()) {
        LOG(FATAL) << ctx->op_kernel().name() << " GEOP::DoRunAsync get output failed.";
      }
    } else if (ge_status == ge::END_OF_SEQUENCE) {
      ctx->SetStatus(errors::OutOfRange("End of sequence"));
      LOG(ERROR) << "[GEOP] Out of range: End of sequence.";
    } else if (ge_status != ge::SUCCESS) {
      tensorflow::Status tfStatus = errors::Unavailable(ToString(ge_status));
      ctx->CtxFailureWithWarning(tfStatus);
      std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
      LOG(FATAL) << ctx->op_kernel().name() << "GEOP::::DoRunAsync Failed";
    }
    int64 run_end_time = InferShapeUtil::GetCurrentTimestap();
    LOG(INFO) << "[GEOP] RunGraphAsync callback, status:" << ge_status << ", kernel_name:" << ctx->op_kernel().name()
              << "[ " << (run_end_time - run_start_time) << "us]";
    done();
  };
  std::vector<ge::InputTensorInfo> inputs;
  OP_REQUIRES_OK_ASYNC(ctx, (BuildInputTensorInfo(ctx, inputs)), done);

  LOG(INFO) << "[GEOP] Call ge session RunGraphAsync, kernel_name:" << geop_name << " ,tf session: " << tf_session_
            << " ,graph id: " << cache_graph_id;
  // call ge session runGraphAsync api
  ge::Status status = ge_session_->RunGraphAsync(cache_graph_id, inputs, callback);
  if (status != ge::SUCCESS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(kFatalSleepTime));
    LOG(FATAL) << "[GEOP] call ge session RunGraphAsync Failed, kernel:" << geop_name << " ,tf session: " << tf_session_
               << " ,graph id: " << cache_graph_id;
  }
  OP_REQUIRES_ASYNC(ctx, status == ge::SUCCESS,
                    errors::Unavailable("ge session run graph failed, ret_status:", ToString(status)), done);

  int64 endTime = InferShapeUtil::GetCurrentTimestap();
  LOG(INFO) << "[GEOP] End GeOp::ComputeAsync, kernel_name:" << geop_name << ", ret_status:" << ToString(status)
            << " ,tf session: " << tf_session_ << " ,graph id: " << cache_graph_id << " ["
            << ((endTime - startTime) / kMicrosToMillis) << " ms]";
  return;
}

void GeOp::AddNodeAttrs(Node *node, bool &is_initialize) {
  // Add dp custom kernel label
  if (node->type_string() == "IteratorGetNext") { node->AddAttr("_kernel", "dp"); }
  if (node->type_string() == "Assert" || node->type_string() == "Print" || node->type_string() == "PrintV2") {
    node->AddAttr("_kernel", "extend");
  }
  NodeDef &node_def = const_cast<NodeDef &>(node->def());
  if (node_def.op() == "Where") { is_initialize = InferShapeUtil::IsInitializedGraph(node); }
  if (node->name() == "IterationOp") {
    this->need_iteration_ = true;
    LOG(INFO) << "subgraph  has iteration op.";
  }
  if (node->name() == "var_in_host") {
    is_host_graph_ = true;
    LOG(INFO) << "[GeOp] variable subgraph is initialized in host.";
  }
  if (node->name().find("_Allreduce") != string::npos) {
    is_train_graph_ = true;
  }
  // clear device info && attr
  node_def.set_device("");
  if (node_def.op() == "Const") {
    node_def.mutable_attr()->erase("data_format");
    node_def.mutable_attr()->erase("cce_format");
    node_def.mutable_attr()->erase("output_type");
  }
}

// Build GraphDef from FunctionDef.
void GeOp::BuildGraphDef(OpKernelContext *ctx, DoneCallback done, const FunctionLibraryDefinition &flib_def,
                         const FunctionDef &func_def, const std::vector<Tensor> &input_vec, GraphDef &graph_def,
                         bool &is_initialize) {
  // get infershape
  Graph graph(OpRegistry::Global());
  OP_REQUIRES_OK_ASYNC(ctx, InferShapeUtil::InferShape(input_vec, &flib_def, &func_def, &graph), done);

  bool is_set_dynamic_config = !sess_options_["ge.inputShape"].empty() && !sess_options_["ge.dynamicDims"].empty() &&
                               !sess_options_["ge.dynamicNodeType"].empty();
  if (is_set_dynamic_config) { BuildShapeNodeAndCacheArgNodes(graph); }

  bool is_tuning = !mstune_mode_.empty() && !work_path_.empty();
  for (Node *node : graph.nodes()) {
    AddNodeAttrs(node, is_initialize);
    // Add Input&Output Desc into NodeDef
    OP_REQUIRES_OK_ASYNC(ctx, this->GenerateDesc(node), done);
    if (is_tuning) {
      // output handle
      NodeDef &node_def = const_cast<NodeDef &>(node->def());
      if (node->type_string() == "_Retval") {
        int index = node_def.attr().at("index").i();
        // format: AttrValue.list(ListValue).func(repeated NameAttrList)
        NameAttrList desc_attr = node_def.attr().at(INPUT_DESC).list().func(0);

        std::vector<int64> dims;
        int dim_num = desc_attr.attr().at(SERIALIZE_SHAPE).list().i_size();
        for (int t = 0; t < dim_num; t++) {
          int64 dim_i = (int64_t) desc_attr.attr().at(SERIALIZE_SHAPE).list().i(t);
          if (dim_i < 0) { dim_i = 1; }
          dims.push_back(dim_i);
        }

        TensorShape out_shape(dims);
        outputs_shape_.insert(std::map<int, TensorShape>::value_type(index, out_shape));
      }
    }
  }
  // set input_shape to dynamic nodes shape desc
  if (is_set_dynamic_config) { ChangeInputsShapeDesc(ctx, done); }
  graph.ToGraphDef(&graph_def);
}

void GeOp::BuildShapeNodeAndCacheArgNodes(Graph &graph) {
  std::string dynamic_node_type = sess_options_["ge.dynamicNodeType"];
  for (Node *node : graph.nodes()) {
    // add shape node to get getnext node real shape
    if (dynamic_node_type == "0" && node->type_string() == "IteratorGetNext") {
      dynamic_shape_nodes_.emplace_back(node);
      int i = 0;
      for (auto out_edge : node->out_edges()) {
        if (!out_edge->IsControlEdge()) {
          std::string shape_name = "getnext_shape_" + std::to_string(i);
          Node *shape_node = nullptr;
          TF_CHECK_OK(NodeBuilder(shape_name, "Shape")
                      .Input(node, out_edge->src_output())
                      .Device(node->def().device())
                      .Finalize(&graph, &shape_node));
          std::string identity_name = "shape_identity_" + std::to_string(i);
          Node *identity_node = nullptr;
          TF_CHECK_OK(NodeBuilder(identity_name, "Identity")
                      .Input(shape_node, 0)
                      .Device(shape_node->def().device())
                      .Finalize(&graph, &identity_node));
        }
        i++;
      }
    }
    // count data args and getnext args for dynamic dims
    if (node->type_string() == "_Arg") {
      if (node->name().find("IteratorGetNext_") != std::string::npos) {
        if (dynamic_node_type == "0") { dynamic_shape_nodes_.emplace_back(node); }
      } else {
        if (dynamic_node_type == "1") { dynamic_shape_nodes_.emplace_back(node); }
      }
    }
  }
  // sort dynamic nodes to match input_shapes
  std::sort(dynamic_shape_nodes_.begin(), dynamic_shape_nodes_.end(), CmpVecValue);
}

void GeOp::ChangeInputsShapeDesc(OpKernelContext *ctx, DoneCallback done) {
  std::vector<std::string> result;
  std::string input_shapes = sess_options_["ge.inputShape"];
  Split(input_shapes, result, ";"); //e.g. result:["data:2,3", "data1:3,4"]

  if (dynamic_shape_nodes_.size() == 1 && dynamic_shape_nodes_[0]->type_string() == "IteratorGetNext") {
    LOG(INFO) << "[GEOP] change " << dynamic_shape_nodes_[0]->name() << " shape desc.";
    NodeDef &node_def = const_cast<NodeDef &>(dynamic_shape_nodes_[0]->def());
    AttrValue &output_tensor_descs = (*node_def.mutable_attr())[OUTPUT_DESC];
    for (size_t i = 0; i < dynamic_shape_nodes_[0]->num_outputs(); ++i) {
      AttrValue attr_shape_value;
      SetShapesToOutputDesc(result, i, attr_shape_value);
      (*output_tensor_descs.mutable_list()->mutable_func(i)->mutable_attr())[SERIALIZE_SHAPE] = attr_shape_value;
    }
  } else {
    if (!dynamic_shape_nodes_.empty()) {
      OP_REQUIRES_ASYNC(ctx, dynamic_shape_nodes_.size() == result.size(),
                        errors::Internal("input_shape is not match inputs num in graph"), done);
    }
    for (size_t i = 0; i < dynamic_shape_nodes_.size(); ++i) {
      LOG(INFO) << "[GEOP] change " << dynamic_shape_nodes_[i]->name() << " shape desc.";
      NodeDef &node_def = const_cast<NodeDef &>(dynamic_shape_nodes_[i]->def());
      AttrValue &output_tensor_descs = (*node_def.mutable_attr())[OUTPUT_DESC];
      AttrValue attr_shape_value;
      SetShapesToOutputDesc(result, i, attr_shape_value);
      (*output_tensor_descs.mutable_list()->mutable_func(0)->mutable_attr())[SERIALIZE_SHAPE] = attr_shape_value;
    }
  }
  LOG(INFO) << "[GEOP] change input shapes desc success.";
}

void GeOp::SetShapesToOutputDesc(const std::vector<std::string> &input_shapes,
                                 const int &index, AttrValue &attr_shape_value) {
  if (input_shapes.empty()) {
    LOG(ERROR) << "[GEOP] input_shapes is empty.";
    return;
  }
  if (index < 0) {
    LOG(ERROR) << "[GEOP] index must more than 0.";
    return;
  }
  LOG(INFO) << "[GEOP] get input: " << index << " input shape is: " << input_shapes[index];
  std::vector<std::string> shape;
  Split(input_shapes[index], shape, ":"); // e.g. shape:["data", "2,3,4"]
  if (shape.empty() || shape.size() != 2) {
    LOG(ERROR) << "[GEOP] shape is empty or shape size is not 2.";
    return;
  }
  if (shape[1] == "0") {
    // scale node has no shape.
    return;
  }
  std::vector<std::string> dims;
  Split(shape[1], dims, ","); // e.g. dims:["2", "3", "4"]
  for (auto dim : dims) {
    attr_shape_value.mutable_list()->add_i(std::atoi(dim.c_str()));
  }
}

Status GeOp::BuildInputTensorInfo(OpKernelContext *ctx, std::vector<ge::InputTensorInfo> &inputs) {
  // ctx is not nullptr
  int num_inputs = ctx->num_inputs();

  // populate inputs
  for (int i = 0; i < num_inputs; i++) {
    Tensor tensor(ctx->input(i));
    DataType data_type = tensor.dtype();
    size_t total_bytes = tensor.TotalBytes();
    void *tensor_ptr = DMAHelper::base(&tensor);

    ge::InputTensorInfo input;
    std::shared_ptr<domi::ModelParser> model_parser =
        domi::ModelParserFactory::Instance()->CreateModelParser(domi::FrameworkType::TENSORFLOW);
    REQUIRES_NOT_NULL(model_parser);
    ge::DataType type = model_parser->ConvertToGeDataType(static_cast<uint32_t>(data_type));
    if (type == ge::DT_UNDEFINED) {
      LOG(ERROR) << "[GEOP] No Supported datatype : " << data_type;
      return errors::InvalidArgument("No Supported datatype : ", data_type);
    }
    input.data_type = static_cast<uint32_t>(type);
    input.dims.clear();
    for (uint32_t dim : tensor.shape().dim_sizes()) { input.dims.push_back(static_cast<int64_t>(dim)); }
    input.data = tensor_ptr;
    input.length = static_cast<int64_t>(total_bytes);

    inputs.push_back(input);
  }
  return Status::OK();
}

Status GeOp::BuildOutTensorInfo(OpKernelContext *ctx) {
  int num_outputs = ctx->num_outputs();
    // populate outputs
  for (int i = 0; i < num_outputs; i++) {
    TensorShape out_shape = outputs_shape_.at(i);
    Tensor *tensor = nullptr;
    TF_RETURN_IF_ERROR(ctx->allocate_output(i, out_shape, &tensor));
  }
  return Status::OK();
}

// For each NodeDef, Create Input&Output Desc(shape,format,dataType)
Status GeOp::GenerateDesc(Node *&node) {
  REQUIRES_NOT_NULL(node);
  NodeDef &node_def = const_cast<NodeDef &>(node->def());
  const OpDef &op_def = node->op_def();

  std::string format = this->data_format_;  // format
  int32_t domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_RESERVED;
  TF_RETURN_IF_ERROR(this->DomiFormatFromString(format, domi_format));

  // Get signature(dataType) from the OpDef & NodeDef
  DataTypeVector inputs;
  DataTypeVector outputs;
  TF_RETURN_IF_ERROR(tensorflow::InOutTypesForNode(node_def, op_def, &inputs, &outputs));

  int num;
  Node *in_node = nullptr;
  const Edge *in_edge = nullptr;

  if (inputs.size() > INT_MAX) { return errors::InvalidArgument("inputs size should be less than INT_MAX."); }

  // Create input Desc
  int inputs_size = static_cast<int>(inputs.size());
  if (inputs_size > 0) {
    AttrValue input_tensor_descs;
    AttrValue input_tensor_descs_s;
    num = 0;
    for (; num < inputs_size;) {
      node->input_node(num, &in_node);
      node->input_edge(num, &in_edge);
      REQUIRES_NOT_NULL(in_node);
      REQUIRES_NOT_NULL(in_edge);
      int src_output = in_edge->src_output();
      if (in_node->def().attr().find(OUTPUT_DESC) != in_node->def().attr().end()) {
        NameAttrList desc_attr = in_node->def().attr().at(OUTPUT_DESC).list().func(src_output);
        *(input_tensor_descs.mutable_list()->add_func()) = desc_attr;
      } else {
        LOG(INFO) << "[GEOP] no OUTPUT_DESC: " << node->name() << " <-- " << in_node->name();
        if (num > 0 && node->type_string() == "Merge" && in_node->type_string() == "NextIteration") {
          node->input_node(num - 1, &in_node);
          node->input_edge(num - 1, &in_edge);
          REQUIRES_NOT_NULL(in_node);
          REQUIRES_NOT_NULL(in_edge);
          int src_output = in_edge->src_output();
          NameAttrList desc_attr = in_node->def().attr().at(OUTPUT_DESC).list().func(src_output);
          *(input_tensor_descs.mutable_list()->add_func()) = desc_attr;
        }
      }
      num++;
    }
    REQUIRES_NOT_NULL(node_def.mutable_attr());
    node_def.mutable_attr()->insert({INPUT_DESC, input_tensor_descs});
  }

  // Create output Desc
  if (outputs.size() > 0) {
    // Get infershape
    const std::string KEY_SHAPE = tensorflow::KEY_SHAPE;
    AttrValue shape_value;
    const auto &it = node_def.attr().find(KEY_SHAPE);
    if (it == node_def.attr().end()) {  // no find
      LOG(WARNING) << "[GEOP] There is no infershape of node : " << node_def.name();
    } else {
      shape_value = node_def.attr().at(KEY_SHAPE);
      uint32_t shape_size = static_cast<uint32_t>(shape_value.list().shape_size());
      if (shape_size != outputs.size()) {
        LOG(ERROR) << "[GEOP] size not equal, shape_size : " << shape_size << " outputs size:" << outputs.size();
        shape_value.clear_list();
      }
    }
    // Create output Desc
    AttrValue output_tensor_descs;
    AttrValue output_tensor_descs_s;
    int i = 0;
    num = 0;
    for (DataType data_type : outputs) {
      string desc_string_s;
      AttrValue attr_format_value;
      attr_format_value.set_i((int64_t) domi_format);
      AttrValue attr_datatype_value;
      attr_datatype_value.set_i((int64_t) data_type);

      // shape
      AttrValue attr_shape_value;
      if (shape_value.has_list()) {
        TensorShapeProto shape_proto = shape_value.list().shape(num);
        for (int j = 0; j < shape_proto.dim_size(); j++) {
          attr_shape_value.mutable_list()->add_i(shape_proto.dim(j).size());
        }
      }

      NameAttrList name_attr_list;
      name_attr_list.set_name(std::to_string(i));
      REQUIRES_NOT_NULL(name_attr_list.mutable_attr());
      name_attr_list.mutable_attr()->insert({SERIALIZE_FORMAT, attr_format_value});
      name_attr_list.mutable_attr()->insert({SERIALIZE_DATATYPE, attr_datatype_value});
      name_attr_list.mutable_attr()->insert({SERIALIZE_SHAPE, attr_shape_value});
      REQUIRES_NOT_NULL(output_tensor_descs.mutable_list());
      *(output_tensor_descs.mutable_list()->add_func()) = name_attr_list;

      num++;
      i++;
    }
    node_def.mutable_attr()->erase(KEY_SHAPE);
    node_def.mutable_attr()->insert({OUTPUT_DESC, output_tensor_descs});
  }
  string op_def_string;
  op_def.SerializeToString(&op_def_string);

  tensorflow::AttrValue value;
  value.set_s(op_def_string);
  node_def.mutable_attr()->insert({"op_def", value});
  return tensorflow::Status::OK();
}

Status GeOp::DomiFormatFromString(std::string format, int32_t &domi_format) {
  if (format == "NCHW") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_NCHW;
    return Status::OK();
  } else if (format == "NHWC") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_NHWC;
    return Status::OK();
  } else if (format == "NC1HWC0") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_NC1HWC0;
    return Status::OK();
  } else if (format == "NDHWC") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_NDHWC;
    return Status::OK();
  } else if (format == "NCDHW") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_NCDHW;
    return Status::OK();
  } else if (format == "DHWCN") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_DHWCN;
    return Status::OK();
  } else if (format == "DHWNC") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_DHWNC;
    return Status::OK();
  } else if (format == "FRACTALZ") {
    domi_format = domi::domiTensorFormat_t::DOMI_TENSOR_FRACTAL_Z;
  }
  return errors::Unavailable("DomiFormatFromString, not supported format, format = ", format);
}
}  // namespace tensorflow

namespace tensorflow {
mutex GeOp::mu_(LINKER_INITIALIZED);

const std::string GeOp::INPUT_DESC = "input_tensor_desc";
const std::string GeOp::OUTPUT_DESC = "output_tensor_desc";
const std::string GeOp::SERIALIZE_FORMAT = "serialize_format";
const std::string GeOp::SERIALIZE_DATATYPE = "serialize_datatype";
const std::string GeOp::SERIALIZE_SHAPE = "serialize_shape";
const std::string GeOp::SubGraph = "SubGraph";
std::unordered_map<std::string, uint32_t> GeOp::session_and_graph_id_map_;

REGISTER_KERNEL_BUILDER(Name("GeOp").Device(DEVICE_CPU), GeOp);
}  // namespace tensorflow
