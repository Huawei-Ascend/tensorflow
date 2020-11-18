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

#include "tf_adapter/optimizers/gradient_fusion_optimizer.h"
#include "tf_adapter/common/common.h"

#include "tensorflow/core/grappler/grappler_item.h"
#include "tensorflow/core/grappler/utils/topological_sort.h"
#include "tensorflow/core/util/overflow.h"

namespace tensorflow {
namespace grappler {
const string OP_TYPE_BROADCAST = "HcomBroadcast";

const string DATA_TYPE_ATTR = "T";
const string SHAPE_ATTR = "shape";
const string GROUP_ATTR = "group";
const string ROOT_RANK_ATTR = "root_rank";
const string REDUCE_TYPE_ATTR = "reduction";
const string FUSION_ATTR = "fusion";
const string FUSION_ID_ATTR = "fusion_id";

Status SizeofDataType(DataType dType, size_t &size) {
  static const std::map<DataType, int> dataTypeSizeMap = {
      {DT_INVALID, 0},     {DT_FLOAT, 4},      {DT_DOUBLE, 8},          {DT_INT32, 4},        {DT_UINT8, 1},
      {DT_INT16, 2},       {DT_INT8, 1},       {DT_COMPLEX64, 8},       {DT_INT64, 8},        {DT_BOOL, 1},
      {DT_QINT8, 8},       {DT_QUINT8, 8},     {DT_QINT32, 4},          {DT_BFLOAT16, 2},     {DT_QINT16, 2},
      {DT_QUINT16, 2},     {DT_UINT16, 2},     {DT_COMPLEX128, 16},     {DT_HALF, 2},         {DT_UINT32, 4},
      {DT_UINT64, 8},      {DT_FLOAT_REF, 4},  {DT_DOUBLE_REF, 8},      {DT_INT32_REF, 4},    {DT_UINT8_REF, 1},
      {DT_INT16_REF, 2},   {DT_INT8_REF, 1},   {DT_COMPLEX64_REF, 8},   {DT_INT64_REF, 8},    {DT_BOOL_REF, 1},
      {DT_QINT8_REF, 1},   {DT_QUINT8_REF, 1}, {DT_QINT32_REF, 4},      {DT_BFLOAT16_REF, 2}, {DT_QINT16_REF, 2},
      {DT_QUINT16_REF, 2}, {DT_UINT16_REF, 2}, {DT_COMPLEX128_REF, 16}, {DT_HALF_REF, 2},     {DT_UINT32_REF, 4},
      {DT_UINT64_REF, 8},
  };
  auto iter = dataTypeSizeMap.find(dType);
  if (iter != dataTypeSizeMap.end()) {
    size = iter->second;
    return Status::OK();
  }
  return errors::InvalidArgument("data type not support");
}

bool GradFusionOptimizer::IsHcomOp(const NodeDef &nodeDef) {
  if (nodeDef.op() == OP_TYPE_BROADCAST) {
    return true;
  } else {
    return false;
  }
}

string GetNodeScopeName(NodeDef &nodeDef) {
  string::size_type pos = nodeDef.name().find_last_of('/');
  if (pos == string::npos) { return string(""); }
  return nodeDef.name().substr(0, pos + 1);
}

Status GradFusionOptimizer::GetInputTensorSize(NodeDef &nodeDef, int64_t &inputTensorSize) {
  int64_t inputTensorSizeRet = 0;
  int64_t inputTensorSizeRetTmp = 1;
  string err;
  TensorShapeProto shape;
  auto attrMap = nodeDef.attr();
  int inputsNameSize = nodeDef.input_size();
  if (inputsNameSize > 0) {
    for (int i = 0; i < inputsNameSize; i++) {
      const string &inputNodeName = nodeDef.input(i);
      if (inputNodeName.empty()) {
        LOG(INFO) << "Cannot get input node name, curr node : " << nodeDef.name() << " index: " << i;
        continue;
      }
      NodeDef inputNode = nameToNode_[inputNodeName];
      shape = (*inputNode.mutable_attr())[SHAPE_ATTR].shape();
      if (!shape.unknown_rank()) {
        inputTensorSizeRetTmp = 1;
        for (const auto &dim : shape.dim()) {
          if (dim.size() == -1) {
            err = string("Unknown size");
            break;
          }
          inputTensorSizeRetTmp = MultiplyWithoutOverflow(inputTensorSizeRetTmp, dim.size());
          if (inputTensorSizeRetTmp < 0) { return errors::InvalidArgument("input tensor size is overflow"); }
        }
      } else {
        err = string("Unknown rank");
      }
      if (!err.empty()) { break; }
      if (INT64_MAX - inputTensorSizeRetTmp < inputTensorSizeRet) {
        return errors::InvalidArgument("input tensor size is overflow");
      }
      inputTensorSizeRet += inputTensorSizeRetTmp;
    }
  } else {
    err = string("Does not have the attr of \'inputs_name\'");
  }
  if (!err.empty()) {
    return errors::InvalidArgument("GetInputTensorSize failed. Node name: ", nodeDef.name(), ", error msg: ", err);
  }

  DataType dType = (*nodeDef.mutable_attr())[DATA_TYPE_ATTR].list().type(0);
  size_t size = 0;
  Status ret = SizeofDataType(dType, size);
  REQUIRES_STATUS_OK(ret);
  inputTensorSize = MultiplyWithoutOverflow(inputTensorSizeRet, size);
  if (inputTensorSize < 0) { return errors::InvalidArgument("input tensor size is overflow"); }
  return Status::OK();
}

Status GradFusionOptimizer::SetHcomBroadcastAttr(NodeDef *fusionNode, NodeDef &originNode) {
  std::set<string> &fusionBroadcastName = fusionOpPool_[fusionNode->op()];
  DataType dType = (*originNode.mutable_attr())[DATA_TYPE_ATTR].list().type(0);
  if (fusionBroadcastName.count(fusionNode->name()) == 0) {
    int64 rootRank = (*originNode.mutable_attr())[ROOT_RANK_ATTR].i();
    (*fusionNode->mutable_attr())[ROOT_RANK_ATTR].set_i(rootRank);
    string group = (*originNode.mutable_attr())[GROUP_ATTR].s();
    (*fusionNode->mutable_attr())[GROUP_ATTR].set_s(group);
    (*fusionNode->mutable_attr())[FUSION_ATTR].set_i(0);
    (*fusionNode->mutable_attr())[FUSION_ID_ATTR].set_i(-1);
    fusionBroadcastName.insert(fusionNode->name());
  }
  // All input data types are the same.
  (*fusionNode->mutable_attr())[DATA_TYPE_ATTR].mutable_list()->add_type(dType);
  return Status::OK();
}

Status GradFusionOptimizer::SetFusionNodeAttr(NodeDef *fusionNode, NodeDef &originNode) {
  string opType = fusionNode->op();
  if (opType == OP_TYPE_BROADCAST) { return SetHcomBroadcastAttr(fusionNode, originNode); }
  return Status::OK();
}

Status GradFusionOptimizer::FusionOp(std::vector<NodeDef> fusionHcomOps, GraphDef *graphDef) {
  std::set<NodeDef *> outputsSet;
  string opType = fusionHcomOps[0].op();
  if (fusionOpInfo_.count(opType) == 0) { fusionOpInfo_[opType].push_back(std::make_pair(0, "")); }
  int &fusionOpIndex = fusionOpInfo_[opType].back().first;
  string &fusionOpName = fusionOpInfo_[opType].back().second;
  string scope = GetNodeScopeName(fusionHcomOps[0]);
  string fusionNodeName = scope + "FusionNode_" + opType + "_" + std::to_string(fusionOpIndex);

  // add fusion node
  NodeDef *fusionNode = graphDef->add_node();
  REQUIRES_NOT_NULL(fusionNode);
  fusionNode->set_name(fusionNodeName);
  fusionNode->set_device(fusionHcomOps[0].device());
  fusionNode->set_op(opType);

  LOG(INFO) << "INFO: GradFusionOptimizer::FusionOp  New FusionNodeName: " << fusionNodeName;
  int fusionOutputIdx = 0;
  std::set<string> fusionInputs;
  std::set<string> fusionCtrlInputs;
  for (auto &nodeDef : fusionHcomOps) {
    for (const string &input : nodeDef.input()) {
      if (fusionInputs.count(input) == 1) { continue; }
      // Control dependencies must come after regular dependencies
      if (!input.empty() && str_util::StartsWith(input, "^")) {
        fusionCtrlInputs.insert(input);
        continue;
      }
      fusionNode->add_input(input);
      TF_RETURN_IF_ERROR(SetFusionNodeAttr(fusionNode, nodeDef));
      fusionInputs.insert(input);
    }
    fusionInputs.clear();

    // outputs
    outputsSet = nodeMap_->GetOutputs(nodeDef.name());
    for (auto outNodeDef : outputsSet) {
      int idx = 0;
      for (const auto &input : outNodeDef->input()) {
        if (input.find(nodeDef.name()) == std::string::npos) {
          idx++;
          continue;
        }
        if (input.find('^') == std::string::npos) {
          outNodeDef->set_input(idx, fusionNodeName + ":" + std::to_string(fusionOutputIdx));
          fusionOutputIdx++;
        } else {
          outNodeDef->set_input(idx, "^" + fusionNodeName);
        }
      }
    }

    // delete origin nodes
    ::google::protobuf::RepeatedPtrField<::tensorflow::NodeDef> *nodeList = graphDef->mutable_node();
    for (::google::protobuf::RepeatedPtrField<::tensorflow::NodeDef>::const_iterator const_it = nodeList->begin();
         const_it != nodeList->end(); ++const_it) {
      if (const_it->name() == nodeDef.name()) {
        nodeList->erase(const_it);
        break;
      }
    }
  }
  if (!fusionCtrlInputs.empty()) {
    for (const string &ctrlInput : fusionCtrlInputs) { fusionNode->add_input(ctrlInput); }
  }
  fusionCtrlInputs.clear();

  // add control edges
  if (fusionOpIndex >= 1) { fusionNode->add_input("^" + fusionOpName); }

  // update fusion op info.
  fusionOpIndex++;
  fusionOpName = fusionNodeName;

  return Status::OK();
}

int64 GradFusionOptimizer::GetFusionTensorSize() {
  const char *env = getenv("FUSION_TENSOR_SIZE");
  // default (50KBytes)
  const int64 fusionTensorSizeDefault = 524288000;
  if (env == nullptr || strlen(env) >= ADAPTER_ENV_MAX_LENTH) { return fusionTensorSizeDefault; }
  string envSize(env);
  std::istringstream ss(envSize);
  int64 fusionTensorSize;
  if (!(ss >> fusionTensorSize)) { fusionTensorSize = fusionTensorSizeDefault; }
  return fusionTensorSize;
}

Status GradFusionOptimizer::Optimize(Cluster *cluster, const GrapplerItem &item, GraphDef *optimizedGraph) {
  REQUIRES_NOT_NULL(optimizedGraph);
  const int64 fusionTensorSize = GetFusionTensorSize();
  GraphDef graphOrigin;
  std::map<std::pair<string, DataType>, std::vector<NodeDef>> fusionHcomOps;
  std::map<std::pair<string, DataType>, int64_t> currentGradSumSize;
  *optimizedGraph = item.graph;
  LOG(INFO) << "INFO: GradFusionOptimizer::Optimize begin, OriginNodeNum: " << item.graph.node_size();
  LOG(INFO) << "INFO: FUSION_TENSOR_SIZE: " << fusionTensorSize;

  if (fusionTensorSize < 0) { return errors::InvalidArgument("FUSION_TENSOR_SIZE is invalid"); }

  REQUIRES_STATUS_OK(TopologicalSort(optimizedGraph));
  nodeMap_.reset(new (std::nothrow) NodeMap(optimizedGraph));
  REQUIRES_NOT_NULL(nodeMap_);
  fusionOpInfo_.clear();
  fusionOpPool_.clear();
  graphOrigin = *optimizedGraph;
  for (const auto &nodeDef : graphOrigin.node()) { nameToNode_[nodeDef.name()] = nodeDef; }

  for (const auto &nodeDef : graphOrigin.node()) {
    if (IsHcomOp(nodeDef)) {
      DataType dType;
      auto attrMap = nodeDef.attr();
      auto iter = attrMap.find(DATA_TYPE_ATTR);
      if (iter != attrMap.end()) {
        dType = iter->second.list().type(0);
      } else {
        LOG(INFO) << "INFO: Use default dataType: DT_FLOAT";
        dType = DT_FLOAT;
      }
      std::pair<string, DataType> key = std::make_pair(nodeDef.op(), dType);

      fusionHcomOps[key].push_back(nodeDef);
      int64_t inputTensorSize = 0;
      NodeDef tmpNode = nodeDef;
      TF_RETURN_IF_ERROR(GetInputTensorSize(tmpNode, inputTensorSize));
      if (currentGradSumSize.count(key) != 0) {
        if (INT64_MAX - inputTensorSize < currentGradSumSize[key]) {
          return errors::InvalidArgument("input tensor size is overflow");
        }
        currentGradSumSize[key] += inputTensorSize;
      } else {
        currentGradSumSize[key] = inputTensorSize;
      }
      if (currentGradSumSize[key] >= fusionTensorSize) {
        if (fusionHcomOps[key].size() > 1) { TF_RETURN_IF_ERROR(FusionOp(fusionHcomOps[key], optimizedGraph)); }
        fusionHcomOps[key].clear();
        currentGradSumSize[key] = 0;
      }
    }
  }

  for (auto iter : fusionHcomOps) {
    if (!iter.second.empty()) {
      if (iter.second.size() > 1) { TF_RETURN_IF_ERROR(FusionOp(iter.second, optimizedGraph)); }
      iter.second.clear();
    }
  }
  LOG(INFO) << "INFO: GradFusionOptimizer::Optimize end, finalNodeNum: " << optimizedGraph->node_size();

  return Status::OK();
}

REGISTER_GRAPH_OPTIMIZER(GradFusionOptimizer);
}  // end namespace grappler
}  // end namespace tensorflow
