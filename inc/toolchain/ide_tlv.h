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

/** @defgroup adx ADX */
#ifndef IDE_TLV_H
#define IDE_TLV_H

/**
 * @ingroup adx
 *
 * adx 命令请求列表
 */
enum cmd_class {
    IDE_EXEC_COMMAND_REQ = 0, /**< 执行device命令请求\n */
    IDE_SEND_FILE_REQ,        /**< 发送文件到device命令请求\n */
    IDE_DEBUG_REQ,            /**< Debug命令请求\n */
    IDE_BBOX_REQ,             /**< Bbox命令请求\n */
    IDE_LOG_REQ,              /**< Log命令请求\n */
    IDE_PROFILING_REQ,        /**< Profiling命令请求\n */
    IDE_OME_DUMP_REQ,         /**< Ome dump命令请求\n */
    IDE_FILE_SYNC_REQ,        /**< 发送文件到AiHost 命令请求\n */
    IDE_EXEC_API_REQ,         /**< 执行AiHost Api命令请求\n */
    IDE_EXEC_HOSTCMD_REQ,     /**< 执行AiHost 命令命令请求\n */
    IDE_DETECT_REQ,           /**< 执行AiHost 通路命令请求\n */
    IDE_FILE_GET_REQ,         /**< 获取AiHost侧文件命令请求\n */
    IDE_NV_REQ,               /**< 执行AiHost Nv命令请求\n */
    IDE_DUMP_REQ,             /**< Dump命令请求\n */
    IDE_FILE_GETD_REQ,        /**< 获取Device侧文件命令请求\n */
    IDE_INVALID_REQ,          /**< 无效命令请求\n */
    NR_IDE_CMD_CLASS,         /**< 标识命令请求最大值\n */
};

/**
 * @ingroup adx
 *
 * adx 命令请求列表
 */
typedef enum cmd_class CmdClassT;

/**
 * @ingroup adx
 *
 * adx 数据交互格式
 */
struct tlv_req {
    enum cmd_class type; /**< 数据包命令类型 */
    int dev_id;          /**< 设备 ID */
    int len;             /**< 数据包数据长度 */
    char value[0];       /**< 数据包数据 */
};

/**
 * @ingroup adx
 *
 * adx 数据交互格式
 */
typedef struct tlv_req TlvReqT;

#endif
/*
 * History: \n
 * 2018-10-10, huawei, 初始化该文件。 \n
 * 2020-02-10, huawei, 更改API规范化。 \n
 *
 * vi: set expandtab ts=4 sw=4 tw=120:
 */
