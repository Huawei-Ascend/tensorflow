/**
* Copyright (C) <2019>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
* Description: manage the relationship between tf session and ge session
*/

#ifndef TENSORFLOW_SESSION_MANAGER_H_
#define TENSORFLOW_SESSION_MANAGER_H_

#include <mutex>
#include <string>
#include <unordered_map>
#include "ge/ge_api.h"
#include "tensorflow/core/platform/mutex.h"
#include "tensorflow/core/framework/op_kernel.h"

// Sigleton class for manage the relationship between
// tf session and ge session
class SessionManager {
 public:
  static SessionManager& GetInstance();

  // Retrieves an already existing ge session to run the compute graph,
  // or create a ge session for future use.
  bool GetOrCreateGeSession(std::string& tf_session, ge::Session*& ge_session,
                            std::map<std::string, std::string>& sess_options);

  // Destory a ge session divided by tf session.
  void DestoryGeSession(std::string tf_session);

  // Whether a ge session exist.
  bool IsGeSessionExist();
 private:
  // Create a ge session to run the compute graph divided by tf session.
  bool CreateGeSession(std::string session, ge::Session*& ge_session,
                       std::map<std::string, std::string>& sess_options);
  // Print ge session options
  void PrintGeSessionOptions(std::map<std::string, std::string>& sess_options);
  // Mapping relationship between tf session and ge session.
  std::unordered_map<std::string, ge::Session*> ge_sessions_;
};
#endif
