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

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PATTERN_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PATTERN_H_
#include <map>
#include <string>
#include <vector>
#include "graph/debug/ge_attr_define.h"
#include "graph/utils/attr_utils.h"
#include "graph/utils/graph_utils.h"

namespace fe {
static const int TBE_FUSION_OP_NUM_MAX = 5;
static const int TBE_PATTERN_NUM_MAX = 5;
static const int TBE_PATTERN_NUM_NONE = 0;
static const int TBE_PATTERN_NUM_DEFAULT = 1;
static const int TBE_OUTPUT_BRANCH_DEFAULT = 0;
static const int TBE_OUTPUT_BRANCH_SINGLE = 1;
static const int TBE_OUTPUT_BRANCH_MULTI = 2;
static const int TBE_PATTERN_GROUPID_INVALID = -1;

enum SkipStatus { DISABLED = 0, AVAILABLE = 1, SKIPPED = 2 };

struct BufferFusionOpDesc {
  std::string descName;                       // description name
  std::vector<std::string> types;             // description type
  std::vector<BufferFusionOpDesc *> inputs;   // all input op
  std::vector<BufferFusionOpDesc *> outputs;  // all output op
  int64_t outBranchType;                      // out desc type, 1:single, 2: multi
  int64_t repeateMin;                         // opdesc min repeat num
  int64_t repeateMax;                         // opdesc max repeat num
  int64_t repeateCurr;                        // opdesc current repeat num
  bool matchStatus;
  int64_t groupId;  // record desc groupid, need one desc matched at least in
                    // the same group
  bool ignoreInputNum;
  bool ignoreOutputNum;
  // used for two connected op, first opdesc has optional multiple nodes and
  // ignoreOutputNum is true, second opdesc is same pattern type and
  // outBranchType is TBE_OUTPUT_BRANCH_MULTI
  std::map<int64_t, SkipStatus> multiOutputSkipStatus;
};
using BufferFusionMapping = std::map<const BufferFusionOpDesc *, std::vector<ge::NodePtr>>;
using BufferFusionMappings = std::vector<BufferFusionMapping>;

class BufferFusionPattern {
 public:
  explicit BufferFusionPattern(std::string name = "", int64_t opMaxCount = TBE_FUSION_OP_NUM_MAX);

  virtual ~BufferFusionPattern();

  BufferFusionPattern &AddOpDesc(const std::string &descName, const std::vector<std::string> &patterns,
                                 int64_t repeatMin = TBE_PATTERN_NUM_DEFAULT,
                                 int64_t repeatMax = TBE_PATTERN_NUM_DEFAULT,
                                 int64_t groupId = TBE_PATTERN_GROUPID_INVALID);

  BufferFusionPattern &SetOutputs(const std::string &descName, const std::vector<std::string> &patterns,
                                  int64_t relation = TBE_OUTPUT_BRANCH_SINGLE, bool ignoreInputNum = false,
                                  bool ignoreOutputNum = false);

  BufferFusionPattern &SetHead(const std::vector<std::string> &opPatterns);

  std::string GetName();
  int64_t GetOpMaxCount();
  std::vector<BufferFusionOpDesc *> GetOpDescs();
  bool GetOutputs(BufferFusionOpDesc *opDesc, std::vector<BufferFusionOpDesc *> &outputs, bool ignoreRepeat = false);
  std::vector<BufferFusionOpDesc *> GetHead();
  int64_t GetErrorCnt();
  void InitRepeatCurr(const BufferFusionPattern &pattern);

 private:
  BufferFusionOpDesc *GetOpDesc(const std::string &descName);
  void UpdateSkipStatus(BufferFusionOpDesc *opDesc);
  std::string name_;
  int64_t opMaxCount_;
  std::vector<BufferFusionOpDesc *> ops_;
  std::map<std::string, BufferFusionOpDesc *> opMap_;
  std::vector<BufferFusionOpDesc *> head_;
  int64_t errorCount_;
};
}  // namespace fe
#endif  // INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PATTERN_H_
