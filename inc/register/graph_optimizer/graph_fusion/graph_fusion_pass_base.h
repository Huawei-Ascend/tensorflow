/**
 * @file custom_pattern_fusion_base_pass.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * @brief custom_pattern_fusion_base_pass
 *
 * @author Huawei
 *
 * @version 1.0
 *
 */

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_FUSION_PASS_BASE_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_FUSION_PASS_BASE_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "register/graph_optimizer/graph_fusion/fusion_pattern.h"
#include "register/graph_optimizer/graph_fusion/graph_pass.h"

using std::initializer_list;
using std::map;
using std::string;
using std::vector;

using namespace std;

namespace fe {
enum GraphFusionPassType {
  BUILT_IN_GRAPH_PASS = 0,
  BUILT_IN_VECTOR_CORE_GRAPH_PASS,
  CUSTOM_AI_CORE_GRAPH_PASS,
  CUSTOM_VECTOR_CORE_GRAPH_PASS,
  SECOND_ROUND_BUILT_IN_GRAPH_PASS,
  GRAPH_FUSION_PASS_TYPE_RESERVED,
};
class PatternFusionBasePassImpl;
using PatternFusionBasePassImplPtr = std::shared_ptr<PatternFusionBasePassImpl>;

/** Pass based on pattern
 * @ingroup FUSION_PASS_GROUP
 * @note New virtual methods should be append at the end of this class
 */
class GraphFusionPassBase : public GraphPass {
public:
  using OpDesc = FusionPattern::OpDesc;
  using Mapping = map<const std::shared_ptr<OpDesc>, vector<ge::NodePtr>>;
  using Mappings = vector<Mapping>;

  GraphFusionPassBase();
  virtual ~GraphFusionPassBase();

  /** execute pass
   *
   * @param [in] graph, the graph waiting for pass level optimization
   * @return SUCCESS, successfully optimized the graph by the pass
   * @return NOT_CHANGED, the graph did not change
   * @return FAILED, fail to modify graph
   */
  Status Run(ge::ComputeGraph &graph) override;

protected:
  /** define pattern
   *
   * @return NA
   */
  virtual vector<FusionPattern *> DefinePatterns() = 0;

  /** do fusion according to nodes matched
   *
   * @param graph the graph waiting for pass level optimization
   * @param newNodes fusion result node(s)
   * @return SUCCESS, successfully optimized the graph by the pass
   * @return NOT_CHANGED, the graph did not change
   * @return FAILED, fail to modify graph
   */
  virtual Status Fusion(ge::ComputeGraph &graph, Mapping &mapping,
                        vector<ge::NodePtr> &newNodes) = 0; // lint !e148

  /** get nodes from matched result
   *
   * @param mapping match result
   * @return nodes result
   */
  static ge::NodePtr GetNodeFromMapping(const string &id,
                                        const Mapping &mapping);

private:
  /** match all nodes in graph according to pattern
   *
   * @param pattern fusion pattern defined
   * @param mappings match result
   * @return SUCCESS, successfully add edge
   * @return FAILED, fail
   */
  bool MatchAll(ge::ComputeGraph &graph, const FusionPattern &pattern,
                Mappings &mappings);

  Status RunOnePattern(ge::ComputeGraph &graph, const FusionPattern &pattern,
                       bool &changed); // lint !e148

  /** Internal implement class ptr */
  std::shared_ptr<PatternFusionBasePassImpl> patternFusionBasePassImplPtr_;
};

} // namespace fe

#endif // INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_FUSION_PASS_BASE_H_
