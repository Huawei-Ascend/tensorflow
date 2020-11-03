/**
 * @file graph_comm.h
 *
 * Copyright(C), 2017 - 2017, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED.
 *
 * @brief graph builder
 *
 * @version 1.0
 *
 */

#ifndef INC_COMMON_UTILS_AI_CORE_COMMON_GRAPH_COMMON_H_
#define INC_COMMON_UTILS_AI_CORE_COMMON_GRAPH_COMMON_H_

#include "graph/compute_graph.h"
#include "common/aicore_util_types.h"
#include "register/graph_optimizer/graph_optimize_register_error_codes.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace fe {

using kScopeNodeMap_t = std::map<int64_t, std::vector<ge::NodePtr>>;
using kScopeNodePair_t = std::pair<int64_t, std::vector<ge::NodePtr>>;

class GraphCommImpl;
using GraphCommImplPtr = std::unique_ptr<GraphCommImpl>;

class GraphComm {
public:
  GraphComm(const string &engineName);
  virtual ~GraphComm();
  GraphComm(const GraphComm &in) = delete;
  GraphComm &operator=(const GraphComm &in) = delete;

  Status GetscopeNodeMap(ge::ComputeGraph &graph, kScopeNodeMap_t &fusionMap);

  Status CopyFusionOpNodes(vector<FusionDataFlow> &fusInputEdgeList,
                           vector<FusionDataFlow> &fusOutputEdgeList,
                           vector<ge::NodePtr> &fusNodelist,
                           ge::OpDescPtr fusionOpDesc,
                           ge::ComputeGraphPtr fusionGraph);

  Status CopyFusionOpEdges(ge::OpDescPtr fusionOpDesc,
                           ge::ComputeGraph &origGraph,
                           ge::ComputeGraphPtr fusionGraph);

  Status GetNodeDataFlowMap(
      const ge::NodePtr &fusNode,
      std::map<ge::NodePtr, std::map<ge::AnchorPtr, ge::AnchorPtr>>
          &fusionOpAnchorsMap,
      ge::kFusionDataFlowVec_t &fusDataflowList, const int &mapType);

  Status GetFusionNodeEdgeList(std::vector<ge::NodePtr> &fusNodelist,
                               std::vector<FusionDataFlow> &fusInputEdgeList,
                               std::vector<FusionDataFlow> &fusOutputEdgeList);
  void ClearFusionSrc();

  void ClearFusionDst();

  void
  AddFusionOutputSrc(const uint32_t &src_op_id, const ge::AnchorPtr &src_anchor,
                     const int32_t &fusion_src_index,
                     std::pair<string, ge::AnchorPtr> &node_dataindex_pair);

  void AddFusionInputSrc(const uint32_t &src_op_id,
                         const ge::AnchorPtr &src_anchor,
                         const int32_t &fusion_dst_index,
                         std::pair<string, ge::AnchorPtr> &node_dataindex_pair);

  void SaveFusionDst(const uint32_t &dst_op_id, ge::AnchorPtr dst_anchor);

  bool IsFusionDstExist(const uint32_t &dst_op_id,
                        const ge::AnchorPtr &dst_anchor);

  bool GetFusionSrc(const uint32_t &src_op_id, const ge::AnchorPtr &src_anchor,
                    int32_t &fusion_src_index, int32_t &fusion_dst_index);

  Status
  GetFusionNodeCtrlEdgeList(vector<ge::NodePtr> &fusNodelist,
                            vector<FusionDataFlow> &fusInputCtrlEdgeList,
                            vector<FusionDataFlow> &fusOutputCtrlEdgeList);

  Status MergeFusionNodeEdgeList(ge::NodePtr &fusNode,
                                 vector<ge::NodePtr> &fusNodelist,
                                 vector<FusionDataFlow> &fusInputEdgeList,
                                 vector<FusionDataFlow> &fusOutputEdgeList);

  Status MergeFusionNodeCtrlEdgeList(ge::NodePtr &fusNode,
                                     vector<ge::NodePtr> &fusNodelist,
                                     vector<FusionDataFlow> &fusInputEdgeList,
                                     vector<FusionDataFlow> &fusOutputEdgeList);

  string GetEngineName();

private:
  Status
  MergeFusionNodeInputEdgeList(ge::NodePtr fusNode,
                               std::vector<ge::NodePtr> &fusNodelist,
                               std::vector<FusionDataFlow> &fusInputEdgeList);
  Status
  MergeFusionNodeOutputEdgeList(ge::NodePtr fusNode,
                                std::vector<ge::NodePtr> &fusNodelist,
                                std::vector<FusionDataFlow> &fusOutputEdgeList);

  string engineName_;

  std::vector<FusionOpSrc> exist_fusion_src_list_;
  std::vector<FusionOpDst> exist_fusion_dst_list_;

  // std::vector<std::multimap<std::string, uint32_t>>
  ge::kFusionDataFlowVec_t fusion_input_dataflow_list_;

  // std::vector<std::multimap<std::string, ge::AnchorPtr>>
  ge::kFusionDataFlowVec_t fusion_output_dataflow_list_;

  GraphCommImplPtr graphCommImplPtr_;
};
} // namespace fe
#endif
