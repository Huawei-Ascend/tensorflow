/**
 * Copyright 2019-2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_FUSION_STATISTIC_RECORDER_H
#define INC_REGISTER_GRAPH_OPTIMIZER_FUSION_STATISTIC_RECORDER_H

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

namespace fe {

class FusionInfo {
 public:
  explicit FusionInfo(uint64_t sessionId = 0, std::string graphId = "", std::string passName = "",
                      int32_t matchTimes = 0, int32_t effectTimes = 0);

  virtual ~FusionInfo();

  void AddMatchTimes(int32_t matchTimes);

  void AddEffectTimes(int32_t effectTimes);

  int32_t GetMatchTimes();

  int32_t GetEffectTimes();

  std::string GetGraphId();

  std::string GetPassName();

  uint64_t GetSessionId();

  void SetMatchTimes(int32_t matchTimes);

  void SetEffectTimes(int32_t effectTimes);

 private:
  uint64_t sessionId_;
  std::string graphId_;
  std::string passName_;
  int32_t matchTimes_;
  int32_t effectTimes_;
};

using FusionStatisticMap = std::map<std::string, std::map<std::string, FusionInfo>>;

class FusionStatisticRecorder {
 public:
  FusionStatisticRecorder(const FusionStatisticRecorder &) = delete;

  FusionStatisticRecorder &operator=(const FusionStatisticRecorder &) = delete;

  static FusionStatisticRecorder &Instance();

  void UpdateGraphFusionMatchTimes(FusionInfo &fusionInfo);

  void UpdateGraphFusionEffectTimes(FusionInfo &fusionInfo);

  void UpdateBufferFusionMatchTimes(FusionInfo &fusionInfo);

  void UpdateBufferFusionEffectTimes(FusionInfo &fusionInfo);

  void GetAndClearFusionInfo(const std::string &sessionGraphId, std::map<std::string, FusionInfo> &graphFusionInfoMap,
                             std::map<std::string, FusionInfo> &bufferFusionInfoMap);

 private:
  FusionStatisticRecorder();
  virtual ~FusionStatisticRecorder();
  FusionStatisticMap graphFusionInfoMap_;
  FusionStatisticMap bufferFusionInfoMap_;
  void GetFusionInfo(const std::string &sessionGraphId, std::map<std::string, FusionInfo> &graphFusionInfoMap,
                     std::map<std::string, FusionInfo> &bufferFusionInfoMap);
  void ClearFusionInfo(std::string sessionGraphId);
  std::recursive_mutex mutex_;
};
}  // namespace fe

#endif  // INC_REGISTER_GRAPH_OPTIMIZER_FUSION_STATISTIC_RECORDER_H
