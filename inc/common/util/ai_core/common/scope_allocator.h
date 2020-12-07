/**
 * @file scope_allocator.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 *
 * @brief tbe fusion scope id allocator
 *
 * @version 1.0
 *
 */

#ifndef INC_COMMON_UTILS_AI_CORE_COMMON_SCOPE_ALLOCATOR_H_
#define INC_COMMON_UTILS_AI_CORE_COMMON_SCOPE_ALLOCATOR_H_

#include "graph/op_desc.h"

namespace fe {
class ScopeAllocator {
 public:
  ScopeAllocator();
  virtual ~ScopeAllocator();
  ScopeAllocator(const ScopeAllocator& in) = delete;
  ScopeAllocator& operator = (const ScopeAllocator& in) = delete;

 public:
  void Init();
  int64_t GetCurrentScopeId();
  int64_t AllocateScopeId(void);
  bool HasScopeAttr(ge::ConstOpDescPtr opdef);
  bool GetScopeAttr(ge::ConstOpDescPtr opdef, int64_t &scope_id);
  bool SetScopeAttr(ge::OpDescPtr opdef, int64_t scope_id);
  bool ResetScopeId(int64_t scope_id);
 private:
    int64_t scope_id;
};
}  // namespace fe
#endif
