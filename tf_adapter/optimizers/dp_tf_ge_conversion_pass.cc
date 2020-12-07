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

#include "tf_adapter/optimizers/dp_tf_ge_conversion_pass.h"

#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "tensorflow/core/common_runtime/function.h"
#include "tensorflow/core/framework/graph_to_functiondef.h"
#include "tensorflow/core/graph/algorithm.h"
#include "tensorflow/core/graph/graph_constructor.h"
#include "tensorflow/core/graph/node_builder.h"
#include "tensorflow/core/lib/gtl/map_util.h"
#include "tensorflow/core/lib/random/simple_philox.h"
#include "tensorflow/core/public/session_options.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/infershape_util.h"
#include "tf_adapter/util/npu_attrs.h"

namespace tensorflow {
static const int64 kMicrosToMillis = 1000;
// GE ops white list
const static std::vector<std::string> GE_OPS_WHITELIST = {
    "MapDataset",     "ParallelMapDataset",   "BatchDataset", "MapAndBatchDataset", "DeviceQueueDataset",
    "BatchDatasetV2", "MapAndBatchDatasetV2", "ModelDataset", "OptimizeDataset"};

// Customize dataset list
const static std::vector<std::string> CUSTOMIZE_DATASET_LIST = {"BatchDataset",       "BatchDatasetV2",
                                                                "MapAndBatchDataset", "MapAndBatchDatasetV2",
                                                                "ParallelMapDataset", "MakeIterator"};
// Skip dataset list
const static std::vector<std::string> SKIP_DATASET_LIST = {"ModelDataset", "OptimizeDataset"};

// GE fun black list
const static std::vector<std::string> GE_FUN_BLACKLIST = {"PyFunc",
                                                          "SaveV2",
                                                          "RestoreV2",
                                                          "MergeV2Checkpoints",
                                                          "Save",
                                                          "SaveSlices",
                                                          "Restore",
                                                          "RestoreSlice",
                                                          "ShardedFilename",
                                                          "ShardedFilespec",
                                                          "WholeFileReader",
                                                          "WholeFileReaderV2",
                                                          "TextLineReader",
                                                          "TextLineReaderV2",
                                                          "FixedLengthRecordReader",
                                                          "FixedLengthRecordReaderV2",
                                                          "LMDBReader",
                                                          "IdentityReader",
                                                          "IdentityReaderV2",
                                                          "ReaderRead",
                                                          "ReaderReadV2",
                                                          "ReaderReadUpTo",
                                                          "ReaderReadUpToV2",
                                                          "ReaderNumRecordsProduced",
                                                          "ReaderNumRecordsProducedV2",
                                                          "ReaderNumWorkUnitsCompleted",
                                                          "ReaderNumWorkUnitsCompletedV2",
                                                          "ReaderSerializeState",
                                                          "ReaderSerializeStateV2",
                                                          "ReaderRestoreState",
                                                          "ReaderRestoreStateV2",
                                                          "ReaderReset",
                                                          "ReaderResetV2",
                                                          "ReadFile",
                                                          "WriteFile",
                                                          "MatchingFiles",
                                                          "TFRecordReader",
                                                          "TFRecordReaderV2",
                                                          "MutableHashTable",
                                                          "MutableHashTableV2",
                                                          "MutableDenseHashTable",
                                                          "MutableDenseHashTableV2",
                                                          "InitializeTable",
                                                          "InitializeTableV2",
                                                          "InitializeTableFromTextFile",
                                                          "InitializeTableFromTextFileV2",
                                                          "MutableHashTableOfTensors",
                                                          "MutableHashTableOfTensorsV2",
                                                          "HashTable",
                                                          "HashTableV2",
                                                          "LookupTableInsert",
                                                          "LookupTableInsertV2",
                                                          "LookupTableExport",
                                                          "LookupTableExportV2",
                                                          "LookupTableImport",
                                                          "LookupTableImportV2",
                                                          "LookupTableFind",
                                                          "LookupTableFindV2"};
// Mark string for iterator_name
const static std::string DP_ITERATOR_MARK = "_iterator_name";
// Mark string for dp_init graph
const static std::string DP_INIT_GRAPH_MARK = "MakeIterator";
const static std::string DP_INIT_NOOP_GRAPH_MARK = "NoOp";
// Mark string for iterator node
const static std::string DP_INIT_ITERATOR_MARK = "Iterator";
// Mark string for device node
const static std::string DP_INIT_DEVICEQUEUE_MARK = "DeviceQueueDataset";
// Mark string for queue node
const static std::string DP_INIT_QUEUE_MARK = "QueueDataset";
// Used for 0-input NodeDefBuilder
const static std::vector<NodeDefBuilder::NodeOut> EMPTY_DEF_INPUT;
// Used for 0-input NodeBuilder
const static std::vector<NodeBuilder::NodeOut> EMPTY_INPUT;
// Used for 0-type Node(Def)Builder
const static DataTypeVector EMPTY_TYPE;
// Used for 0-shape Node(Def)Builder
const static std::vector<PartialTensorShape> EMPTY_SHAPE;

class DpTfToGEConversionPassImpl {
 public:
  explicit DpTfToGEConversionPassImpl() : graph_run_num_(0), graph_(nullptr), flib_def_(nullptr){};

  ~DpTfToGEConversionPassImpl() = default;
  Status Run(const GraphOptimizationPassOptions &options);

 private:
  Status ProcessGraph(std::unique_ptr<Graph> *graph, FunctionLibraryDefinition *func_lib,
                      const OptimizationPassRegistry::Grouping pass_group_value);
  bool RunPass(std::unique_ptr<Graph> *g, FunctionLibraryDefinition *flib,
               std::map<std::string, std::string> all_options);
  bool CheckMakeIteratorNode(Node *&n) const;
  inline bool IsMakeIteratorNode(const Node *n) const;
  inline bool IsDeviceQueueDatasetNode() const;
  inline bool IsIteratorNode(const Node *n) const;
  inline bool IsSkipDataset(const Node *n) const;
  inline std::string GetEdgeName(const Edge *e) const;
  inline std::string GetRandomName(const std::string &prefix) const;
  std::string GetRandomName() const;
  inline bool EndsWith(const std::string &str, const std::string &suffix) const;
  inline bool CheckNode(const std::string &op) const;
  inline bool IsDeviceSupportedOp(const NodeDef &n) const;
  inline bool IsDeviceSupportedFunc(const std::string &fn) const;
  inline Status GetSplitEdges(const Node *n, std::vector<const Edge *> &split_edges, const Edge *e);
  inline void RemoveSplitEdges(Node *topo_end);
  inline Status InsertChannelQueue(Node *topo_end, std::string &host_queue_name, std::string &device_queue_name) const;
  bool GetNodeFuncs(const FunctionLibraryDefinition *flib_def, Node *node, std::vector<string> &node_funcs);
  bool RemoveIsolatedNode(Graph *g, std::unordered_set<Node *> visited);
  Status RemoveNotSupportDataset(Graph *g, const std::string &device_queue_dataset,
                                 const std::string &make_iterator) const;

  // graph num
  int graph_run_num_;
  // All split edges, split edges means edges that combine A and B in this case
  // 1) A = a node that can only run on tensorflow python host and,
  // 2) B = a node that can run on GE device and all nodes followed B can run on
  // GE device,
  std::unordered_map<Node *, std::vector<const Edge *>> split_edges_;
  // Input graph, not owned
  Graph *graph_;
  // Input flib, not owned
  const FunctionLibraryDefinition *flib_def_;
};

bool DpTfToGEConversionPassImpl::CheckMakeIteratorNode(Node *&n) const {
  if (str_util::StartsWith(n->type_string(), DP_INIT_GRAPH_MARK)) {
    return true;
  } else if (str_util::StartsWith(n->type_string(), DP_INIT_NOOP_GRAPH_MARK)) {
    for (const Edge *e : n->in_edges()) {  // MakeIterator is contains group_deps
      if (e == nullptr || e->src() == nullptr) { continue; }
      if (str_util::StartsWith(e->src()->def().name(), DP_INIT_GRAPH_MARK)) {
        LOG(INFO) << "Remove node: " << n->type_string();
        n = e->src();
        if (e->IsControlEdge()) {
          graph_->RemoveControlEdge(e);
        } else {
          graph_->RemoveEdge(e);
        }
        LOG(INFO) << "PruneForReverseReachability node: " << n->type_string();
        PruneForReverseReachability(&*graph_, {n});
        return true;
      }
    }
  }
  return false;
}

inline bool DpTfToGEConversionPassImpl::IsMakeIteratorNode(const Node *n) const {
  return str_util::StartsWith(n->type_string(), DP_INIT_GRAPH_MARK);
}

inline bool DpTfToGEConversionPassImpl::IsDeviceQueueDatasetNode() const {
  for (const Node *n : graph_->op_nodes()) {
    if (str_util::StartsWith(n->type_string(), DP_INIT_DEVICEQUEUE_MARK)
        || str_util::StartsWith(n->type_string(), DP_INIT_QUEUE_MARK)) {
      return true;
    }
  }
  return false;
}

inline bool DpTfToGEConversionPassImpl::IsIteratorNode(const Node *n) const {
  return str_util::StartsWith(n->type_string(), DP_INIT_ITERATOR_MARK);
}

inline bool DpTfToGEConversionPassImpl::IsSkipDataset(const Node *n) const {
  return std::find(SKIP_DATASET_LIST.begin(), SKIP_DATASET_LIST.end(), n->type_string()) != SKIP_DATASET_LIST.end();
}

inline std::string DpTfToGEConversionPassImpl::GetEdgeName(const Edge *e) const {
  if (e == nullptr || e->src() == nullptr || e->dst() == nullptr) { return "invalid_edge"; }
  return strings::StrCat("Edge_from_", e->src()->name(), "_out", e->src_output(), "_To_", e->dst()->name(), "_in",
                         e->dst_input());
}

inline std::string DpTfToGEConversionPassImpl::GetRandomName(const std::string &prefix) const {
  return strings::StrCat(prefix, "_", GetRandomName());
}

std::string DpTfToGEConversionPassImpl::GetRandomName() const {
  random::PhiloxRandom philox(random::New64(), random::New64());
  random::SimplePhilox rnd(&philox);
  const static size_t RANDOM_LEN = 11;
  const static uint32_t CHARACTER_SETS_HEAD = 2;
  const static uint32_t CHARACTER_SETS = 3;
  const static uint32_t CHARACTER_SET_SIZE[] = {26, 26, 10};  // a-z A-Z 0-9
  const static uint32_t ASCII_UP_A = 65;                      // Ascii of 'A'
  const static uint32_t ASCII_LO_A = 97;                      // Ascii of 'a'
  const static uint32_t ASCII_0 = 48;                         // Ascii of '0'
  const static uint32_t ASCII_BASE[] = {ASCII_UP_A, ASCII_LO_A, ASCII_0};
  string x;
  uint32_t setIdx = 0;
  for (size_t i = 0; i < RANDOM_LEN; i++) {
    if (i == 0) {  // Character must not start with 0-9
      setIdx = rnd.Uniform(CHARACTER_SETS_HEAD);
    } else {
      setIdx = rnd.Uniform(CHARACTER_SETS);
    }
    uint32_t asciiIdx = rnd.Uniform(CHARACTER_SET_SIZE[setIdx]);
    x += char(ASCII_BASE[setIdx] + asciiIdx);
  }
  return x;
}

bool DpTfToGEConversionPassImpl::EndsWith(const std::string &str, const std::string &suffix) const {
  return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool DpTfToGEConversionPassImpl::CheckNode(const std::string &op) const {
  std::string suffix_op = "Dataset";
  std::string suffix_op_v2 = "DatasetV2";
  if (EndsWith(op, suffix_op) || EndsWith(op, suffix_op_v2)) {
    if (std::find(GE_OPS_WHITELIST.begin(), GE_OPS_WHITELIST.end(), op) != GE_OPS_WHITELIST.end()) {
      return true;
    } else {
      return false;
    }
  } else {
    if (std::find(GE_FUN_BLACKLIST.begin(), GE_FUN_BLACKLIST.end(), op) == GE_FUN_BLACKLIST.end()) {
      return true;
    } else {
      return false;
    }
  }
}

bool DpTfToGEConversionPassImpl::IsDeviceSupportedOp(const NodeDef &n) const {
  const OpRegistrationData *op_reg_data = nullptr;
  // Tenorflow original op
  if (OpRegistry::Global() == nullptr) {
    LOG(ERROR) << "OpRegistry global is nullptr";
    return false;
  }
  if (OpRegistry::Global()->LookUp(n.op(), &op_reg_data).ok()) {
    // Node in GE supported
    if (!CheckNode(n.op())) {
      LOG(INFO) << "Node [" << n.name() << "] op [" << n.op() << "] not supported by GE";
      return false;
    } else {  // Top node supported by GE, check its owned function
      for (auto &attr : n.attr()) {
        if (attr.second.has_func()) {
          if (!IsDeviceSupportedFunc(attr.second.func().name())) {
            LOG(INFO) << "Node [" << n.name() << "] function [" << attr.second.func().name() << "] not supported by GE";
            return false;
          }
        }
      }
    }
  } else {  // Not tenorflow original op, this must be a function node
    if (!IsDeviceSupportedFunc(n.op())) {
      LOG(INFO) << "Node [" << n.name() << "] op [" << n.op() << "] is not a supported function by GE";
      return false;
    }
  }
  return true;
}

bool DpTfToGEConversionPassImpl::IsDeviceSupportedFunc(const std::string &fn) const {
  const FunctionDef *fdef = flib_def_->Find(fn);
  // Node contains not found function
  if (fdef == nullptr) {
    LOG(ERROR) << "Function [" << fn << "] not found";
    return false;
  }
  // Recursive check function node
  for (const NodeDef &node : fdef->node_def()) {
    if (!IsDeviceSupportedOp(node)) {
      LOG(INFO) << "Function [" << fn << "] node [" << node.name() << "] not supported by GE";
      return false;
    }
  }
  return true;
}

Status DpTfToGEConversionPassImpl::GetSplitEdges(const Node *n, std::vector<const Edge *> &split_edges,
                                                 const Edge *last_edge) {
  if (IsMakeIteratorNode(n)) {
    for (const Edge *e : n->in_edges()) {
      REQUIRES_NOT_NULL(e);
      if (!IsIteratorNode(e->src())) {
        last_edge = e;
        LOG(INFO) << strings::StrCat("last edge", GetEdgeName(last_edge));
      }
    }
  }
  // GE supported node, continue find
  if (IsDeviceSupportedOp(n->def())) {
    for (const Edge *e : n->in_edges()) {
      REQUIRES_NOT_NULL(e);
      REQUIRES_NOT_NULL(e->src());
      REQUIRES_NOT_NULL(e->dst());
      if (e->IsControlEdge() && !e->src()->IsSource()) {
        return errors::InvalidArgument("Graph contains control edges witch not from _SOURCE, will not try "
                                       "optimize");
      }
      // GE supported node, continue find
      if (IsDeviceSupportedOp(e->src()->def())) {
        Status s = GetSplitEdges(e->src(), split_edges, last_edge);
        if (!s.ok()) { return s; }
      } else {  // GE unsupported node, this is a split edge
        LOG(INFO) << strings::StrCat("Split_", GetEdgeName(e));
        LOG(INFO) << "Begin check split edge.";
        if (IsSkipDataset(e->dst())) {
          LOG(INFO) << "ADD last edge " << GetEdgeName(last_edge);
          split_edges.push_back(last_edge);
        } else {
          LOG(INFO) << "ADD last edge " << GetEdgeName(e);
          split_edges.push_back(e);
        }
      }
    }
  }
  return Status::OK();
}

Status DpTfToGEConversionPassImpl::InsertChannelQueue(Node *topo_end, std::string &host_queue_name,
                                                      std::string &device_queue_name) const {
  LOG(INFO) << "Start to insert HostQueueDataset and DeviceQueueDataset.";
  for (const Edge *e : split_edges_.at(topo_end)) {
    REQUIRES_NOT_NULL(e);
    REQUIRES_NOT_NULL(e->src());
    REQUIRES_NOT_NULL(e->dst());
    std::string queue_name = strings::StrCat("Queue_", GetEdgeName(e), "_", GetRandomName());
    host_queue_name = strings::StrCat("Host", queue_name);
    device_queue_name = strings::StrCat("Device", queue_name);
    LOG(INFO) << "Add_" << host_queue_name;
    // Host and Device queue should save type and shape
    auto m_src = e->src()->def().attr();
    bool type_status = false;
    string::size_type idx;
    idx = SummarizeAttrValue(m_src["output_types"]).find("Unknown AttrValue");
    if (idx == string::npos) { type_status = true; }
    Node *queue_node_host = nullptr;
    // Make sure that 'queue_name' of host and device queue be same
    TF_CHECK_OK(NodeBuilder(host_queue_name, "HostQueueDataset")
                    .Input(e->src(), e->src_output())  // Will be replaced by GEOPDataset later
                    .Input(e->src(), e->src_output())
                    .Device(e->src()->def().device())
                    .Attr("channel_name", queue_name)
                    .Attr("output_types", type_status ? m_src["output_types"] : m_src["Toutput_types"])
                    .Attr("output_shapes", m_src["output_shapes"])
                    .Finalize(&*graph_, &queue_node_host));
    REQUIRES_NOT_NULL(queue_node_host);
    LOG(INFO) << "Add_" << device_queue_name;
    Node *queue_node_device = nullptr;
    // Make sure that 'queue_name' of host and device queue be same
    TF_CHECK_OK(NodeBuilder(device_queue_name, "DeviceQueueDataset")
                    .Device(e->dst()->def().device())
                    .Attr("channel_name", queue_name)
                    .Attr("output_types", type_status ? m_src["output_types"] : m_src["Toutput_types"])
                    .Attr("output_shapes", m_src["output_shapes"])
                    .Finalize(&*graph_, &queue_node_device));
    REQUIRES_NOT_NULL(queue_node_device);
    // 0 means the the 0th output of queue_node_device
    REQUIRES_NOT_NULL(graph_->AddEdge(queue_node_device, 0, e->dst(), e->dst_input()));
  }
  return Status::OK();
}

Status DpTfToGEConversionPassImpl::RemoveNotSupportDataset(Graph *g, const std::string &device_queue_dataset,
                                                           const std::string &make_iterator) const {
  LOG(INFO) << "Begin RemoveSplitDataset.";
  // find device_queue_dataset and make_iterator
  Node *node = nullptr;
  Node *topo_end = nullptr;
  for (Node *n : g->op_nodes()) {
    REQUIRES_NOT_NULL(n);
    if (n->type_string() == "DeviceQueueDataset" && n->name() == device_queue_dataset) {
      LOG(INFO) << "device queue dataset node is " << n->name();
      node = n;
    }
    if (n->type_string() == "MakeIterator" && n->name() == make_iterator) {
      LOG(INFO) << "make iterator node is " << n->name();
      topo_end = n;
    }
  }
  REQUIRES_NOT_NULL(node);
  Node *end_dataset = node;
  std::vector<Node *> delete_nodes;
  while (!IsMakeIteratorNode(node)) {
    if (IsSkipDataset(node)) {
      delete_nodes.push_back(node);
    } else {
      end_dataset = node;
    }
    if (node->num_outputs() != 1) {
      LOG(ERROR) << "Invalid node " << node->name() << ", op is" << node->type_string();
      return errors::InvalidArgument("RemoveSplitDataset: find invalid node.");
    }
    const Edge *edge = nullptr;
    for (const Edge *e : node->out_edges()) { edge = e; }
    REQUIRES_NOT_NULL(edge);
    REQUIRES_NOT_NULL(edge->dst());
    node = edge->dst();
  }
  if (delete_nodes.empty()) {
    LOG(INFO) << "all sink datasets are supported.";
    return Status::OK();
  }
  for (Node *n : delete_nodes) {
    LOG(INFO) << "ready to remove node " << n->name();
    g->RemoveNode(n);
  }
  LOG(INFO) << "end dataset node is " << end_dataset->name();
  REQUIRES_NOT_NULL(g->AddEdge(end_dataset, 0, topo_end, 0));
  return Status::OK();
}

void DpTfToGEConversionPassImpl::RemoveSplitEdges(Node *topo_end) {
  for (const Edge *e : split_edges_.at(topo_end)) {
    LOG(INFO) << "Remove_" << GetEdgeName(e);
    graph_->RemoveEdge(e);
  }
}

bool DpTfToGEConversionPassImpl::GetNodeFuncs(const FunctionLibraryDefinition *flib_def, Node *node,
                                              std::vector<string> &node_funcs) {
  node_funcs.clear();
  for (auto iter = node->attrs().begin(); iter != node->attrs().end(); ++iter) {
    if (iter->second.has_func()) {
      node_funcs.push_back(iter->second.func().name());
      std::vector<string> func_name_stack;
      func_name_stack.clear();
      func_name_stack.push_back(iter->second.func().name());
      while (!func_name_stack.empty()) {
        string func_name = func_name_stack.back();
        func_name_stack.pop_back();
        const FunctionDef *fdef = flib_def->Find(func_name);
        if (fdef != nullptr) {
          for (const NodeDef &ndef : fdef->node_def()) {
            for (auto &item : ndef.attr()) {
              if (item.second.has_func()) {
                node_funcs.push_back(item.second.func().name());
                func_name_stack.push_back(item.second.func().name());
                continue;
              }
            }
          }
        }
      }
      continue;
    }
  }
  return !node_funcs.empty();
}

bool DpTfToGEConversionPassImpl::RunPass(std::unique_ptr<Graph> *g, FunctionLibraryDefinition *flib,
                                         std::map<std::string, std::string> all_options) {
  LOG(INFO) << ">>>> DpTfToGEConversionPassImpl::RunPass <<<<";
  // Convert just for convenient access
  split_edges_.clear();
  graph_ = &**g;
  flib_def_ = &(*g)->flib_def();

  // Find split edges from subgraphs, which MakeIterator connect to Itearator op
  std::vector<Node *> topo_ends;
  for (Node *node : graph_->op_nodes()) {
    if (IsMakeIteratorNode(node)) {
      for (Node *in_node : node->in_nodes()) {
        if (IsIteratorNode(in_node)) {
          topo_ends.push_back(node);
          LOG(INFO) << "Insert topo end node " << node->name();
          break;
        }
      }
    }
  }
  // After traversal, topo_ends should store MakeIterator Nodes.
  if (topo_ends.empty()) {
    LOG(INFO) << "Do not find MakeIterator <- IteratorV2 connects in the graph,"
              << " pass datapreprocess pass.";
    return true;
  }
  LOG(INFO) << "Start to write graph's pbtxt before optimization.";

  const char *need_print = getenv("PRINT_MODEL");
  if (nullptr != need_print && strcmp("1", need_print) == 0) {
    GraphDef before_graphdef;
    (*g)->ToGraphDef(&before_graphdef);
    string pre_model_path = "BeforeSubGraph_dp_";
    string pmodel_path = pre_model_path + std::to_string(graph_run_num_) + ".pbtxt";
    TF_DO_CHECK_OK(WriteTextProto(Env::Default(), pmodel_path, before_graphdef), ERROR);
  }

  LOG(INFO) << "Start to optimize dp_init topological graph";
  for (Node *topo_end : topo_ends) {
    // Get all edges that should be replace with HostQueue->DeviceQueue
    LOG(INFO) << "Start to find split edges, topo_end node is : " << topo_end->name() << ", op is "
              << topo_end->type_string();
    const Edge *tmp_edge = nullptr;
    TF_DO_CHECK_OK(GetSplitEdges(topo_end, split_edges_[topo_end], tmp_edge), ERROR);

    const string DEFAULT_DEVICE = topo_end->def().device();
    // Start optimize graph
    // Insert Host and Device queue
    LOG(INFO) << "Start to add host and device queue on split edges";
    std::string host_queue_name;
    std::string device_queue_name;
    TF_DO_CHECK_OK(InsertChannelQueue(topo_end, host_queue_name, device_queue_name), ERROR);
    LOG(INFO) << "host queue name is " << host_queue_name;
    LOG(INFO) << "device queue name is " << device_queue_name;
    // Remove all split edges
    LOG(INFO) << "Start to remove split edges";
    RemoveSplitEdges(topo_end);

    // Make a copy of graph for pruned GE
    LOG(INFO) << "Start to prune GE graph";
    std::unique_ptr<Graph> graph_ge(new (std::nothrow) Graph(OpRegistry::Global()));
    if (graph_ge == nullptr) {
      LOG(ERROR) << "new graph ge failed";
      return false;
    }
    CopyGraph(*graph_, &*graph_ge);
    // Prune visiable GE graph
    std::unordered_set<const Node *> visiable_ge;
    for (const Node *n : graph_ge->op_nodes()) {
      if (IsMakeIteratorNode(n) && n->name() == topo_end->name()) {
        visiable_ge.emplace(n);
        break;
      }
    }
    TF_DO_CHECK_OK(RemoveNotSupportDataset(&*graph_ge, device_queue_name, topo_end->name()), ERROR);

    LOG(INFO) << "Start to to PruneForReverseReachability.";
    PruneForReverseReachability(&*graph_ge, visiable_ge);
    // add function_def begin
    LOG(INFO) << "Start to add function_def for GEOP's func";
    FunctionDefLibrary fdeflib;
    for (auto node : graph_ge->nodes()) {
      std::vector<string> node_funcs;
      if (GetNodeFuncs(flib, node, node_funcs)) {
        LOG(INFO) << "Node [" << node->name() << "] has func:";
        for (const auto &func : node_funcs) {
          FunctionDef *fdef = fdeflib.add_function();
          if (flib->Find(func) == nullptr) {
            LOG(ERROR) << "function def is nullptr";
            return false;
          }
          *fdef = *(flib->Find(func));
        }
      }
    }

    // Add required function for GEOPDataset->func(GEOP->func2) topo graph
    std::string fn_dpop = GetRandomName("dpop_function");
    std::string fn_geop = GetRandomName("geop_function");
    std::string fn_geop_dataset = GetRandomName("geopdataset_function");
    std::string iterator_name = "";
    for (auto in_node : topo_end->in_nodes()) {
      if (in_node == nullptr) {
        LOG(ERROR) << "topo end node is nullptr";
        return false;
      }
      LOG(INFO) << "in_node name is " << in_node->name();
      if (IsIteratorNode(in_node)) {
        iterator_name = in_node->name();
        LOG(INFO) << "iterator name is " << iterator_name;
        break;
      }
    }
    if (iterator_name.empty()) {
      LOG(ERROR) << "There is no connection between MakeIteraotr and IteratorV2";
      return false;
    }
    // Add dp custom kernel label
    for (auto node : graph_ge->nodes()) {
      if (node->type_string() == "DeviceQueueDataset") { node->AddAttr(DP_ITERATOR_MARK, iterator_name); }
      if (std::find(CUSTOMIZE_DATASET_LIST.begin(), CUSTOMIZE_DATASET_LIST.end(), node->type_string())
          != CUSTOMIZE_DATASET_LIST.end()) {
        LOG(INFO) << node->name() << " is " << node->type_string() << ", need to add label.";
        node->AddAttr("_kernel", "dp");
        node->AddAttr(DP_ITERATOR_MARK, iterator_name);
      }
    }
    // Convert GE graph to GEOP function body
    {
      LOG(INFO) << "Start to convert GE graph to geop function";
      FunctionDef *fd = fdeflib.add_function();
      TF_DO_CHECK_OK(GraphToFunctionDef(*graph_ge, fn_dpop, fd), ERROR);
    }
    // Add DPOP node(visable only by function of geop)
    {
      string func_def_str;
      fdeflib.SerializeToString(&func_def_str);

      // DPOP node should created by function of geop
      LOG(INFO) << "Start to convert dpop node to geop function";
      FunctionDef *fd = fdeflib.add_function();
      if (fd == nullptr || fd->mutable_signature() == nullptr) {
        LOG(ERROR) << "fd is nullptr";
        return false;
      }
      fd->mutable_signature()->set_name(fn_geop);
      NodeDef *n = fd->add_node_def();
      if (n == nullptr) {
        LOG(ERROR) << "fd node def is nullptr";
        return false;
      }
      NameAttrList f_attr;
      f_attr.set_name(fn_dpop);
      *f_attr.mutable_attr() = n->attr();
      TF_CHECK_OK(NodeDefBuilder(fn_dpop, "DPOP")
                      .Input(EMPTY_DEF_INPUT)  // No partition dp_init graph on GE
                      .Device(DEFAULT_DEVICE)
                      .Attr("function", f_attr)  // dpop funcion
                      .Attr("func_def", func_def_str)
                      .Attr("Tin", EMPTY_TYPE)
                      .Attr("Tout", EMPTY_TYPE)
                      .Attr("Tout", EMPTY_TYPE)
                      .Finalize(n));  // n is created by function of geop function
    }
    {
      // GEOP node should created by function of geopDataset
      LOG(INFO) << "Start to convert geop node to geopdataset function";
      FunctionDef *fd = fdeflib.add_function();
      if (fd == nullptr || fd->mutable_signature() == nullptr) {
        LOG(ERROR) << "fd is nullptr";
        return false;
      }
      fd->mutable_signature()->set_name(fn_geop_dataset);
      NodeDef *n = fd->add_node_def();
      if (n == nullptr) {
        LOG(ERROR) << "fd node def is nullptr";
        return false;
      }
      NameAttrList f_attr;
      f_attr.set_name(fn_geop);
      *f_attr.mutable_attr() = n->attr();
      TF_CHECK_OK(NodeDefBuilder(GetRandomName("GeOp"), "GeOp")
                      .Input(EMPTY_DEF_INPUT)  // No partition dp_init graph on GE
                      .Device(DEFAULT_DEVICE)
                      .Attr("function", f_attr)  // geop funcion
                      .Attr("Tin", EMPTY_TYPE)
                      .Attr("Tout", EMPTY_TYPE)
                      .Attr("Tout", EMPTY_TYPE)
                      .Attr("_enableDP", true)
                      .Finalize(n));  // n is created by function of geopDataset function
      std::string attr_name = "";
      for (auto option : all_options) {
        attr_name = std::string("_") + option.first;
        AddNodeAttr(attr_name, option.second, n);
      }
      AddNodeAttr("_NpuOptimizer", "NpuOptimizer", n);
    }
    // Update graph function libray
    LOG(INFO) << "Start to add geop and geopdataset function in graph library";
    // Not a must, just for Tensorbord viewing convenience
    graph_->AddFunctionLibrary(fdeflib);
    flib->AddLibrary(fdeflib);

    // Add GEOPDataset node to graph_
    std::vector<const Edge *> topo_end_input_edges(topo_end->in_edges().begin(), topo_end->in_edges().end());
    Node *dpgroup_dataset_node = nullptr;
    Node *geop_dataset_node = nullptr;
    std::unordered_set<Node *> isolated_nodes;
    {
      LOG(INFO) << "Start to add geopdataset node in graph";
      const Node *n = nullptr;
      for (const Edge *e : topo_end_input_edges) {
        if (IsIteratorNode(e->src())) { n = e->src(); }
      }
      if (n == nullptr) {
        LOG(ERROR) << "edge src is nullptr";
        return false;
      }
      auto m_src = n->def().attr();

      NameAttrList f_attr;
      f_attr.set_name(fn_geop_dataset);
      // Combine all host queue dataset with GEOPDataset
      std::vector<NodeBuilder::NodeOut> inputs;
      for (Node *n : graph_->op_nodes()) {
        // host tf makeiterator add dp label
        if (IsMakeIteratorNode(n)) { n->AddAttr("_kernel", "dp"); }
        if (n->type_string() == "HostQueueDataset" && n->name() == host_queue_name) {
          // 0: Host queue always generate one dataset
          LOG(INFO) << "inputs add node : name is " << n->name() << ", op is " << n->type_string();
          inputs.push_back(NodeBuilder::NodeOut(n, 0));
        }
        if (n->type_string().compare("DeviceQueueDataset") == 0 && n->name() == device_queue_name) {
          isolated_nodes.insert(n);
        }
      }

      TF_CHECK_OK(NodeBuilder(GetRandomName("DPGroupDataset"), "DPGroupDataset")
                      .Input(inputs)  // All host queue flow into geopDataset for driver
                      .Device(DEFAULT_DEVICE)
                      .Attr("output_types", m_src["output_types"])
                      .Attr("output_shapes", m_src["output_shapes"])
                      .Finalize(&*graph_,
                                &dpgroup_dataset_node));  // Finalize geopDataset in graph_
      TF_CHECK_OK(NodeBuilder(GetRandomName("GeopDataset"), "GEOPDataset")
                      .Device(DEFAULT_DEVICE)
                      .Attr("f", f_attr)  // geopDataset function
                      .Finalize(&*graph_,
                                &geop_dataset_node));  // Finalize geopDataset in graph_

      for (Node *n : graph_->op_nodes()) {
        if (n->type_string() == "HostQueueDataset" && n->name() == host_queue_name) {
          graph_->RemoveEdge(*(n->in_edges().begin()));
          graph_->AddEdge(geop_dataset_node, 0, n, 0);
        }
      }
    }
    // Remove all edges flow to MakeIterator except the one from IteratorV2
    LOG(INFO) << "Start to combine geopdataset with iterator node and remove "
                 "orignal edges";

    // We must copy all topoend input edges as we can't modify it when combine
    // geopdataset an topoend
    for (const Edge *e : topo_end_input_edges) {
      if (!IsIteratorNode(e->src())) {
        CHECK_NOTNULL(graph_->AddEdge(dpgroup_dataset_node, 0, e->dst(), e->dst_input()));
        LOG(INFO) << "Remove_" << GetEdgeName(e);
        graph_->RemoveEdge(e);
      }
    }
    // Prune for the final optimized graph
    LOG(INFO) << "Start to prune final optimized graph";

    RemoveIsolatedNode(&*graph_, isolated_nodes);
    LOG(INFO) << "Start to assign unassigned node on default device";
    // We do pass after assign, so we must assign all new added nodes
    for (Node *n : (*g)->op_nodes()) {
      if (n->assigned_device_name().empty()) {
        // Use device of MakeIterator node as default
        n->set_assigned_device_name(DEFAULT_DEVICE);
        LOG(INFO) << "Assigned node [" << n->name() << "] on device [" << n->assigned_device_name() << "]";
      }
    }
  }

  LOG(INFO) << "End optimize dp_init topological graph";
  if (nullptr != need_print && strcmp("1", need_print) == 0) {
    GraphDef after_graphdef;
    (*g)->ToGraphDef(&after_graphdef);
    string suffix_model_path = "AfterSubGraph_dp_";
    string smodel_path = suffix_model_path + std::to_string(graph_run_num_) + ".pbtxt";
    TF_DO_CHECK_OK(WriteTextProto(Env::Default(), smodel_path, after_graphdef), ERROR);
  }
  return true;
}

bool DpTfToGEConversionPassImpl::RemoveIsolatedNode(Graph *g, std::unordered_set<Node *> visited) {
  // Compute set of nodes that we need to traverse in order to reach
  // the nodes in "nodes" by performing a breadth-first search from those
  // nodes, and accumulating the visited nodes.
  std::deque<Node *> queue;
  for (Node *n : visited) {
    VLOG(2) << "Reverse reach init: " << n->name();
    queue.push_back(n);
  }
  while (!queue.empty()) {
    Node *n = queue.front();
    queue.pop_front();
    for (Node *out : n->out_nodes()) {
      if (visited.insert(out).second) {
        queue.push_back(out);
        VLOG(2) << "Reverse reach : " << n->name() << " from " << out->name();
      }
    }
  }

  // Make a pass over the graph to remove nodes in "visited"
  std::vector<Node *> all_nodes;
  all_nodes.reserve(g->num_nodes());
  for (Node *n : g->nodes()) { all_nodes.push_back(n); }

  bool any_removed = false;
  for (Node *n : all_nodes) {
    if (visited.count(n) != 0) {
      g->RemoveNode(n);
      any_removed = true;
    }
  }

  return any_removed;
}

Status DpTfToGEConversionPassImpl::Run(const GraphOptimizationPassOptions &options) {
  if ((options.graph == nullptr && options.partition_graphs == nullptr) || options.flib_def == nullptr) {
    return Status::OK();
  }

  Status s = Status::OK();
  if (options.graph != nullptr) {
    std::unique_ptr<Graph> *graph = options.graph;
    FunctionLibraryDefinition *func_lib = options.flib_def;
    s = ProcessGraph(graph, func_lib, OptimizationPassRegistry::POST_REWRITE_FOR_EXEC);
    if (s != Status::OK()) { return s; }
  } else if (options.partition_graphs != nullptr) {
    for (auto &pg : *options.partition_graphs) {
      std::unique_ptr<Graph> *graph = &pg.second;
      FunctionLibraryDefinition *func_lib = options.flib_def;
      s = ProcessGraph(graph, func_lib, OptimizationPassRegistry::POST_PARTITIONING);
      if (s != Status::OK()) { return s; }
    }
  }

  return Status::OK();
}

static std::atomic<int> graph_run_num(1);
static mutex graph_num_mutex(LINKER_INITIALIZED);
Status DpTfToGEConversionPass::Run(const GraphOptimizationPassOptions &options) {

  return DpTfToGEConversionPassImpl().Run(options);
}

Status DpTfToGEConversionPassImpl::ProcessGraph(std::unique_ptr<Graph> *graph, FunctionLibraryDefinition *func_lib,
                                                const OptimizationPassRegistry::Grouping pass_group_value) {
  int64 startTime = InferShapeUtil::GetCurrentTimestap();

  graph_run_num_ = graph_run_num++;

  if (graph == nullptr) { return Status::OK(); }

  for (Node *n : graph->get()->nodes()) {
    REQUIRES_NOT_NULL(n);
    if (n->attrs().Find("_NoNeedOptimize")) {
      LOG(INFO) << "Found mark of noneed optimize on node [" << n->name() << "], skip DpTfToGEConversionPass.";
      return Status::OK();
    }
  }

  std::map<std::string, std::string> all_options;
  std::map<std::string, std::string> pass_options;
  pass_options = NpuAttrs::GetDefaultPassOptions();

  for (Node *n : graph->get()->nodes()) {
    REQUIRES_NOT_NULL(n);
    if (n->attrs().Find("_NpuOptimizer")) {
      pass_options = NpuAttrs::GetPassOptions(n->attrs());
      all_options = NpuAttrs::GetAllAttrOptions(n->attrs());
      break;
    }
  }
  std::string job = pass_options["job"];
  if (job == "ps" || job == "default") {
    LOG(INFO) << "job is " << job << " Skip the optimizer : DpTfToGEConversionPass.";
    return Status::OK();
  }
  if (job == "localhost" && pass_group_value != OptimizationPassRegistry::POST_REWRITE_FOR_EXEC) {
    return Status::OK();
  }
  if (job != "localhost" && pass_group_value != OptimizationPassRegistry::POST_PARTITIONING) { return Status::OK(); }

  bool enableDP = (pass_options["enable_dp"] == "1");
  bool use_off_line = (pass_options["use_off_line"] == "1");
  bool do_npu_optimizer = (pass_options["do_npu_optimizer"] == "1");
  if (do_npu_optimizer) {
    if (!use_off_line) {
      LOG(INFO) << "Run online process and skip the optimizer";
      return Status::OK();
    }
  } else {
    return Status::OK();
  }

  if (!enableDP) {
    LOG(INFO) << "DpTfToGEConversionPassImpl::RunPass, enable data preproc is false";
    return Status::OK();
  }
  auto process_graph = [&](std::unique_ptr<Graph> *g, FunctionLibraryDefinition *flib,
                           std::map<std::string, std::string> all_options) { RunPass(g, flib, all_options); };

  // For any pre-partitioning phase, graph is stored in options.graph.
  process_graph(graph, func_lib, all_options);
  int64 endTime = InferShapeUtil::GetCurrentTimestap();
  LOG(INFO) << "DpTfToGEConversionPassImpl Run success. [" << ((endTime - startTime) / kMicrosToMillis) << " ms]";

  return Status::OK();
}

// We register DpTfToGE insertion for phase 102 in POST_PARTITIONING grouping
REGISTER_OPTIMIZATION(OptimizationPassRegistry::POST_REWRITE_FOR_EXEC, 3, DpTfToGEConversionPass);
REGISTER_OPTIMIZATION(OptimizationPassRegistry::POST_PARTITIONING, 102, DpTfToGEConversionPass);
}  // namespace tensorflow
