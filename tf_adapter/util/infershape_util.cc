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

#include "tf_adapter/util/infershape_util.h"
#include "tensorflow/core/framework/node_def_util.h"
#include "tf_adapter/common/common.h"
#include "tf_adapter/util/npu_ops_identifier.h"

namespace tensorflow {
struct EdgeInfo {
  EdgeInfo(Node *src, Node *dst, int src_output, int dst_input)
      : src_(src), dst_(dst), src_output_(src_output), dst_input_(dst_input) {}

  Node *src_;
  Node *dst_;
  int src_output_;
  int dst_input_;
};

int64 InferShapeUtil::GetCurrentTimestap() {
  struct timeval tv;
  int ret = gettimeofday(&tv, nullptr);
  if (ret != 0) {
    LOG(ERROR) << "Func gettimeofday may failed, ret:" << ret;
    return 0;
  }
  int64 totalUsec = tv.tv_usec + tv.tv_sec * 1000000;
  return totalUsec;
}

Status InferShapeUtil::setArgShapeFromTensorShape(std::vector<Tensor> vecTensor, Graph *graph, const OpDef &sig,
                                                  ShapeRefiner &shapeRef) {
  REQUIRES_NOT_NULL(graph);
  int idx = 0;
  for (const OpDef::ArgDef &arg_def : sig.input_arg()) {
    for (Node *pNode : graph->nodes()) {
      REQUIRES_NOT_NULL(pNode);
      if (pNode->name() == arg_def.name()) {
        TF_RETURN_IF_ERROR(shapeRef.AddNode(pNode));  // here the arg node must add succ
        tensorflow::shape_inference::InferenceContext *pCxt = shapeRef.GetContext(pNode);
        if (pCxt == nullptr)  // this is a protect
        {
          return errors::Internal("The InferenceContext of node ", pNode->name(), " is null, add node failed.");
        }

        tensorflow::shape_inference::ShapeHandle shapeHandle;
        pCxt->MakeShapeFromTensorShape(vecTensor[idx].shape(), &shapeHandle);
        pCxt->set_output(0, shapeHandle);  // this arg has only one output
        idx++;
        break;  // next arg
      }
    }
  }

  return Status::OK();
}

Status InferShapeUtil::getSubGraphFromFunctionDef(const FunctionDef &func_def, Graph *graph) {
  LOG(INFO) << "The signature name of FunctionDef is " << func_def.signature().name() << ".";
  InstantiationResult result;
  AttrSlice attrs(&func_def.attr());
  TF_RETURN_IF_ERROR(InstantiateFunction(
      func_def, attrs, [](const string &op, const OpDef **sig) { return OpRegistry::Global()->LookUpOpDef(op, sig); },
      &result));

  GraphConstructorOptions opts;
  opts.allow_internal_ops = true;
  opts.expect_device_spec = false;
  TF_RETURN_IF_ERROR(ConvertNodeDefsToGraph(opts, result.nodes, graph));
  return Status::OK();
}

bool InferShapeUtil::IsInitializedGraph(Node *node) {
  Node *logical_not_node = nullptr;
  node->input_node(0, &logical_not_node);
  if (logical_not_node == nullptr) { return false; }

  if (logical_not_node->type_string() == "Reshape") {
    Node *reshape_node = logical_not_node;
    reshape_node->input_node(0, &logical_not_node);
    if (logical_not_node == nullptr) { return false; }
  }
  if (logical_not_node->type_string() != "LogicalNot") { return false; }

  Node *stack_node = nullptr;
  logical_not_node->input_node(0, &stack_node);
  if (stack_node == nullptr || stack_node->type_string() != "Pack") { return false; }

  Node *is_var_init_node = nullptr;
  stack_node->input_node(0, &is_var_init_node);
  if (is_var_init_node == nullptr) { return false; }

  if (is_var_init_node->type_string() == "VarIsInitializedOp"
      || is_var_init_node->type_string() == "IsVariableInitialized") {
    LOG(INFO) << "GEOP::IsInitializedGraph";
    return true;
  }

  return false;
}

Status InferShapeUtil::getInputShapesOfNode(ShapeRefiner &shapeRef, Node *pNode,
                                            std::vector<tensorflow::shape_inference::ShapeHandle> &inputShapeVec) {
  REQUIRES_NOT_NULL(pNode);
  for (const Edge *pEdge : pNode->in_edges()) {
    REQUIRES_NOT_NULL(pEdge);
    if (pEdge->IsControlEdge()) { continue; }

    Node *pNodeIn = pEdge->src();
    tensorflow::shape_inference::InferenceContext *pCxtIn = shapeRef.GetContext(pNodeIn);
    if (pCxtIn == nullptr) {
      return errors::Internal("Can't get context of the input ", pNodeIn->name(), " of the node ", pNode->name(), ".");
    }

    int iDstInput = pEdge->dst_input();
    inputShapeVec[iDstInput] = pCxtIn->output(pEdge->src_output());
  }

  return Status::OK();
}

void InferShapeUtil::setShapeOfEnterOP(ShapeRefiner &shapeRef, Node *pNode) {
  CHECK_NOT_NULL(pNode);
  tensorflow::shape_inference::InferenceContext *pCxt = shapeRef.GetContext(pNode);
  CHECK_NOT_NULL(pCxt);
  tensorflow::shape_inference::ShapeHandle shapeOutOne = pCxt->output(0);  // Enter has only one output
  if (pCxt->DebugString(shapeOutOne).find('?') == std::string::npos)       // Enter op has shape
  {
    return;
  }

  int iInputNums = pNode->num_inputs();  // Enter has only one input
  if (iInputNums != 1) {
    LOG(ERROR) << "Node " << pNode->name() << ", type is " << pNode->type_string()
               << ", must has only one input, but now=" << iInputNums;
    return;
  }
  std::vector<tensorflow::shape_inference::ShapeHandle> inputShapes(iInputNums);

  (void) getInputShapesOfNode(shapeRef, pNode, inputShapes);

  pCxt->set_output(0, inputShapes.at(0));  // Enter op can't be unknown shape.
}

void InferShapeUtil::setShapeOfMergeOP(ShapeRefiner &shapeRef, Node *pNode) {
  CHECK_NOT_NULL(pNode);
  tensorflow::shape_inference::InferenceContext *pCxt = shapeRef.GetContext(pNode);
  CHECK_NOT_NULL(pCxt);
  tensorflow::shape_inference::ShapeHandle shapeOutOne = pCxt->output(0);  // Set Ref/Merge first output
  if (pCxt->DebugString(shapeOutOne).find('?') == std::string::npos)       // Ref/Merge op has shape
  {
    return;
  }

  for (const Edge *e : pNode->in_edges()) {
    CHECK_NOT_NULL(e);
    if (e->IsControlEdge()) continue;
    if (e->dst_input() < 0) continue;

    if (e->src()->type_string() == "Enter" || e->src()->type_string() == "RefEnter") {
      Node *pNodeIn = e->src();
      tensorflow::shape_inference::InferenceContext *pCxtIn = shapeRef.GetContext(pNodeIn);
      if (pCxtIn == nullptr) {
        LOG(ERROR) << "Can't get context of the input " << pNodeIn->name() << " of the node " << pNode->name() << ".";
        return;
      }
      pCxt->set_output(0, pCxtIn->output(e->src_output()));
      return;
    }
  }
}

void InferShapeUtil::setShapeOfBroadcastGradientArgsOP(ShapeRefiner &shapeRef, Node *pNode) {
  CHECK_NOT_NULL(pNode);
  int iInputNums = pNode->num_inputs();
  std::vector<tensorflow::shape_inference::ShapeHandle> inputShapes(iInputNums);

  (void) getInputShapesOfNode(shapeRef, pNode, inputShapes);

  tensorflow::shape_inference::InferenceContext *pCxt = shapeRef.GetContext(pNode);
  CHECK_NOT_NULL(pCxt);
  int64 maxDimVal = pCxt->Value(pCxt->Dim(inputShapes.at(0), 0));
  int iMaxDimIndex = 0;
  for (int i = 1; i < iInputNums; i++) {
    const int64 secondValue = pCxt->Value(pCxt->Dim(inputShapes.at(i), 0));
    if (secondValue > maxDimVal) {
      iMaxDimIndex = i;
      maxDimVal = secondValue;
    }
  }

  std::vector<TensorShapeProto> shapeVec;
  int iOutputNums = pNode->num_outputs();
  for (int i = 0; i < iOutputNums; i++) {
    TensorShapeProto proto;
    if (pCxt->DebugString(pCxt->output(i)).find('?') != std::string::npos)  // the shape of this output has ?
    {
      pCxt->ShapeHandleToProto(inputShapes[iMaxDimIndex], &proto);
      LOG(INFO) << "Node name " << pNode->name() << " add attr shape " << pCxt->DebugString(inputShapes[iMaxDimIndex]);
    } else {
      pCxt->ShapeHandleToProto(pCxt->output(i), &proto);
    }
    shapeVec.push_back(proto);
  }

  pNode->AddAttr(KEY_SHAPE, gtl::ArraySlice<TensorShapeProto>(shapeVec));
}

void InferShapeUtil::setShapeOfReshapeOP(ShapeRefiner &shapeRef, Node *pNode) {
  CHECK_NOT_NULL(pNode);
  tensorflow::shape_inference::InferenceContext *pCxt = shapeRef.GetContext(pNode);
  CHECK_NOT_NULL(pCxt);
  if (pCxt->DebugString(pCxt->output(0)).find('?') == std::string::npos) { return; }

  std::vector<tensorflow::shape_inference::ShapeHandle> inShapes(pNode->num_inputs());
  (void) getInputShapesOfNode(shapeRef, pNode, inShapes);

  if (pCxt->DebugString(inShapes[0]).find('?') == std::string::npos) {
    TensorShapeProto proto;
    pCxt->ShapeHandleToProto(inShapes[0], &proto);
    pNode->AddAttr(KEY_SHAPE, proto);  // Reshape has only one output
    LOG(INFO) << "Node name " << pNode->name() << " add attr shape " << pCxt->DebugString(inShapes[0]);
  }
}

void InferShapeUtil::inferShapeOfGraph(const Graph *graph, ShapeRefiner &shapeRef, int iTime) {
  CHECK_NOT_NULL(graph);
  for (Node *pNode : graph->nodes()) {
    CHECK_NOT_NULL(pNode);
    if (pNode->type_string() == "NoOp" || shapeRef.GetContext(pNode) != nullptr) { continue; }

    Status addStatus = shapeRef.AddNode(pNode);
    if (!addStatus.ok()) {
      if (iTime != INFER_SHAPE_FIRST_TIME) {
        LOG(WARNING) << "AddNode failed, errormsg is " << addStatus.error_message() << ".";
      }
      continue;
    } else if (iTime == INFER_SHAPE_FIRST_TIME && pNode->type_string() == "Enter") {
      setShapeOfEnterOP(shapeRef, pNode);
    } else if ((iTime == INFER_SHAPE_FIRST_TIME)
               && ((pNode->type_string() == "Merge") || (pNode->type_string() == "RefMerge"))) {
      setShapeOfMergeOP(shapeRef, pNode);
    }
  }
}

void InferShapeUtil::printGraphShape(ShapeRefiner &shapeRef, Graph *graph) {
  CHECK_NOT_NULL(graph);
  shape_inference::InferenceContext *pCxt = nullptr;
  int iOutNums;
  for (Node *pNode : graph->nodes()) {
    CHECK_NOT_NULL(pNode);
    pCxt = shapeRef.GetContext(pNode);
    if (pCxt == nullptr) { continue; }
    iOutNums = pCxt->num_outputs();
    if (iOutNums <= 0) {
      LOG(INFO) << "Node " << pNode->name() << " has none outputs.";
      return;
    }
    for (int i = 0; i < iOutNums; i++) {
      tensorflow::shape_inference::ShapeHandle shape = pCxt->output(i);
      string strShape = pCxt->DebugString(shape);
      LOG(INFO) << "The shape of node " << pNode->name() << " output " << i << " is " << strShape;
    }
  }
}

Status InferShapeUtil::addShapeToAttr(ShapeRefiner &shapeRef, Node *pNode) {
  REQUIRES_NOT_NULL(pNode);
  shape_inference::InferenceContext *pCxt = shapeRef.GetContext(pNode);
  if (pCxt == nullptr) {
    LOG(WARNING) << "The InferenceContext of node " << pNode->name() << " is null.";
    return Status::OK();
  }

  int iOutNums = pCxt->num_outputs();
  if (iOutNums <= 0) { return Status::OK(); }

  AttrSlice attrList = pNode->attrs();
  if (attrList.Find(KEY_SHAPE) != nullptr) {
    LOG(INFO) << "Node " << pNode->name() << " already has omop_shape attribute.";
    return Status::OK();
  }

  std::vector<TensorShapeProto> shapeVec;
  for (int i = 0; i < iOutNums; i++) {
    tensorflow::shape_inference::ShapeHandle shape = pCxt->output(i);
    TensorShapeProto proto;
    pCxt->ShapeHandleToProto(shape, &proto);
    shapeVec.push_back(proto);

    string strShape = pCxt->DebugString(shape);
    if (strShape.find('?') != std::string::npos) {
      LOG(WARNING) << "The shape of node " << pNode->name() << " output " << i << " is " << strShape
                   << ", unknown shape.";

      auto identifier = NpuOpsIdentifier::GetInstance(false);
      if (identifier->IsPerformanceSensitive(pNode->type_string())) {
        return errors::Internal("Node ", pNode->name(), " output ", i, " shape is ", strShape, ", type is ",
                                pNode->type_string(), ", performance sensitive op shouldn't has unknown shape.");
      }
    }
  }

  pNode->AddAttr(KEY_SHAPE, gtl::ArraySlice<TensorShapeProto>(shapeVec));
  return Status::OK();
}

Status InferShapeUtil::InferShape(const std::vector<Tensor> &vecTensor, const FunctionLibraryDefinition *flib_def,
                                  const FunctionDef *func_def, Graph *graph) {
  (void)flib_def;
  REQUIRES_NOT_NULL(graph);
  REQUIRES_NOT_NULL(func_def);
  LOG(INFO) << "InferShapeUtil::InferShape";
  int iTensorNums = vecTensor.size();
  const OpDef &sig = func_def->signature();
  int iInputArgNums = sig.input_arg_size();
  if (iTensorNums < iInputArgNums) {
    return errors::Internal("Input tensor num ", iTensorNums, " is less than arg num ", iInputArgNums, ".");
  }

  TF_RETURN_IF_ERROR(getSubGraphFromFunctionDef(*func_def, graph));

  // Control flow loops in the graph; we have to break them.
  std::vector<EdgeInfo> NextIterationEdges;
  std::unordered_set<const Edge *> needRemoveEdges;
  for (Node *pNode : graph->nodes()) {
    REQUIRES_NOT_NULL(pNode);
    if ((pNode->type_string() != "Merge") && (pNode->type_string() != "RefMerge")) continue;

    needRemoveEdges.clear();
    for (const Edge *e : pNode->in_edges()) {
      REQUIRES_NOT_NULL(e);
      if (e->IsControlEdge()) continue;
      if (e->dst_input() < 0) continue;

      LOG(INFO) << "in_edges: " << e->src()->name() << " --> " << pNode->name();
      if ((e->src()->type_string() == "NextIteration") || (e->src()->type_string() == "RefNextIteration")) {
        EdgeInfo edgeInfo(e->src(), pNode, e->src_output(), e->dst_input());
        NextIterationEdges.push_back(edgeInfo);
        needRemoveEdges.insert(e);
      }
    }
    for (auto needRemoveEdge : needRemoveEdges) {
      graph->RemoveEdge(needRemoveEdge);  // Use Enter replace NextIteration.
    }
  }

  ShapeRefiner shapeRefinerSub(graph->versions(), graph->op_registry());
  shapeRefinerSub.set_require_shape_inference_fns(false);
  shapeRefinerSub.set_disable_constant_propagation(true);

  TF_RETURN_IF_ERROR(setArgShapeFromTensorShape(vecTensor, graph, sig, shapeRefinerSub));
  inferShapeOfGraph(graph, shapeRefinerSub, INFER_SHAPE_FIRST_TIME);
  inferShapeOfGraph(graph, shapeRefinerSub, INFER_SHAPE_OTHER_TIME);

  for (Node *pNode : graph->nodes()) { TF_RETURN_IF_ERROR(addShapeToAttr(shapeRefinerSub, pNode)); }

  for (auto &edgeInfo : NextIterationEdges) {
    graph->AddEdge(edgeInfo.src_, edgeInfo.src_output_, edgeInfo.dst_, edgeInfo.dst_input_);
  }

  LOG(INFO) << "InferShapeUtil::InferShape success";
  return Status::OK();
}
}  // namespace tensorflow
