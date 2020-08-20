/*
 * @file bbox_types.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2019. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * blackbox header file (blackbox: kernel run data recorder.)
 * define base data type
 */

#ifndef RDR_TYPES_H
#define RDR_TYPES_H
#include <linux/types.h>

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef struct excep_time_t {
    u64 tv_sec;
    u64 tv_usec;
} excep_time;

#define BBOX_TRUE    1
#define BBOX_FALSE   0

#define BBOX_SUCCESS  0
#define BBOX_FAILURE (-1)
#define BBOX_COMM_INVAL     (-2)    // communication failure
#define BBOX_COMM_TIMEOUT   (-3)    // communication timeout
#define BBOX_MSG_INVAL      (-4)    // message invalid
#define BBOX_MSG_NONE       (-5)    // no have data
#define BBOX_NO_SUPPORT     (-10)   // not support

#define BBOX_DISALLOW_REETRANT 1    // return value

#define BBOX_EOK     0
#define BBOX_ENXIO   (-6)       // No such device or address
#define BBOX_ENODEV  (-19)      // No such device
#define BBOX_EINVAL  (-22)      // Invalid argument
#define BBOX_ENOSPC  (-28)      // No space left on device

#define BBOX_NOTIFY_DONE    (NOTIFY_DONE)
#define BBOX_NOTIFY_OK      (NOTIFY_OK)

#define BBOX_UCHAR_INVALID          0xFF
#define BBOX_UINT_INVALID           0xFFFFFFFF
#define BBOX_MODULE_NAME_LEN        16
#define BBOX_EXCEPTIONDESC_MAXLEN   48

#endif // RDR_TYPES_H

