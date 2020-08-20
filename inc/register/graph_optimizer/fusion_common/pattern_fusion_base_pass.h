/**
 * @file pattern_fusion_base_pass.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * @brief define fusion pass based on pattern
 *
 * @author Huawei
 *
 * @version 1.0
 */

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_PATTERN_FUSION_BASE_PASS_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_PATTERN_FUSION_BASE_PASS_H_

#include "common/opskernel/ops_kernel_info_store.h"
#include "register/graph_optimizer/graph_fusion/fusion_pattern.h"
#include "register/graph_optimizer/graph_fusion/graph_fusion_pass_base.h"
#include "register/graph_optimizer/graph_fusion/graph_pass.h"
#include "register/graph_optimizer/graph_optimize_register_error_codes.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

using std::initializer_list;
using std::map;
using std::string;
using std::vector;

using namespace std;

namespace fe {
using OpsKernelInfoStorePtr = std::shared_ptr<ge::OpsKernelInfoStore>;
class PatternFusionBasePassImpl;
using PatternFusionBasePassImplPtr = std::shared_ptr<PatternFusionBasePassImpl>;

/** Pass based on pattern
 * @ingroup FUSION_PASS_GROUP
 * @note New virtual methods should be append at the end of this class
 */
class PatternFusionBasePass : public GraphPass {
public:
  using OpDesc = FusionPattern::OpDesc;
  using Mapping = map<const std::shared_ptr<OpDesc>, vector<ge::NodePtr>>;
  using Mappings = vector<Mapping>;
  std::map<ge::InDataAnchorPtr, ge::OutDataAnchorPtr> originOpAnchorsMap_;

  PatternFusionBasePass();
  virtual ~PatternFusionBasePass();

  /** execute pass
   *
   * @param [in] graph, the graph waiting for pass level optimization
   * @return SUCCESS, successfully optimized the graph by the pass
   * @return NOT_CHANGED, the graph did not change
   * @return FAILED, fail to modify graph
   */
  Status Run(ge::ComputeGraph &graph) override;

  /** execute pass
   *
   * @param [in] graph, the graph waiting for pass level optimization
   * @param [opsKernelInfoStorePtr, OP info kernel instance
   * @return SUCCESS, successfully optimized the graph by the pass
   * @return NOT_CHANGED, the graph did not change
   * @return FAILED, fail to modify graph
   */
  virtual Status Run(ge::ComputeGraph &graph,
                     OpsKernelInfoStorePtr opsKernelInfoStorePtr);

protected:
  virtual vector<FusionPattern *> DefinePatterns() = 0;
  virtual Status Fusion(ge::ComputeGraph &graph, Mapping &mapping,
                        vector<ge::NodePtr> &newNodes) = 0;

  std::vector<ge::NodePtr> GetNodesFromMapping(const Mapping &mapping);
  ge::NodePtr GetNodeFromMapping(const string &id, const Mapping &mapping);

  void RecordOutputAnchorMap(ge::NodePtr outputNode);
  Status SetDataDumpAttr(vector<ge::NodePtr> &originalNodes,
                         vector<ge::NodePtr> &fusNodes);

  bool CheckOpSupported(const ge::OpDescPtr &opDescPtr);

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

#endif // INC_REGISTER_GRAPH_OPTIMIZER_PATTERN_FUSION_BASE_PASS_H_
