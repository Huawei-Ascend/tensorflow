/**
 * @file fusion_pass_registry.h
 *
 * Copyright(C), 2017 - 2017, Huawei Tech. Co., Ltd. ALL RIGHTS RESERVED.
 *
 * @brief provide interface: GraphFusionByPass
 *
 * @author Huawei
 *
 * @version 1.0
 *
 */
#ifndef INC_REGISTER_GRAPH_OPTIMIZER_FUSION_PASS_REGISTRY_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_FUSION_PASS_REGISTRY_H_

#include "register/graph_optimizer/graph_fusion/graph_fusion_pass_base.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace fe {
class FusionPassRegistry {
public:
  using CreateFn = GraphPass *(*)();
  ~FusionPassRegistry();

  static FusionPassRegistry &GetInstance();

  void RegisterPass(const GraphFusionPassType &pass_type,
                    const std::string &pass_name, CreateFn create_fn);

  std::map<std::string, CreateFn>
  GetCreateFnByType(const GraphFusionPassType &pass_type);

private:
  FusionPassRegistry();
  class FusionPassRegistryImpl;
  std::unique_ptr<FusionPassRegistryImpl> impl_;
};

class FusionPassRegistrar {
public:
  FusionPassRegistrar(const GraphFusionPassType &pass_type,
                      const std::string &pass_name, GraphPass *(*create_fun)());
  ~FusionPassRegistrar() {}
};

#define REGISTER_PASS(pass_name, pass_type, pass_class)                        \
  REGISTER_PASS_UNIQ_HELPER(__COUNTER__, pass_name, pass_type, pass_class)

#define REGISTER_PASS_UNIQ_HELPER(ctr, pass_name, pass_type, pass_class)       \
  REGISTER_PASS_UNIQ(ctr, pass_name, pass_type, pass_class)

#define REGISTER_PASS_UNIQ(ctr, pass_name, pass_type, pass_class)              \
  static ::fe::FusionPassRegistrar register_fusion_pass##ctr                   \
      __attribute__((unused)) = ::fe::FusionPassRegistrar(                     \
          pass_type, pass_name, []() -> ::fe::GraphPass * {                    \
            return new (std::nothrow) pass_class();                            \
          })

} // namespace fe
#endif // INC_REGISTER_GRAPH_OPTIMIZER_FUSION_PASS_REGISTRY_H_
