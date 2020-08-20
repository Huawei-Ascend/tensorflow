/**
 * @file buffer_fusion_pass_registry.h
 *
 * Copyright(C), 2017 - 2017, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED.
 *
 * @brief provide interface: BufferFusionByPass
 *
 * @author Huawei
 *
 * @version 1.0
 *
 */
#ifndef INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PASS_REGISTRY_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PASS_REGISTRY_H_
#include "register/graph_optimizer/buffer_fusion/buffer_fusion_pass_base.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace fe {
class BufferFusionPassRegistry {
public:
  using CreateFn = BufferFusionPassBase *(*)();
  ~BufferFusionPassRegistry();

  static BufferFusionPassRegistry &GetInstance();

  void RegisterPass(const BufferFusionPassType &pass_type,
                    const std::string &pass_name, CreateFn create_fn);

  std::map<std::string, CreateFn>
  GetCreateFnByType(const BufferFusionPassType &pass_type);

private:
  BufferFusionPassRegistry();
  class BufferFusionPassRegistryImpl;
  std::unique_ptr<BufferFusionPassRegistryImpl> impl_;
};

class BufferFusionPassRegistrar {
public:
  BufferFusionPassRegistrar(const BufferFusionPassType &pass_type,
                            const std::string &pass_name,
                            BufferFusionPassBase *(*create_fun)());
  ~BufferFusionPassRegistrar() {}
};

#define REGISTER_BUFFER_FUSION_PASS(pass_name, pass_type, pass_class)          \
  REGISTER_BUFFER_FUSION_PASS_UNIQ_HELPER(__COUNTER__, pass_name, pass_type,   \
                                          pass_class)

#define REGISTER_BUFFER_FUSION_PASS_UNIQ_HELPER(ctr, pass_name, pass_type,     \
                                                pass_class)                    \
  REGISTER_BUFFER_FUSION_PASS_UNIQ(ctr, pass_name, pass_type, pass_class)

#define REGISTER_BUFFER_FUSION_PASS_UNIQ(ctr, pass_name, pass_type,            \
                                         pass_class)                           \
  static ::fe::BufferFusionPassRegistrar register_buffer_fusion_pass##ctr      \
      __attribute__((unused)) = ::fe::BufferFusionPassRegistrar(               \
          pass_type, pass_name, []() -> ::fe::BufferFusionPassBase * {         \
            return new (std::nothrow) pass_class();                            \
          })

} // namespace fe
#endif // INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PASS_REGISTRY_H_
