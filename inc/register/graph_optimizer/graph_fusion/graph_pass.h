/**
 * @file graph_pass.h
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * @brief define graph pass, which provides two interface: 1. run pass;
 * 2. record op names before fusion
 *
 * @author Huawei
 *
 * @version 1.0
 */

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_PASS_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_PASS_H_

#include "register/graph_optimizer/graph_fusion/pass.h"
#include <string>

namespace fe {

/** graph pass
 * @ingroup GRAPH_PASS_GROUP
 * graph level pass
 */
class GraphPass : public Pass<ge::ComputeGraph> {
public:
  /** execute pass
   *
   * @param [in] graph, the graph waiting for pass level optimization
   * @return SUCCESS, successfully optimized the graph by the pass
   * @return NOT_CHANGED, the graph did not change
   * @return FAILED, fail to modify graph
   */
  virtual Status Run(ge::ComputeGraph &graph) = 0;
};

} // namespace fe

#endif // INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_PASS_H_
