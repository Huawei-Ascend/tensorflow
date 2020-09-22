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

#ifndef BBOX_PROXY_CONFIG_H
#define BBOX_PROXY_CONFIG_H

#define BBOX_EXCEPTIONDESC_MAXLEN   48

struct bbox_proxy_exception_info {
    unsigned int e_excepid;                             // 异常id
    unsigned char e_process_priority;                   // 异常处理级别
    unsigned char e_reboot_priority;                    // 异常重启级别
    unsigned char e_excep_type;                         // 异常类型
    unsigned char e_reentrant;                          // 异常是否可重入
    unsigned long long e_notify_core_mask;              // 异常联动掩码
    unsigned long long e_reset_core_mask;               // 异常联动掩码
    unsigned char e_desc[BBOX_EXCEPTIONDESC_MAXLEN];    // 异常描述
};

enum BBOX_PROXY_CAPACITY {
    BBOX_PROXY_CAPACITY_REGISTER    = 1 << 0,
    BBOX_PROXY_CAPACITY_DUMP_DDR    = 1 << 1,
    BBOX_PROXY_CAPACITY_DUMP_LOG    = 1 << 2,
    BBOX_PROXY_CAPACITY_TRANS_ID    = 1 << 3,
};

struct bbox_proxy_module_info {
    unsigned char coreid;           // 模块id
    unsigned long long flag;        // dump能力标记位，BBOX_PROXY_CAPACITY
    const char *name;               // 模块名
    unsigned long long log_addr;    // 模块dump起始地址
    unsigned int log_len;           // 模块dump长度
    unsigned int wait_timeout;      // dump等待超时时间，单位ms
    unsigned int e_count;           // 异常信息注册数量
};

enum BBOX_PROXY_BLOCK_TYPE {
    BLOCK_TYPE_NORMAL        = 1 << 0,    // 普通数据
    BLOCK_TYPE_STARTUP       = 1 << 1,    // 启动异常数据
};

enum BBOX_PROXY_CHECK_FLAG {
    CHECK_NONE                 = 0,
    CHECK_STARTUP_EXCEPID      = 1 << 0,
    CHECK_STARTUP_TMSTMP       = 1 << 1,
    CHECK_RUNTIME_EXCEPID      = 1 << 2,
    CHECK_RUNTIME_TMSTMP       = 1 << 3,
    CHECK_HEARTBEAT_EXCEPID    = 1 << 4,
    CHECK_HEARTBEAT_TMSTMP     = 1 << 5,
};

struct bbox_proxy_block_info {
    unsigned int ctrl_type : 16;
    unsigned int ctrl_flag : 16;
    unsigned int info_offset;
    unsigned int info_block_len;
};

#define BBOX_PROXY_CTRL_NUM             6
#define BBOX_PROXY_CTRL_PAD             3

struct bbox_proxy_ctrl_info {
    unsigned char e_block_num;                                      // 需要使用的控制块个数，最多BBOX_PROXY_CTRL_NUM
    unsigned char padding[BBOX_PROXY_CTRL_PAD];                     // padding
    struct bbox_proxy_block_info block_info[BBOX_PROXY_CTRL_NUM];   // 控制块配置
};

#define BBOX_PROXY_EXCEPTION_NUM    256

struct bbox_proxy_info {
    struct bbox_proxy_module_info module;
    struct bbox_proxy_exception_info exception[BBOX_PROXY_EXCEPTION_NUM];
    struct bbox_proxy_ctrl_info ctrl;
};

#define BBOX_PROXY_INITIALIZER(name) { \
    .module = BBOX_PROXY_MODULE_##name, \
    .exception = BBOX_PROXY_EXCEPTION_##name, \
    .ctrl = BBOX_PROXY_CTRL_##name, \
}

#endif // BBOX_PROXY_CONFIG_H
