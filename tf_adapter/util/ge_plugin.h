/**
* Copyright (C) <2019>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
* Description: a plugin control GEInitialize and GEFinalize
*/
#ifndef TENSORFLOW_GE_PLUGIN_H_
#define TENSORFLOW_GE_PLUGIN_H_

#include <mutex>
#include <string>
#include <map>
#include "tensorflow/core/platform/types.h"
using tensorflow::int64;

// Sigleton class for manage the relationship between
// tf session and ge session
class GePlugin {
 public:
  static GePlugin* GetInstance();

  void Init(std::map<std::string, std::string>& init_options, bool is_global = false);

  void Finalize();

  bool IsGlobal();
 private:
  GePlugin();

  ~GePlugin();

  GePlugin(GePlugin &) = delete;

  int64 device_id_;
  bool isInit_;
  bool isGlobal_;
  std::map<std::string, std::string> init_options_;
  std::mutex mutex_;
};
#endif
