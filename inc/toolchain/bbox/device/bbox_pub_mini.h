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

#ifndef BB_PUB_MINI_H
#define BB_PUB_MINI_H

#include "bbox_types.h"

#ifndef RDR_BASIC_TYPE
#define RDR_BASIC_TYPE
/* 模块id列表 */
typedef enum CORE_LIST {
    RDR_UNDEF                   = 0x0,
    RDR_DRIVER                  = 0x1,
    RDR_AP                      = 0x2,
    RDR_TS                      = 0x3,
    RDR_AICPU                   = 0x5,
    RDR_DVPP                    = 0xa,
    RDR_LPM3                    = 0xc,
    RDR_BIOS                    = 0x14,
    RDR_TEEOS                   = 0x15,
    RDR_TF                      = 0x1A,
    RDR_CORE_MAX                = 0x1B,
} rdr_coreid;

#define RDR_COREID_VALID(coreid) ((((coreid) == 0) || ((coreid) >= RDR_CORE_MAX)) ? BBOX_FALSE : BBOX_TRUE)
#define RDR_COREID_MASK(coreid)  (u64)(unsigned)(1 << ((coreid) - 1))

/* 异常类型 */
typedef enum {
    REBOOT_REASON_LABEL0 = 0x0,             /* label0:重启相关 */
    AP_S_COLDBOOT = REBOOT_REASON_LABEL0,                   /* 冷启动，如关机后第一次开机；掉电后第一次开机 */
    BIOS_S_EXCEPTION = 0x1,                                 /* bios异常重启，前一次启动bios异常 */
    AP_S_HOTBOOT = 0x2,                                     /* 热复位，如按键复位，芯片硬复位等 */
    REBOOT_REASON_LABEL1 = 0x10,            /* label1:硬件原因复位 */
    AP_S_ABNORMAL = REBOOT_REASON_LABEL1,                   /* 未检测到的异常 */
    AP_S_TSENSOR = 0x1f,                                    /* soc温保复位 */
    AP_S_PMU = 0x20,                                        /* 过流、欠压、PMU过温引起的硬件复位 */
    AP_S_DDR_FATAL = 0X22,                                  /* ddr fatal异常复位,如:ddr颗粒超温复位 */
    REBOOT_REASON_LABEL2 = 0x24,            /* label2:ap软件原因复位 */
    AP_S_PANIC = REBOOT_REASON_LABEL2,                      /* A核panic，如访问非法地址 */
    AP_S_OOM = 0x2a,                                        /* OOM 异常 */
    AP_S_HDC = 0x2b,                                        /* HDC 断连 */
    REBOOT_REASON_LABEL3 = 0x2c,            /* label3:其他模块复位 */
    STARTUP_S_EXCEPTION = 0x2c,                             /* 模块启动异常 */
    HEARTBEAT_S_EXCEPTION = 0x2d,                           /* 模块心跳异常 */
    LPM3_S_EXCEPTION = 0x32,                                /* LPM3子系统检测到的各种异常 */
    TS_S_EXCEPTION = 0x33,                                  /* TS子系统检测到的各种异常 */
    DVPP_S_EXCEPTION = 0x35,                                /* DVPP异常 */
    DRIVER_S_EXCEPTION = 0x36,                              /* DRIVER异常 */
    TEE_S_EXCEPTION = 0x38,                                 /* teeos异常 */
    REBOOT_REASON_LABEL4 = 0x40,            /* label4: */
    REBOOT_REASON_LABEL5 = 0x65,            /* label5:电源异常 */
    REBOOT_REASON_LABEL6 = 0x6A,            /* label6:xloader异常 */
    REBOOT_REASON_LABEL7 = 0x74,            /* label7:fastboot异常 */
    REBOOT_REASON_LABEL8 = 0x89,            /* label8: host侧异常 */
    DEVICE_LOAD_TIMEOUT = 0x8A,                             /* 设备启动超时 */
    DEVICE_HEAT_BEAT_LOST = 0x8B,                           /* 设备心跳丢失 */
    DEVICE_RESET_INFORM = 0x8C,                             /* 用户复位 */
    DEVICE_ADVANCED_ERR_REPORT = 0x8D,                      /* 设备AER错误 */
    REBOOT_REASON_LABEL9 = 0x90,            /* label9: */
    REBOOT_REASON_LABEL10 = 0xB0,           /* label10: */
    RDR_EXCEPTION_REASON_INVALID = 0xFF,
} EXCH_SOURCE;

enum PROCESS_PRI {
    RDR_OTHER      = 0x0,            /* 不确定 */
    RDR_NOTICE     = 0x1,            /* 提示 */
    RDR_MINOR      = 0x2,            /* 次要 */
    RDR_WARN       = RDR_MINOR,
    RDR_MAJOR      = 0x3,            /* 重要 */
    RDR_ERR        = RDR_MAJOR,
    RDR_CRITICAL   = 0x4,            /* 紧急 */
    RDR_DEAD       = RDR_CRITICAL,
    RDR_PPRI_MAX
};

enum REBOOT_PRI {
    RDR_REBOOT_NOW  = 0x01,         /* 立即重启 */
    RDR_REBOOT_WAIT,                /* 等待重启 */
    RDR_REBOOT_NO,                  /* 不重启 */
    RDR_REBOOT_MAX
};

enum REENTRANT {
    RDR_REENTRANT_ALLOW = 0x01,     /* 可重复触发的异常 */
    RDR_REENTRANT_DISALLOW          /* 不可重复触发的异常 */
};

#define MODULE_MAGIC                        0xbaba0514
#define MODULE_VALID                        1
#define MODULE_EXCEPTION_REGISTER_MAXNUM    512

struct exc_description_s {
    u32 e_excepid;                                              /* 异常id */
    u8 e_process_level;                                         /* 异常处理级别:BBOX_PROCESS_PRI */
    u8 e_reboot_priority;                                       /* 异常重启级别:BBOX_REBOOT_PRI */
    u8 e_excep_type;                                            /* 异常类型 */
    u8 e_reentrant;                                             /* 异常是否可重入 */
    u64 e_notify_core_mask;                                     /* 异常联动掩码 */
    u8 e_desc[BBOX_EXCEPTIONDESC_MAXLEN];                       /* 异常描述 */
};

struct exc_info_s {
    excep_time e_clock;                                         /* 模块触发异常时间 */
    u32 e_excepid;                                              /* 模块触发的异常id */
    u16 e_dump_status;                                          /* 模块将异常信息存预留内存的控制状态 */
    u16 e_save_status;                                          /* 代理将异常信息从预留内存导出的控制状态 */
};

/* 通过共享内存注册异常 */
struct exc_module_info_s {
    u32 magic;                                                  /* 使用宏MODULE_MAGIC */
    u16 e_excep_valid;                                          /* 模块写完注册的异常，则设置MODULE_VALID */
    u16 e_excep_num;                                            /* 模块注册异常个数 */
    u8 e_from_module[BBOX_MODULE_NAME_LEN];                     /* 模块名 */
    struct exc_info_s cur_info;                                 /* 模块dump信息控制状态 */
    u32 e_mini_offset;                                          /* 模块最小集异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_mini_len;                                             /* 模块最小集异常信息长度 */
    u32 e_info_offset;                                          /* 模块全部异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_info_len;                                             /* 模块全部异常信息长度 */
    struct exc_description_s e_description[1];                  /* 模块异常注册信息 */
};

/* 通过注册函数注册异常 */
struct rdr_ddr_module_info_s {
    u32 magic;                                                  /* 使用宏MODULE_MAGIC */
    u32 e_mini_offset;                                          /* 模块最小集异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_mini_len;                                             /* 模块最小集异常信息长度 */
    u32 e_info_offset;                                          /* 模块全部异常信息偏移值，基于模块预留内存首地址，从magic开始 */
    u32 e_info_len;                                             /* 模块全部异常信息长度 */
};

enum MODULE_DUMP_STATUS {
    STATUS_INIT  = 0,
    STATUS_DOING = 1,
    STATUS_DONE  = 2,
};
#endif

#define RDR_MODULE_NAME_LEN         16
#define RDR_EXCEPTIONDESC_MAXLEN    48

typedef void (*rdr_e_callback)(u32, void*);

/*
 *   struct list_head   e_list;
 *   u32 excepid,          exception id;
 *      if excepid equal 0, will auto generation excepid, and return it.
 *   u32 excepid_end,      can register exception id region. [excepid~excepid_end];
        need excepid_end >= excepid,
 *      if excepid_end equal 0, will be register excepid only,
        but excepid & excepid_end cant equal 0 at the same time.
 *   u64 notify_core_mask,  need notify other core mask
 *   u64 reset_core_mask,   need reset other core mask
 *   u8 exce_type,          the type of exception
 *   u8 from_core,          the core of happen exception
 *   u8 process_priority,   exception process priority
 *   u8 reboot_priority,    exception reboot priority
 *   u8 reentrant,          whether to allow exception reentrant
 *   char* from_module,         the module of happen excption
 *   char* desc,                the desc of happen excption
 *   rdr_e_callback callback,   will be called when excption has processed.
 */
struct rdr_exception_info_s {
    struct list_head e_list;
    u32 e_excepid;
    u32 e_excepid_end;
    u64 e_notify_core_mask;
    u64 e_reset_core_mask;
    u8 e_exce_type;
    u8 e_from_core;
    u8 e_process_priority;
    u8 e_reboot_priority;
    u8 e_reentrant;
    u8 e_from_module[RDR_MODULE_NAME_LEN];
    u8 e_desc[RDR_EXCEPTIONDESC_MAXLEN];
    rdr_e_callback e_callback;
};

/*
 * @brief       : module dump done, callback it, tell bbox dump done
 * @param [in]  : u32   excep_id          exception id
 * @param [in]  : u8    core_id            which core done
 * @param [in]  : u8    etype              exception type
 * @param [in]  : excep_time *time          exception time
 * @return      : NA
 */
typedef void (*pfn_cb_dump_done)(u32 excepid, u8 coreid, u8 etype, excep_time time);

/*
 * @brief       : call module dump exception info
 *                the function over, need call fndone to mark dump over
 * @param [in]  : u32   excepid   exception id
 * @param [in]  : u8    coreid    exception core
 * @param [in]  : u8    etype     exception type
 * @param [in]  : excep_time time exception time
 * @param [in]  : char* logpath   exception log path
 * @param [in]  : pfn_cb_dump_done fndone
 * @return      : NA
 */
typedef void (*pfn_dump)(u32 excepid, u8 etype, u8 coreid, excep_time time,
                         char* logpath, pfn_cb_dump_done fndone);

/*
 * @brief       : call module reset
 * @param [in]  : u32   excepid   exception id
 * @param [in]  : u8    core_id             exception core
 * @param [in]  : u8    etype               exception type
 * @return      : NA
 */
typedef void (*pfn_reset)(u32 excepid, u8 etype, u8 coreid);

struct rdr_module_ops {
    pfn_dump    ops_dump;
    pfn_reset   ops_reset;
};

struct rdr_register_module_result {
    u64   log_vaddr;    // reserved physical address
    u32   log_len;      // reserved physical address length
};

/*
 * @brief       : register exception
 * @param [in]  : struct rdr_exception_info_s* e    exception info
 * @return      : excepid
 *                == 0 error; > 0 success
 */
u32 rdr_register_exception(struct rdr_exception_info_s *e);

/*
 * @brief       : unregister exception
 * @param [in]  : u32 excep_id,     exception id;
 * @return      : < 0 fail; >=0 success
 */
int rdr_unregister_exception(u32 excepid);

/*
 * @brief       : register module
 * @param [in]  : u8 core_id                                   core id
 * @param [in]  : struct rdr_module_ops* ops                   ops info
 * @param [out] : struct rdr_register_module_result* retinfo   return info
 * @return      : < 0 error; >=0 success
 */
int rdr_register_module_ops(u8 coreid, const struct rdr_module_ops* ops, struct rdr_register_module_result* retinfo);

/*
 * @brief       : unregister module
 * @param [in]  :u8 core_id   core id
 * @return      : < 0 fail; >=0 success
 */
int rdr_unregister_module_ops(u8 coreid);

/*
 * @brief       : report exception
 * @param [in]  : u32 excep_id          exception id
 * @param [in]  : excep_time timestamp  exception time
 * @param [in]  : u32 arg               arg
 * @return      : NA
 */
void mntn_system_error(u32 excep_id, excep_time timestamp, u32 arg);

/*
 * @brief       : get arg value
 * @param [in]  : u32 excepid           exception id
 * @param [in]  : u8 coreid             exception core id
 * @param [in]  : excep_time timestamp  exception time
 * @param [out] : u32 *arg              return arg value
 * @return      : NA
 */
int rdr_module_dump_get_arg(u32 excepid, u8 coreid, excep_time time, u32 *arg);

#endif // BB_PUB_MINI_H

