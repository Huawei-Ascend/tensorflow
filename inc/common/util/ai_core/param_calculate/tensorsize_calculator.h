/**
 * @file tensorsize_calculator.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * @brief provide the capability of calculating
 * workspace and input/output size
 *
 * @version 1.0
 *
 */
#ifndef TENSORSIZE_CALCULATOR_H
#define TENSORSIZE_CALCULATOR_H

#include "graph_optimizer/graph_optimize_register_error_codes.h"

#include <map>
#include <string>
#include "graph/compute_graph.h"
#include "graph/op_desc.h"

namespace fe {
class TensorSizeCalculator {
 public:
  /**
   * Calculate the tensor size of input and output of each opdesc
   * @param opDesc opdesc object
   * @param opImplType op impl type
   * @return status SUCCESS or FAILED
   */
  static Status CalculateOpTensorSize(ge::OpDesc &opDesc);

 private:
  static Status CalcInputOpTensorSize(ge::OpDesc &opDesc,
                                      int32_t &outputRealCalcFlag);

  static Status CalcOutputOpTensorSize(ge::OpDesc &opDesc,
                                       int32_t &outputRealCalcFlag);
};
}  // namespace fe

#endif  // TENSORSIZE_CALCULATOR_H
