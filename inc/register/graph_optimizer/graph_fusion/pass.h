/**
 * @file pass.h
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * @brief define pass
 *
 * @author Huawei
 *
 * @version 1.0
 */
/** @defgroup FUSION_PASS_GROUP Fusion Pass Interface */

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_PASS_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_PASS_H_

#include "graph/compute_graph.h"
#include "register/graph_optimizer/graph_optimize_register_error_codes.h"

namespace fe {

/** fusion pass
 * @ingroup GRAPH_PASS_GROUP
 * network level pass
 */
template <typename T> class Pass {
public:
  virtual ~Pass() {}

  /** execute pass
   *
   * @param [in] graph, the graph waiting for pass level optimization
   * @return SUCCESS, successfully optimized the graph by the pass
   * @return NOT_CHANGED, the graph did not change
   * @return FAILED, fail to modify graph
   */
  virtual Status Run(ge::ComputeGraph &graph) = 0;

  void SetName(const string &name) { name_ = name; }

  string GetName() { return name_; }

private:
  string name_;
};

} // namespace fe

#endif // INC_REGISTER_GRAPH_OPTIMIZER_PASS_H_
