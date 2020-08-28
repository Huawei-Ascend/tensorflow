/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Copyright (C) 2019-2020. Huawei Technologies Co., Ltd. All rights reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_SESSION_MANAGER_H_
#define TENSORFLOW_SESSION_MANAGER_H_

#include "ge/ge_api.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/platform/mutex.h"
#include <mutex>
#include <string>
#include <unordered_map>

// Singleton class for manage the relationship between
// tf session and ge session
class SessionManager {
 public:
  static SessionManager &GetInstance();

  // Retrieves an already existing ge session to run the compute graph,
  // or create a ge session for future use.
  bool GetOrCreateGeSession(std::string &tf_session, ge::Session *&ge_session,
                            std::map<std::string, std::string> &sess_options);

  // Destroy a ge session divided by tf session.
  void DestroyGeSession(const std::string &tf_session);

  // Whether a ge session exist.
  bool IsGeSessionExist();

 private:
  // Create a ge session to run the compute graph divided by tf session.
  bool CreateGeSession(const std::string &session, ge::Session *&ge_session,
                       std::map<std::string, std::string> &sess_options);
  // Print ge session options
  void PrintGeSessionOptions(std::map<std::string, std::string> &sess_options);
  // Mapping relationship between tf session and ge session.
  std::unordered_map<std::string, ge::Session *> ge_sessions_;
};
#endif
