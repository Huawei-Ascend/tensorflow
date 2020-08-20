/*

 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.

 * Description: platform info init

 * Author:
 * Create: 2020-03-22

 */

#ifndef PLATFORM_INFO_DEF_H
#define PLATFORM_INFO_DEF_H

#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

namespace fe
{
enum MemoryType {
  DDR = 0,
  HBM
};

enum L2Type {
  Cache = 0,
  Buff
};

typedef struct tagStrInfo {
  string aicVersion;
  string ccecAICVersion;
  string ccecAIVVersion;
  string isSupportAIcpuCompiler;
} StrInfo;

typedef struct tagSoCInfo {
  uint32_t aiCoreCnt;
  uint32_t vectorCoreCnt;
  uint32_t aiCpuCnt;
  MemoryType memoryType;
  uint64_t memorySize;
  L2Type l2Type;
  uint64_t l2Size;
  uint32_t l2PageNum;
} SoCInfo;

typedef struct tagAiCoreSpec {
  double cubeFreq;
  uint64_t cubeMSize;
  uint64_t cubeNSize;
  uint64_t cubeKSize;
  uint64_t vecCalcSize;
  uint64_t l0ASize;
  uint64_t l0BSize;
  uint64_t l0CSize;
  uint64_t l1Size;
  uint64_t smaskBuffer;
  uint64_t ubSize;
  uint64_t ubblockSize;
  uint64_t ubbankSize;
  uint64_t ubbankNum;
  uint64_t ubburstInOneBlock;
  uint64_t ubbankGroupNum;
  uint32_t unzipEngines;
  uint32_t unzipMaxRatios;
  uint32_t unzipChannels;
  uint8_t unzipIsTight;
} AiCoreSpec;

typedef struct tagAiCoreMemoryRates {
  double ddrRate;
  double ddrReadRate;
  double ddrWriteRate;
  double l2Rate;
  double l2ReadRate;
  double l2WriteRate;
  double l1ToL0ARate;
  double l1ToL0BRate;
  double l1ToUBRate;
  double l0CToUBRate;
  double ubToL2Rate;
  double ubToDdrRate;
  double ubToL1Rate;
} AiCoreMemoryRates;

typedef struct tagVectorCoreSpec {
  double vecFreq;
  uint64_t vecCalcSize;
  uint64_t smaskBuffer;
  uint64_t ubSize;
  uint64_t ubblockSize;
  uint64_t ubbankSize;
  uint64_t ubbankNum;
  uint64_t ubburstInOneBlock;
  uint64_t ubbankGroupNum;
  uint64_t vectorRegSize;
  uint64_t predicateRegSize;
  uint64_t addressRegSize;
} VectorCoreSpec;

typedef struct tagVectorCoreMemoryRates {
  double ddrRate;
  double ddrReadRate;
  double ddrWriteRate;
  double l2Rate;
  double l2ReadRate;
  double l2WriteRate;
  double ubToL2Rate;
  double ubToDdrRate;
} VectorCoreMemoryRates;

typedef struct tagCPUCache {
  uint32_t AICPUSyncBySW;
  uint32_t TSCPUSyncBySW;
} CPUCache;

typedef struct tagPlatformInfo
{
  StrInfo strInfo;
  SoCInfo socInfo;
  AiCoreSpec aiCoreSpec;
  AiCoreMemoryRates aiCoreMemoryRates;
  map<string, vector<string>> aiCoreIntrinsicDtypeMap;
  VectorCoreSpec vectorCoreSpec;
  VectorCoreMemoryRates vectorCoreMemoryRates;
  CPUCache cpucache;
  map<string, vector<string>> vectorCoreIntrinsicDtypeMap;
} PlatformInfo;

typedef struct tagOptionalInfo
{
  string socVersion;
  string coreType;
  uint32_t aiCoreNum;
  string l1FusionFlag;
} OptionalInfo;
}  // namespace fe
#endif
