#ifndef INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PATTERN_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PATTERN_H_
#include "graph/debug/ge_attr_define.h"
#include "graph/utils/attr_utils.h"
#include "graph/utils/graph_utils.h"
#include <map>
#include <string>
#include <vector>

namespace fe {
static const int TBE_FUSION_OP_NUM_MAX = 5;
static const int TBE_PATTERN_NUM_MAX = 5;
static const int TBE_PATTERN_NUM_NONE = 0;
static const int TBE_PATTERN_NUM_DEFAULT = 1;
static const int TBE_OUTPUT_BRANCH_SINGLE = 1;
static const int TBE_OUTPUT_BRANCH_MULTI = 2;
static const int TBE_PATTERN_GROUPID_INVALID = -1;

struct BufferFusionOpDesc {
  std::string descName;                      // description name
  std::vector<std::string> types;            // description type
  std::vector<BufferFusionOpDesc *> inputs;  // all input op
  std::vector<BufferFusionOpDesc *> outputs; // all output op
  int64_t outBranchType; // out desc type, 1:single, 2: multi
  int64_t repeateMin;    // opdesc min repeat num
  int64_t repeateMax;    // opdesc max repeat num
  int64_t repeateCurr;   // opdesc current repeat num
  bool matchStatus;
  int64_t groupId; // record desc groupid, need one desc matched at least in
                   // the same group
};
using BufferFusionMapping =
    std::map<const BufferFusionOpDesc *, std::vector<ge::NodePtr>>;
using BufferFusionMappings = std::vector<BufferFusionMapping>;

class BufferFusionPattern {
public:
  explicit BufferFusionPattern(std::string name = "",
                               int64_t opMaxCount = TBE_FUSION_OP_NUM_MAX);

  virtual ~BufferFusionPattern();

  BufferFusionPattern &AddOpDesc(const std::string &descName,
                                 const std::vector<std::string> &patterns,
                                 int64_t repeatMin = TBE_PATTERN_NUM_DEFAULT,
                                 int64_t repeatMax = TBE_PATTERN_NUM_DEFAULT,
                                 int64_t groupId = TBE_PATTERN_GROUPID_INVALID);

  BufferFusionPattern &SetOutputs(const std::string &descName,
                                  const std::vector<std::string> &patterns,
                                  int64_t relation = TBE_OUTPUT_BRANCH_SINGLE);

  BufferFusionPattern &SetHead(const std::vector<std::string> &opPatterns);

  std::string GetName();
  int64_t GetOpMaxCount();
  std::vector<BufferFusionOpDesc *> GetOpDescs();
  bool GetOutputs(BufferFusionOpDesc *opDesc,
                  std::vector<BufferFusionOpDesc *> &outputs);
  std::vector<BufferFusionOpDesc *> GetHead();
  int64_t GetErrorCnt();
  void InitRepeatCurr(const BufferFusionPattern &pattern);

private:
  BufferFusionOpDesc *GetOpDesc(const std::string &descName);
  std::string name_;
  int64_t opMaxCount_;
  std::vector<BufferFusionOpDesc *> ops_;
  std::map<std::string, BufferFusionOpDesc *> opMap_;
  std::vector<BufferFusionOpDesc *> head_;
  int64_t errorCount_;
};
} // namespace fe
#endif // INC_REGISTER_GRAPH_OPTIMIZER_BUFFER_FUSION_PATTERN_H_