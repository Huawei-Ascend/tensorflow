/**
 * @file aicore_util_attr_define.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * @brief attribute define
 *
 * @version 1.0
 *
 */
#ifndef INC_COMMON_UTILS_AI_CORE_COMMON_ATTR_DEFINE_H_
#define INC_COMMON_UTILS_AI_CORE_COMMON_ATTR_DEFINE_H_

#include <string>

namespace fe {
static const std::string SCOPE_ID_ATTR = "fusion_scope";

static const std::string FE_IMPLY_TYPE = "_fe_imply_type";

static const std::string PARENT_OP_TYPE = "parentOpType";

static const std::string ATTR_NAME_TASK_L2_FUSION_INFO_EXTEND_PTR =
    "task_l2_fusion_info_extend_content";

static const std::string ATTR_DATA_DUMP_REF = "_datadump_ref";

static const std::string ATTR_NAME_L2_FUSION_EXTEND_PTR =
    "l2_fusion_extend_content";

static const std::string L1_OPTIMIZED = "l1_optimized";

static const std::string L2_OPTIMIZED = "l2_optimized";

static const std::string OP_SLICE_INFO = "_op_slice_info";
} // namespace fe
#endif
