/*

 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.

 * Description: platform info init

 * Author:
 * Create: 2020-03-22

 */

#ifndef PLATFORM_INFO_H
#define PLATFORM_INFO_H

#include <map>
#include <string>
#include <vector>
#include "platform_info_def.h"

using std::map;
using std::vector;
using std::string;

namespace fe {
class PlatformInfoManager {
 public:
  PlatformInfoManager(const PlatformInfoManager &) = delete;
  PlatformInfoManager &operator=(const PlatformInfoManager &) = delete;

  static PlatformInfoManager &Instance();
  uint32_t InitializePlatformInfo();
  uint32_t Finalize();

  uint32_t GetPlatformInfo(const string SoCVersion, PlatformInfo &platformInfo,
                           OptionalInfo &optiCompilationInfo);

  uint32_t GetPlatformInfoWithOutSocVersion(PlatformInfo &platformInfo,
                                            OptionalInfo &optiCompilationInfo);

  void SetOptionalCompilationInfo(OptionalInfo &optiCompilationInfo);

 private:
  PlatformInfoManager();
  ~PlatformInfoManager();

  uint32_t LoadIniFile(string iniFileRealPath);

  void Trim(string &str);

  uint32_t LoadConfigFile(string realPath);

  string RealPath(const std::string &path);

  string GetSoFilePath();

  void ParseVersion(map<string, string> &versionMap, string &socVersion,
                    PlatformInfo &platformInfoTemp);

  void ParseSocInfo(map<string, string> &socInfoMap,
                    PlatformInfo &platformInfoTemp);

  void ParseCubeOfAICoreSpec(map<string, string> &aiCoreSpecMap,
                             PlatformInfo &platformInfoTemp);

  void ParseBufferOfAICoreSpec(map<string, string> &aiCoreSpecMap,
                               PlatformInfo &platformInfoTemp);

  void ParseUBOfAICoreSpec(map<string, string> &aiCoreSpecMap,
                           PlatformInfo &platformInfoTemp);

  void ParseUnzipOfAICoreSpec(map<string, string> &aiCoreSpecMap,
                              PlatformInfo &platformInfoTemp);

  void ParseAICoreSpec(map<string, string> &aiCoreSpecMap,
                       PlatformInfo &platformInfoTemp);

  void ParseBufferOfAICoreMemoryRates(map<string, string> &aiCoreMemoryRatesMap,
                                      PlatformInfo &platformInfoTemp);

  void ParseAICoreMemoryRates(map<string, string> &aiCoreMemoryRatesMap,
                              PlatformInfo &platformInfoTemp);

  void ParseUBOfAICoreMemoryRates(map<string, string> &aiCoreMemoryRatesMap,
                                  PlatformInfo &platformInfoTemp);

  void ParseAICoreintrinsicDtypeMap(
      map<string, string> &aiCoreintrinsicDtypeMap,
      PlatformInfo &platformInfoTemp);

  void ParseVectorCoreSpec(map<string, string> &vectorCoreSpecMap,
                           PlatformInfo &platformInfoTemp);

  void ParseVectorCoreMemoryRates(map<string, string> &vectorCoreMemoryRatesMap,
                                  PlatformInfo &platformInfoTemp);

  void ParseCPUCache(map<string, string> &CPUCacheMap,
                                  PlatformInfo &platformInfoTemp);

  void ParseVectorCoreintrinsicDtypeMap(
      map<string, string> &vectorCoreintrinsicDtypeMap,
      PlatformInfo &platformInfoTemp);

  uint32_t ParsePlatformInfoFromStrToStruct(
      map<string, map<string, string>> &contentInfoMap, string &socVersion,
      PlatformInfo &platformInfoTemp);

  uint32_t AssemblePlatformInfoVector(
      map<string, map<string, string>> &contentInfoMap);
 private:
  bool initFlag_;
  map<string, PlatformInfo> platformInfoMap_;
  OptionalInfo optiCompilationInfo_;
};
}  // namespace fe
#endif
