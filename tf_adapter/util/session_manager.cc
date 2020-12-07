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

#include "tf_adapter/util/session_manager.h"
#include "tf_adapter/util/npu_attrs.h"

using namespace tensorflow;
SessionManager &SessionManager::GetInstance() {
  static SessionManager instance;
  return instance;
}

// Returns True if get ge session success.
bool SessionManager::GetOrCreateGeSession(std::string &tf_session, ge::Session *&ge_session,
                                          std::map<std::string, std::string> &sess_options) {
  // find valid tf session handle
  if (tf_session.empty()) {
    LOG(ERROR) << "tf session is empty, get ge session failed.";
    return false;
  }

  // find valid ge session
  auto it = ge_sessions_.find(tf_session);
  if (it != ge_sessions_.end()) {
    ge_session = it->second;
    LOG(INFO) << "tf session " << tf_session << " get ge session success.";
    return true;
  }

  PrintGeSessionOptions(sess_options);
  bool ret = SessionManager::CreateGeSession(tf_session, ge_session, sess_options);
  if (!ret) {
    LOG(ERROR) << "tf session " << tf_session << " create ge session failed.";
    return false;
  }
  return true;
}

// destroy ge session.
void SessionManager::DestroyGeSession(const std::string &tf_session) {
  if (tf_session.empty()) { LOG(ERROR) << "tf session is empty, can not destroy ge session."; }
  auto it = ge_sessions_.find(tf_session);
  if (it != ge_sessions_.end()) {
    if (it->second != nullptr) {
      LOG(INFO) << "find ge session connect with tf session " << tf_session;
      delete it->second;
      it->second = nullptr;
    }
    ge_sessions_.erase(it);
    LOG(INFO) << "destroy ge session connect with tf session " << tf_session << " success.";
  }
}

// Returns True if create ge session success.
bool SessionManager::CreateGeSession(const std::string &tf_session, ge::Session *&ge_session,
                                     std::map<std::string, std::string> &sess_options) {
  // hcom parallel
  LOG(INFO) << "[GEOP] hcom_parallel :" << sess_options[ge::HCOM_PARALLEL];

  // stream max parallel num
  LOG(INFO) << "[GEOP] stream_max_parallel_num :" << sess_options[ge::STREAM_MAX_PARALLEL_NUM];
  ge_session = new (std::nothrow) ge::Session(sess_options);
  if (ge_session == nullptr) {
    LOG(ERROR) << "tf session " << tf_session << " create ge session failed.";
    return false;
  }
  ge_sessions_.insert(std::make_pair(tf_session, ge_session));
  return true;
}

// Returns True if any ge session exist.
bool SessionManager::IsGeSessionExist() { return !ge_sessions_.empty(); }

void SessionManager::PrintGeSessionOptions(std::map<std::string, std::string> &sess_options) {
  // variable acceleration configuration
  LOG(INFO) << "[GEOP] variable_acceleration :" << sess_options["ge.exec.variable_acc"];
  // hcom parallel
  LOG(INFO) << "[GEOP] hcom_parallel :" << sess_options[ge::HCOM_PARALLEL];

  // stream max parallel num
  LOG(INFO) << "[GEOP] stream_max_parallel_num :" << sess_options[ge::STREAM_MAX_PARALLEL_NUM];

  // graph memory configuration
  if (!sess_options[ge::GRAPH_MEMORY_MAX_SIZE].empty()) {
    LOG(INFO) << "[GEOP] set graph_memory_max_size: " << sess_options[ge::GRAPH_MEMORY_MAX_SIZE];
  } else {
    sess_options.erase(ge::GRAPH_MEMORY_MAX_SIZE);
  }

  // variable memory configuration
  if (!sess_options[ge::VARIABLE_MEMORY_MAX_SIZE].empty()) {
    LOG(INFO) << "[GEOP] set variable_memory_max_size: " << sess_options[ge::VARIABLE_MEMORY_MAX_SIZE];
  } else {
    sess_options.erase(ge::VARIABLE_MEMORY_MAX_SIZE);
  }

  LOG(INFO) << "[GEOP] op_select_implmode : " << sess_options[ge::OP_SELECT_IMPL_MODE];

  LOG(INFO) << "[GEOP] optypelist_for_implmode : " << sess_options[ge::OPTYPELIST_FOR_IMPLMODE];

  // reuse memory env
  const char *disable_reuse_memory = std::getenv("DISABLE_REUSE_MEMORY");
  if (disable_reuse_memory == nullptr) {
    disable_reuse_memory = "0";
    LOG(WARNING) << "[GEOP] can not get DISABLE_REUSE_MEMORY in env, set to default 0";
  }
  sess_options["ge.exec.disableReuseMemory"] = disable_reuse_memory;

  // dump configuration
  string dump_step = sess_options[ge::OPTION_EXEC_DUMP_STEP];
  LOG(INFO) << "[GEOP] enable_dump :" << sess_options[ge::OPTION_EXEC_ENABLE_DUMP]
            << ", dump_path :" << sess_options[ge::OPTION_EXEC_DUMP_PATH]
            << ", dump_step :" << (dump_step.empty() ? "NA" : dump_step)
            << ", dump_mode :" << sess_options[ge::OPTION_EXEC_DUMP_MODE]
            << ", enable_dump_debug :" << sess_options[ge::OPTION_EXEC_ENABLE_DUMP_DEBUG]
            << ", dump_debug_mode :" << sess_options[ge::OPTION_EXEC_DUMP_DEBUG_MODE];
}