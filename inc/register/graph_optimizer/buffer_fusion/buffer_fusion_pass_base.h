#ifndef INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PASS_BASE_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PASS_BASE_H_

#include "register/graph_optimizer/buffer_fusion/buffer_fusion_constant.h"
#include "register/graph_optimizer/buffer_fusion/buffer_fusion_pattern.h"
#include "register/graph_optimizer/graph_optimize_register_error_codes.h"
#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace fe {
enum BufferFusionPassType {
  BUILT_IN_AI_CORE_BUFFER_FUSION_PASS,
  BUILT_IN_VECTOR_CORE_BUFFER_FUSION_PASS,
  CUSTOM_AI_CORE_BUFFER_FUSION_PASS,
  CUSTOM_VECTOR_CORE_BUFFER_FUSION_PASS,
  BUFFER_FUSION_PASS_TYPE_RESERVED
};
class BufferFusionPassBase {
public:
  explicit BufferFusionPassBase();
  virtual ~BufferFusionPassBase();
  virtual std::vector<BufferFusionPattern *> DefinePatterns() = 0;
  virtual Status GetFusionNodes(const BufferFusionMapping &mapping,
                                vector<ge::NodePtr> &fusionNodes);
  std::vector<ge::NodePtr> GetMatchedNodes(const BufferFusionMapping &mapping);
  std::vector<ge::NodePtr>
  GetMatchedNodesByDescName(const std::string &descName,
                            const BufferFusionMapping &mapping);
  ge::NodePtr GetMatchedHeadNode(const std::vector<ge::NodePtr> &matchedNodes);
};

} // namespace fe

#endif // INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PASS_BASE_H_
