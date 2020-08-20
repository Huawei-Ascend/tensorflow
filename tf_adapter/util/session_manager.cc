/**
* Copyright (C) <2019>  <Huawei Technologies Co., Ltd.>. All Rights Reserved.
* Description: manage the relationship between tf session and ge session
*/

#include "tf_adapter/util/session_manager.h"

#include "tf_adapter/common/common.h"
#include "tf_adapter/util/npu_attrs.h"

using namespace tensorflow;
SessionManager& SessionManager::GetInstance() {
  static SessionManager instance;
  return instance;
}

// Returns True if get ge session success.
bool SessionManager::GetOrCreateGeSession(std::string& tf_session,
                                          ge::Session*& ge_session,
                                          std::map<std::string, std::string>& sess_options) {
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

// destory ge session.
void SessionManager::DestoryGeSession(std::string tf_session) {
  if (tf_session.empty()) {
    LOG(ERROR) << "tf session is empty, can not destory ge session.";
  }
  auto it = ge_sessions_.find(tf_session);
  if (it != ge_sessions_.end()) {
    if (it->second != nullptr) {
      LOG(INFO) << "find ge session connect with tf session " << tf_session;
      delete it->second;
      it->second = nullptr;
    }
    ge_sessions_.erase(it);
    LOG(INFO) << "destory ge session connect with tf session "
              << tf_session << " success.";
  }
}

// Returns True if create ge session success.
bool SessionManager::CreateGeSession(std::string tf_session, ge::Session*& ge_session,
    std::map<std::string, std::string>& sess_options) {
  // hcom parallel
  LOG(INFO) << "[GEOP] hcom_parallel :" << sess_options[ge::HCOM_PARALLEL];

  // stream max parallel num
  LOG(INFO) << "[GEOP] stream_max_parallel_num :" << sess_options[ge::STREAM_MAX_PARALLEL_NUM];
  ge_session = new(std::nothrow) ge::Session(sess_options);
  if (ge_session == nullptr) {
    LOG(ERROR) << "tf session " << tf_session << " create ge session failed.";
    return false;
  }
  ge_sessions_.insert(std::make_pair(tf_session, ge_session));
  return true;
}

// Returns True if any ge session exist.
bool SessionManager::IsGeSessionExist() {
  return !ge_sessions_.empty();
}

void SessionManager::PrintGeSessionOptions(std::map<std::string, std::string>& sess_options) {
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

  // reuse memory env
  const char* disable_reuse_memory = std::getenv("DISABLE_REUSE_MEMORY");
  if (disable_reuse_memory == nullptr) {
    disable_reuse_memory = "0";
    LOG(WARNING) << "[GEOP] can not get DISABLE_REUSE_MEMORY in env, set to default 0";
  }
  sess_options["ge.exec.disableReuseMemory"] = disable_reuse_memory;

  // dump configuration
  string dump_step = sess_options[ge::OPTION_EXEC_DUMP_STEP];
  LOG(INFO) << "[GEOP] enable_dump :" << sess_options[ge::OPTION_EXEC_ENABLE_DUMP]
            << ", dump_path :" << sess_options[ge::OPTION_EXEC_DUMP_PATH]
            << ", dump_step :" << (dump_step == "" ? "NA" : dump_step)
            << ", dump_mode :" << sess_options[ge::OPTION_EXEC_DUMP_MODE]
            << ", enable_dump_enable :" << sess_options[ge::OPTION_EXEC_ENABLE_DUMP_DEBUG]
            << ", dump_debug_mode :" << sess_options[ge::OPTION_EXEC_DUMP_DEBUG_MODE];
}