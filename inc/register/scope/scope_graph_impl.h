/**
 * Copyright 2020 Huawei Technologies Co., Ltd
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

#ifndef REGISTER_SCOPE_SCOPE_GRAPH_IMPL_H_
#define REGISTER_SCOPE_SCOPE_GRAPH_IMPL_H_

#include "external/register/scope/scope_fusion_pass_register.h"
#include "proto/tensorflow/node_def.pb.h"
#include "proto/tensorflow/graph.pb.h"

namespace ge {
class  Scope::ScopeImpl {
 public:
  explicit ScopeImpl(const std::string &name, const std::string &sub_type = "", Scope *father_scope = nullptr)
      : name_(name), sub_type_(sub_type), father_scope_(father_scope) {}
  ~ScopeImpl();

  std::string Name() const { return name_; }
  std::string SubType() const { return sub_type_; }
  void SetSubType(const std::string &sub_type) { sub_type_ = sub_type; }
  void ClearTypeAndSubType();
  void AddNode(ge::OperatorPtr node_def);
  std::vector<ge::OperatorPtr> Nodes() const { return nodes_; }
  std::vector<ge::OperatorPtr> AllNodes() const;
  std::map<std::string, ge::OperatorPtr> AllNodesMap() const;
  void AddSubScope(Scope *scope) { sub_scopes_[scope->Name()] = scope; }
  Scope *GetSubScope(const std::string &scope_name) const;
  std::map<std::string, Scope *> GetSubScopes() const { return sub_scopes_; }
  std::vector<Scope *> GetAllSubScopes() const;
  int32_t GetOpTypeNum(const std::string &op_type) const;
  void OpsNumInc(const std::string &op_type);
  std::string LastName() const;
  const Scope *GetFatherScope() const { return father_scope_; }
  // trim scope_index
  static std::string TrimScopeIndex(const std::string &scope_name);

 private:
  std::string name_;
  std::string sub_type_;
  Scope *father_scope_;
  std::map<std::string, int32_t> op_nums_;
  std::map<std::string, Scope *> sub_scopes_;
  std::vector<ge::OperatorPtr> nodes_;
};

class FusionScopesResult::FusionScopesResultImpl {
 public:
  FusionScopesResultImpl() {}
  ~FusionScopesResultImpl() {};
  void SetName(const std::string &name) { name_ = name; }
  void SetType(const std::string &type) { type_ = type; }
  void SetDescription(const std::string &description) { description_ = description; }
  std::string Name() const { return name_; }
  std::string Type() const { return type_; }
  std::string Description() const { return description_; }
  void AddNodes(std::vector<ge::OperatorPtr> nodes);
  std::vector<ge::OperatorPtr> Nodes() const { return nodes_; }
  void AddScopes(const std::vector<Scope *> &scopes) { scopes_.insert(scopes_.end(), scopes.begin(), scopes.end()); }
  std::vector<Scope *> Scopes() const { return scopes_; }
  std::map<std::string, std::vector<int32_t>> GetInputs() const { return inputs_; }
  std::map<std::string, std::vector<int32_t>> GetOutputs() const { return outputs_; }
  void InsertInputs(const std::string &inner_op_name, const std::vector<int32_t> &index_map);
  void InsertOutputs(const std::string &inner_op_name, const std::vector<int32_t> &index_map);
  bool FindNodes(const std::string &node_name) const;
  bool FindScopes(const std::string &scope_name) const;

 private:
  std::string name_;
  std::string type_;
  std::string description_;
  std::vector<Scope *> scopes_;
  std::vector<ge::OperatorPtr> nodes_;
  std::map<std::string, std::vector<int32_t>> inputs_;
  std::map<std::string, std::vector<int32_t>> outputs_;
};

class ScopeTree::ScopeTreeImpl {
 public:
  ScopeTreeImpl() : root_(nullptr) {}
  ScopeTreeImpl(const ScopeTreeImpl &) = delete;
  ScopeTreeImpl &operator=(const ScopeTreeImpl &) = delete;
  Status Init();
  ~ScopeTreeImpl();

  void AddNodeToScope(ge::OperatorPtr node_def);
  std::vector<Scope *> GetAllScopes() const { return scopes_; }
  const Scope *Root() const { return root_; }

 private:
  std::vector<std::string> SplitNodeName(const std::string &node_name, char delim) const;
  Scope *root_;
  std::vector<Scope *> scopes_;
};

struct ScopeFusionOpInfo {
  std::string node_name;
  std::string fusion_node_name;
  std::string fusion_op_type;
  std::string description;
  bool scope_pass = true;
};

class ScopeGraph::ScopeGraphImpl {
 public:
  ScopeGraphImpl() : scope_tree_(nullptr) {}
  ScopeGraphImpl(const ScopeGraphImpl &) = delete;
  ScopeGraphImpl &operator=(const ScopeGraphImpl &) = delete;
  Status Init();
  ~ScopeGraphImpl();

  const ScopeTree *GetScopeTree() const { return scope_tree_; }
  void BuildScopeGraph(domi::tensorflow::GraphDef *graph_def);
  void AddFusionScopesResult(FusionScopesResult *result);
  std::map<std::string, FusionScopesResult *> FusionScopesResults() const { return fusion_results_; }
  FusionScopesResult *GetFusionScopesResults(const domi::tensorflow::NodeDef *node_def) const;
  std::map<std::string, ge::OperatorPtr> GetNodesMap() const { return nodes_map_; }
  bool IsFusionOpChild(const std::string &node_name, std::vector<ScopeFusionOpInfo> &info_list);
  bool FusionOpChildIgnore(const ScopeFusionOpInfo &info);
  bool IsFusionOp(const domi::tensorflow::NodeDef *node_def);
  Status GetInputOrOutputIndex(const ScopeFusionOpInfo &info, int32_t old_index, bool input, int32_t &new_index);

 private:
  std::vector<int32_t> GetFusionResultInputOrOutput(const ScopeFusionOpInfo &info, bool input);  // input:true,output:false
  void CheckScopesResult(FusionScopesResult *fusion_node);
  std::map<std::string, FusionScopesResult *> fusion_results_;
  std::map<std::string, ge::OperatorPtr> nodes_map_;
  ScopeTree *scope_tree_;
};
}  // namespace ge
#endif  // REGISTER_SCOPE_SCOPE_GRAPH_IMPL_H_