/**
 * @file aicore_param_calculator.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * @brief aicore param calculator
 *
 * @version 1.0
 *
 */

#ifndef AICORE_PARAM_CALCULATOR
#define AICORE_PARAM_CALCULATOR

#include "graph/node.h"
#include "graph_optimizer/graph_optimize_register_error_codes.h"

namespace fe {
class AICoreParamCalculator {
 public:
  AICoreParamCalculator();

  ~AICoreParamCalculator();

  Status CalcOpRunningParam(ge::Node &node);

};
}  // namespace fe
#endif  // AICORE_PARAM_CALCULATOR
