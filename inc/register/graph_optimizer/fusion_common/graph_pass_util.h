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

#ifndef INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_PASS_UTIL_H_
#define INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_PASS_UTIL_H_
#include "graph/compute_graph.h"
#include "graph/debug/ge_attr_define.h"
#include "graph/utils/attr_utils.h"
#include "graph/utils/node_utils.h"
#include "graph/utils/type_utils.h"
#include "register/graph_optimizer/graph_optimize_register_error_codes.h"

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace fe {
using NodeTypeMap = std::unordered_map<string, std::unordered_set<ge::NodePtr>>;
using NodeTypeMapPtr = std::shared_ptr<NodeTypeMap>;
struct NodeMapInfo {
  int64_t runCount;
  NodeTypeMapPtr nodeTypeMap;
};
using NodeMapInfoPtr = std::shared_ptr<NodeMapInfo>;

/** @brief define graph pass, which provides two interface: 1. run pass;
* 2. record op names before fusion */
class GraphPassUtil {
 public:
  /** set outputdesc attr for data dump
   *
   * @param originIndex,usually is origin node output index
   *
   * @param fusionIndex,usually is fusion node output index
   *
   * @param originNode, usually is origin node
   *
   * @param fusionNode, usually is fusion node
   */
  static void SetOutputDescAttr(uint32_t originIndex, uint32_t fusionIndex, ge::NodePtr originNode,
                                ge::NodePtr fusionNode) {
    if (fusionNode->GetOpDesc() == nullptr) {
      return;
    }

    auto fusionNodeOutputDesc = fusionNode->GetOpDesc()->MutableOutputDesc(fusionIndex);
    if (fusionNodeOutputDesc == nullptr) {
      return;
    }
    if (originNode->GetOpDesc() == nullptr) {
      return;
    }
    auto originNodeOutputDesc = originNode->GetOpDesc()->MutableOutputDesc(originIndex);
    if (originNodeOutputDesc == nullptr) {
      return;
    }

    std::vector<std::string> originalNames;
    if (ge::AttrUtils::GetListStr(originNode->GetOpDesc(), ge::ATTR_NAME_DATA_DUMP_ORIGIN_OP_NAMES, originalNames) &&
        originalNames.size() > 0) {
      std::string originalName;
      if (ge::AttrUtils::GetStr(originNodeOutputDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_NAME, originalName)) {
        (void)ge::AttrUtils::SetStr(fusionNodeOutputDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_NAME, originalName);

        std::int64_t originOutputIndex = 0;
        if (ge::AttrUtils::GetInt(originNodeOutputDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_OUTPUT_INDEX,
                                  originOutputIndex)) {
          (void)ge::AttrUtils::SetInt(fusionNodeOutputDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_OUTPUT_INDEX,
                                      originOutputIndex);
        }

        ge::DataType originDataType = GetDataDumpOriginDataType(originNodeOutputDesc);
        if (originDataType != ge::DT_UNDEFINED) {
          SetDataDumpOriginDataType(originDataType, fusionNodeOutputDesc);
        }
        ge::Format originFormat = GetDataDumpOriginFormat(originNodeOutputDesc);
        if (originFormat != ge::FORMAT_RESERVED) {
          SetDataDumpOriginFormat(originFormat, fusionNodeOutputDesc);
        }
      }
    } else {
      (void)ge::AttrUtils::SetStr(fusionNodeOutputDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_NAME, originNode->GetName());
      (void)ge::AttrUtils::SetInt(fusionNodeOutputDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_OUTPUT_INDEX, originIndex);
      SetDataDumpOriginDataType(originNodeOutputDesc->GetOriginDataType(), fusionNodeOutputDesc);
      SetDataDumpOriginFormat(originNodeOutputDesc->GetOriginFormat(), fusionNodeOutputDesc);
    }
  }

  /** get origin format for data dump
   *
   * @param tensorDesc,usually is outputDesc
   *
   * @return format of this tensorDesc
   */
  static ge::Format GetDataDumpOriginFormat(ge::GeTensorDescPtr tensorDesc) {
    std::string originFormatStr;
    if (!ge::AttrUtils::GetStr(tensorDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_FORMAT, originFormatStr)) {
      // Can not get the certificate and it's not set,return directly
      return ge::FORMAT_RESERVED;
    }
    if (originFormatStr == "RESERVED") {
      return ge::FORMAT_RESERVED;
    }
    return ge::TypeUtils::SerialStringToFormat(originFormatStr);
  }

  /** set origin format for data dump
   *
   * @param origin format
   *
   * @param tensorDesc,usually is outputDesc
   */
  static void SetDataDumpOriginFormat(ge::Format originFormat, ge::GeTensorDescPtr tensorDesc) {
    std::string originFormatStr = "RESERVED";
    if (originFormat != ge::FORMAT_RESERVED) {
      originFormatStr = ge::TypeUtils::FormatToSerialString(originFormat);
    }
    (void)ge::AttrUtils::SetStr(tensorDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_FORMAT, originFormatStr);
  }

  /** set origin datatype for data dump
   *
   * @param origin datatype
   *
   * @param tensorDesc,usually is outputDesc
   */
  static void SetDataDumpOriginDataType(ge::DataType originDataType, ge::GeTensorDescPtr tensorDesc) {
    std::string originDataTypeStr = "RESERVED";
    if (originDataType != ge::DT_UNDEFINED) {
      originDataTypeStr = ge::TypeUtils::DataTypeToSerialString(originDataType);
    }
    (void)ge::AttrUtils::SetStr(tensorDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_DATA_TYPE, originDataTypeStr);
  }

  /** get origin datatype for data dump
   *
   * @param tensorDesc,usually is outputDesc
   *
   * @return format of this tensorDesc
   */
  static ge::DataType GetDataDumpOriginDataType(ge::GeTensorDescPtr tensorDesc) {
    std::string originDataTypeStr;
    if (!ge::AttrUtils::GetStr(tensorDesc, ge::ATTR_NAME_DATA_DUMP_ORIGIN_DATA_TYPE, originDataTypeStr)) {
      return ge::DT_UNDEFINED;
    }
    if (originDataTypeStr == "RESERVED") {
      return ge::DT_UNDEFINED;
    }
    return ge::TypeUtils::SerialStringToDataType(originDataTypeStr);
  }

  static void AddNodeFromOpTypeMap(NodeMapInfoPtr &nodeMapInfo, ge::NodePtr &nodePtr) {
    if (nodeMapInfo == nullptr || nodePtr == nullptr) {
      return;
    }
    NodeTypeMapPtr nodeTypeMap = nodeMapInfo->nodeTypeMap;
    string realOpType = ge::NodeUtils::GetNodeType(*nodePtr);
    auto iter = nodeTypeMap->find(realOpType);
    if (iter != nodeTypeMap->end()) {
      iter->second.insert(nodePtr);
    } else {
      nodeTypeMap->emplace(std::make_pair(realOpType, std::unordered_set<ge::NodePtr>{nodePtr}));
    }
  }

  static Status GetOpTypeMapToGraph(NodeMapInfoPtr &nodeMapInfo, const ge::ComputeGraph &graph) {
    nodeMapInfo = graph.TryGetExtAttr("NodeMapInfo", nodeMapInfo);
    if (nodeMapInfo == nullptr) {
      return FAILED;
    }
    return SUCCESS;
  }

  static void RecordOriginalNames(std::vector<ge::NodePtr> originalNodes, ge::NodePtr node) {
    // 1. get the originalNames
    std::vector<std::string> originalNames;
    for (ge::NodePtr originalNode : originalNodes) {
      if (originalNode == nullptr || originalNode->GetOpDesc() == nullptr) {
        return;
      }

      ge::OpDescPtr originOpDescPtr = originalNode->GetOpDesc();
      std::vector<std::string> names_tmp;
      bool isHasAttr = ge::AttrUtils::GetListStr(originOpDescPtr, ge::ATTR_NAME_DATA_DUMP_ORIGIN_OP_NAMES, names_tmp);
      if (isHasAttr) {
        for (const auto &node_name : names_tmp) {
          if (!node_name.empty()) {
            originalNames.push_back(node_name);
          }
        }
      } else {
        originalNames.push_back(originOpDescPtr->GetName());
      }
    }

    // 2. set the dump attr
    if (node == nullptr || node->GetOpDesc() == nullptr) {
      return;
    }
    ge::OpDescPtr nodeOpDescPtr = node->GetOpDesc();
    (void)ge::AttrUtils::SetListStr(nodeOpDescPtr, ge::ATTR_NAME_DATA_DUMP_ORIGIN_OP_NAMES, originalNames);
  }
};

}  // namespace fe

#endif  // INC_REGISTER_GRAPH_OPTIMIZER_GRAPH_PASS_UTIL_H_
