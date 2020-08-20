/**
 * @file bbox_ddr_data_cloud.h
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef BBOX_DDR_DATA_CLOUD_H
#define BBOX_DDR_DATA_CLOUD_H

#include "bbox_ddr_data.h"

/* each Module need define as follows */
#define DATA_MODEL_LPFW MODEL_VECTOR(LPFW) = { \
    {"****exc****reg**", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"reset_reason",     ELEM_OUTPUT_INT, {0x80}, {0x4}}, \
    {"slice",            ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"rtc",              ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"REGSP",            ELEM_OUTPUT_INT, {0x98}, {0x8}}, \
    {"REGPC",            ELEM_OUTPUT_INT, {0xa0}, {0x8}}, \
    {"REGELR",           ELEM_OUTPUT_INT, {0xa8}, {0x8}}, \
    {"REGCPSR",          ELEM_OUTPUT_INT, {0xb0}, {0x8}}, \
    {"REGSPSR",          ELEM_OUTPUT_INT, {0xb8}, {0x8}}, \
    {"ESR",              ELEM_OUTPUT_INT, {0xc0}, {0x8}}, \
    {"FAR",              ELEM_OUTPUT_INT, {0xc8}, {0x8}}, \
    {"excTrace",         ELEM_OUTPUT_INT, {0xd0}, {0x1}}, \
    {"ddrExc",           ELEM_OUTPUT_INT, {0xd1}, {0x1}}, \
    {"irqId",            ELEM_OUTPUT_INT, {0xd2}, {0x2}}, \
    {"taskId",           ELEM_OUTPUT_INT, {0xd4}, {0x4}}, \
    {"**backup**reg***", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"reg_backup_index", ELEM_OUTPUT_INT, {0x280}, {0x4}}, \
    {"reason_0",         ELEM_OUTPUT_INT, {0x284}, {0x4}}, \
    {"reason_1",         ELEM_OUTPUT_INT, {0x288}, {0x4}}, \
    {"reason_2",         ELEM_OUTPUT_INT, {0x28C}, {0x4}}, \
    {"x0",               ELEM_OUTPUT_INT, {0x290}, {0x8}}, \
    {"x1",               ELEM_OUTPUT_INT, {0x298}, {0x8}}, \
    {"x2",               ELEM_OUTPUT_INT, {0x2a0}, {0x8}}, \
    {"x3",               ELEM_OUTPUT_INT, {0x2a8}, {0x8}}, \
    {"x4",               ELEM_OUTPUT_INT, {0x2b0}, {0x8}}, \
    {"x5",               ELEM_OUTPUT_INT, {0x2b8}, {0x8}}, \
    {"x6",               ELEM_OUTPUT_INT, {0x2c0}, {0x8}}, \
    {"x7",               ELEM_OUTPUT_INT, {0x2c8}, {0x8}}, \
    {"x8",               ELEM_OUTPUT_INT, {0x2d0}, {0x8}}, \
    {"x9",               ELEM_OUTPUT_INT, {0x2d8}, {0x8}}, \
    {"x10",              ELEM_OUTPUT_INT, {0x2e0}, {0x8}}, \
    {"x11",              ELEM_OUTPUT_INT, {0x2e8}, {0x8}}, \
    {"x12",              ELEM_OUTPUT_INT, {0x2f0}, {0x8}}, \
    {"x13",              ELEM_OUTPUT_INT, {0x2f8}, {0x8}}, \
    {"x14",              ELEM_OUTPUT_INT, {0x300}, {0x8}}, \
    {"x15",              ELEM_OUTPUT_INT, {0x308}, {0x8}}, \
    {"x16",              ELEM_OUTPUT_INT, {0x310}, {0x8}}, \
    {"x17",              ELEM_OUTPUT_INT, {0x318}, {0x8}}, \
    {"x18",              ELEM_OUTPUT_INT, {0x320}, {0x8}}, \
    {"x19",              ELEM_OUTPUT_INT, {0x328}, {0x8}}, \
    {"x20",              ELEM_OUTPUT_INT, {0x330}, {0x8}}, \
    {"x21",              ELEM_OUTPUT_INT, {0x338}, {0x8}}, \
    {"x22",              ELEM_OUTPUT_INT, {0x340}, {0x8}}, \
    {"x23",              ELEM_OUTPUT_INT, {0x348}, {0x8}}, \
    {"x24",              ELEM_OUTPUT_INT, {0x350}, {0x8}}, \
    {"x25",              ELEM_OUTPUT_INT, {0x358}, {0x8}}, \
    {"x26",              ELEM_OUTPUT_INT, {0x360}, {0x8}}, \
    {"x27",              ELEM_OUTPUT_INT, {0x368}, {0x8}}, \
    {"x28",              ELEM_OUTPUT_INT, {0x370}, {0x8}}, \
    {"x29",              ELEM_OUTPUT_INT, {0x378}, {0x8}}, \
    {"x30",              ELEM_OUTPUT_INT, {0x380}, {0x8}}, \
    {"XZR",              ELEM_OUTPUT_INT, {0x388}, {0x8}}, \
    {"ESR",              ELEM_OUTPUT_INT, {0x390}, {0x8}}, \
    {"FAR",              ELEM_OUTPUT_INT, {0x398}, {0x8}}, \
    {"SPSR",             ELEM_OUTPUT_INT, {0x3a0}, {0x8}}, \
    {"ELR",              ELEM_OUTPUT_INT, {0x3a8}, {0x8}}, \
    {"PC",               ELEM_OUTPUT_INT, {0x3b0}, {0x8}}, \
    {"SP",               ELEM_OUTPUT_INT, {0x3b8}, {0x8}}, \
    {"CPSR",             ELEM_OUTPUT_INT, {0x3c0}, {0x8}}, \
    {"Exceptioncode",    ELEM_OUTPUT_INT, {0x3c8}, {0x8}}, \
    {"**runtime*******", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"T-AIC00",          ELEM_OUTPUT_INT, {0xD380}, {0x1}}, \
    {"T-AIC01",          ELEM_OUTPUT_INT, {0xD381}, {0x1}}, \
    {"T-AIC02",          ELEM_OUTPUT_INT, {0xD382}, {0x1}}, \
    {"T-AIC03",          ELEM_OUTPUT_INT, {0xD383}, {0x1}}, \
    {"T-AIC04",          ELEM_OUTPUT_INT, {0xD384}, {0x1}}, \
    {"T-AIC05",          ELEM_OUTPUT_INT, {0xD385}, {0x1}}, \
    {"T-AIC06",          ELEM_OUTPUT_INT, {0xD386}, {0x1}}, \
    {"T-AIC07",          ELEM_OUTPUT_INT, {0xD387}, {0x1}}, \
    {"T-AIC08",          ELEM_OUTPUT_INT, {0xD388}, {0x1}}, \
    {"T-AIC09",          ELEM_OUTPUT_INT, {0xD389}, {0x1}}, \
    {"T-AIC10",          ELEM_OUTPUT_INT, {0xD38A}, {0x1}}, \
    {"T-AIC11",          ELEM_OUTPUT_INT, {0xD38B}, {0x1}}, \
    {"T-AIC12",          ELEM_OUTPUT_INT, {0xD38C}, {0x1}}, \
    {"T-AIC13",          ELEM_OUTPUT_INT, {0xD38D}, {0x1}}, \
    {"T-AIC14",          ELEM_OUTPUT_INT, {0xD38E}, {0x1}}, \
    {"T-AIC15",          ELEM_OUTPUT_INT, {0xD38F}, {0x1}}, \
    {"T-AIC16",          ELEM_OUTPUT_INT, {0xD390}, {0x1}}, \
    {"T-AIC17",          ELEM_OUTPUT_INT, {0xD391}, {0x1}}, \
    {"T-AIC18",          ELEM_OUTPUT_INT, {0xD392}, {0x1}}, \
    {"T-AIC19",          ELEM_OUTPUT_INT, {0xD393}, {0x1}}, \
    {"T-AIC20",          ELEM_OUTPUT_INT, {0xD394}, {0x1}}, \
    {"T-AIC21",          ELEM_OUTPUT_INT, {0xD395}, {0x1}}, \
    {"T-AIC22",          ELEM_OUTPUT_INT, {0xD396}, {0x1}}, \
    {"T-AIC23",          ELEM_OUTPUT_INT, {0xD397}, {0x1}}, \
    {"T-AIC24",          ELEM_OUTPUT_INT, {0xD398}, {0x1}}, \
    {"T-AIC25",          ELEM_OUTPUT_INT, {0xD399}, {0x1}}, \
    {"T-AIC26",          ELEM_OUTPUT_INT, {0xD39A}, {0x1}}, \
    {"T-AIC27",          ELEM_OUTPUT_INT, {0xD39B}, {0x1}}, \
    {"T-AIC28",          ELEM_OUTPUT_INT, {0xD39C}, {0x1}}, \
    {"T-AIC29",          ELEM_OUTPUT_INT, {0xD39D}, {0x1}}, \
    {"T-AIC30",          ELEM_OUTPUT_INT, {0xD39E}, {0x1}}, \
    {"T-AIC31",          ELEM_OUTPUT_INT, {0xD39F}, {0x1}}, \
    {"T-AICPU0",         ELEM_OUTPUT_INT, {0xD3A0}, {0x1}}, \
    {"T-AICPU1",         ELEM_OUTPUT_INT, {0xD3A1}, {0x1}}, \
    {"T-AICPU2",         ELEM_OUTPUT_INT, {0xD3A2}, {0x1}}, \
    {"T-AICPU3",         ELEM_OUTPUT_INT, {0xD3A3}, {0x1}}, \
    {"T-HBMPHY0",        ELEM_OUTPUT_INT, {0xD3A4}, {0x1}}, \
    {"T-HBMPHY1",        ELEM_OUTPUT_INT, {0xD3A5}, {0x1}}, \
    {"T-HBMPHY2",        ELEM_OUTPUT_INT, {0xD3A6}, {0x1}}, \
    {"T-HBMPHY3",        ELEM_OUTPUT_INT, {0xD3A7}, {0x1}}, \
    {"T-DDRPHY",         ELEM_OUTPUT_INT, {0xD3A8}, {0x1}}, \
    {"T-NIMBUS",         ELEM_OUTPUT_INT, {0xD3A9}, {0x1}}, \
    {"T-HBMDEV0",        ELEM_OUTPUT_INT, {0xD3AA}, {0x1}}, \
    {"T-HBMDEV1",        ELEM_OUTPUT_INT, {0xD3AB}, {0x1}}, \
    {"T-HBMDEV2",        ELEM_OUTPUT_INT, {0xD3AC}, {0x1}}, \
    {"T-HBMDEV3",        ELEM_OUTPUT_INT, {0xD3AD}, {0x1}}, \
    {"T-ZONE-AIC",       ELEM_OUTPUT_INT, {0xD3B0}, {0x1}}, \
    {"T-ZONE-AICPU",     ELEM_OUTPUT_INT, {0xD3B1}, {0x1}}, \
    {"T-ZONE-HBMPHY",    ELEM_OUTPUT_INT, {0xD3B2}, {0x1}}, \
    {"T-ZONE-DDRPHY",    ELEM_OUTPUT_INT, {0xD3B3}, {0x1}}, \
    {"T-ZONE-NIMBUS",    ELEM_OUTPUT_INT, {0xD3B4}, {0x1}}, \
    {"T-ZONE-HBMDEV",    ELEM_OUTPUT_INT, {0xD3B5}, {0x1}}, \
    {"TMP_STATUS",       ELEM_OUTPUT_INT, {0xD3B8}, {0x2}}, \
    {"EDP_AVG_CURRENT",  ELEM_OUTPUT_INT, {0xD3BA}, {0x2}}, \
    {"EDP_HEART_ADDR",   ELEM_OUTPUT_INT, {0xD3BC}, {0x4}}, \
    {"EDP_IRQ_COUNT",    ELEM_OUTPUT_INT, {0xD3C0}, {0x4}}, \
    {"EDP_DOWN_COUNT",   ELEM_OUTPUT_INT, {0xD3C4}, {0x2}}, \
    {"EDP_UP_COUNT",     ELEM_OUTPUT_INT, {0xD3C6}, {0x2}}, \
    {"EDP_TIMER_COUNT",  ELEM_OUTPUT_INT, {0xD3C8}, {0x4}}, \
    {"THERMAL_TIMER_CNT",ELEM_OUTPUT_INT, {0xD3CC}, {0x4}}, \
    {"VOLT_VALUE",       ELEM_OUTPUT_INT, {0XD3D4}, {0x4}}, \
    {"CURRENT_VALUE",    ELEM_OUTPUT_INT, {0XD3D8}, {0x4}}, \
    {"POWER_VALUE",      ELEM_OUTPUT_INT, {0XD3DC}, {0x4}}, \
    {"LPNV_MAGIC",       ELEM_OUTPUT_INT, {0xD3E0}, {0x4}}, \
    {"HI_EDP",           ELEM_OUTPUT_INT, {0xD3E4}, {0x1}}, \
    {"EDP_SCALE",        ELEM_OUTPUT_INT, {0xD3E5}, {0x1}}, \
    {"EDP_PERIOD",       ELEM_OUTPUT_INT, {0xD3E6}, {0x2}}, \
    {"EDP_MAX_CURRENT",  ELEM_OUTPUT_INT, {0xD3E8}, {0x2}}, \
    {"EDP_AVE_CURRENT",  ELEM_OUTPUT_INT, {0xD3EA}, {0x2}}, \
    {"AVS_NV",           ELEM_OUTPUT_INT, {0xD3EC}, {0x1}}, \
    {"SVFD_NV",          ELEM_OUTPUT_INT, {0xD3ED}, {0x1}}, \
    {"PLLMODE",          ELEM_OUTPUT_INT, {0xD3EE}, {0x1}}, \
    {"HOT_RESET",        ELEM_OUTPUT_INT, {0xD3EF}, {0x1}}, \
    {"RESERVED_CTRL",    ELEM_OUTPUT_INT, {0xD3F0}, {0x2}}, \
    {"RESERVED_EN",      ELEM_OUTPUT_INT, {0xD3F2}, {0x2}}, \
    {"RESERVED_IMU",     ELEM_OUTPUT_INT, {0xD3F4}, {0x2}}, \
    {"LP_MNTN",          ELEM_OUTPUT_INT, {0xD3F6}, {0x1}}, \
    {"THERMAL_CTRL",     ELEM_OUTPUT_INT, {0xD3F7}, {0x1}}, \
    {"THERMAL_SHUTDOWN", ELEM_OUTPUT_INT, {0xD3F8}, {0x1}}, \
    {"THERMAL_FREQ_STEP",ELEM_OUTPUT_INT, {0xD3F9}, {0x1}}, \
    {"THERMAL_HWRST",    ELEM_OUTPUT_INT, {0xD3FA}, {0x1}}, \
    {"THERMAL_SWRST",    ELEM_OUTPUT_INT, {0xD3FB}, {0x1}}, \
    {"THERMAL_HIGH_TEMP",ELEM_OUTPUT_INT, {0xD3FC}, {0x1}}, \
    {"THERMAL_NOR_TEMP", ELEM_OUTPUT_INT, {0xD3FD}, {0x1}}, \
    {"THERMAL_SLOW_PER", ELEM_OUTPUT_INT, {0xD3FE}, {0x1}}, \
    {"THERMAL_FAST_PER", ELEM_OUTPUT_INT, {0xD3FF}, {0x1}}, \
    {"THERMAL_COOL_CNT", ELEM_OUTPUT_INT, {0xD400}, {0x2}}, \
    {"UTRALSOC_DIS",     ELEM_OUTPUT_INT, {0xD402}, {0x1}}, \
    {"POWERBRAKE_EN",    ELEM_OUTPUT_INT, {0xD403}, {0x1}}, \
    {"POWERBRAKE_SCALE", ELEM_OUTPUT_INT, {0xD404}, {0x2}}, \
    {"FLOOR_FREQ",       ELEM_OUTPUT_INT, {0xD406}, {0x2}}, \
    {"HBM_DEBUG_LEVEL",  ELEM_OUTPUT_INT, {0xD410}, {0x1}}, \
    {"HBM_MP_INIT",      ELEM_OUTPUT_INT, {0xD411}, {0x1}}, \
    {"HBM_ECC_EN",       ELEM_OUTPUT_INT, {0xD412}, {0x1}}, \
    {"HBM_RASC_EN",      ELEM_OUTPUT_INT, {0xD413}, {0x1}}, \
    {"HBM_PATROL_SCRUB", ELEM_OUTPUT_INT, {0xD414}, {0x1}}, \
    {"HBM_CLEAN_MEM_EN", ELEM_OUTPUT_INT, {0xD415}, {0x1}}, \
    {"HBM_POISON_EN",    ELEM_OUTPUT_INT, {0xD416}, {0x1}}, \
    {"HBM_FW_PATCH_EN",  ELEM_OUTPUT_INT, {0xD417}, {0x1}}, \
    {"HBM_FW_SFC_MRS_EN",ELEM_OUTPUT_INT, {0xD418}, {0x1}}, \
    {"HBM_TMON_EN",      ELEM_OUTPUT_INT, {0xD419}, {0x1}}, \
    {"HBM_PD",           ELEM_OUTPUT_INT, {0xD41A}, {0x1}}, \
    {"HBM_HIGH_TEMP",    ELEM_OUTPUT_INT, {0xD41B}, {0x1}}, \
    {"HBM_RESERVED_0",   ELEM_OUTPUT_INT, {0xD41C}, {0x1}}, \
    {"HBM_RESERVED_1",   ELEM_OUTPUT_INT, {0xD41D}, {0x1}}, \
    {"HBM_RESERVED_2",   ELEM_OUTPUT_INT, {0xD41E}, {0x1}}, \
    {"HBM_FREQ",         ELEM_OUTPUT_INT, {0xD420}, {0x2}}, \
    {"DDR_DEBUG_LEVEL",  ELEM_OUTPUT_INT, {0xD422}, {0x1}}, \
    {"DDR_MP_INIT",      ELEM_OUTPUT_INT, {0xD423}, {0x1}}, \
    {"DDR_ECC_EN",       ELEM_OUTPUT_INT, {0xD424}, {0x1}}, \
    {"DDR_RASC_EN",      ELEM_OUTPUT_INT, {0xD425}, {0x1}}, \
    {"DDR_PD_EN",        ELEM_OUTPUT_INT, {0xD426}, {0x1}}, \
    {"DDR_LP_EN",        ELEM_OUTPUT_INT, {0xD427}, {0x1}}, \
    {"DDR_RASC_ALGO",    ELEM_OUTPUT_INT, {0xD428}, {0x1}}, \
    {"DDR_CA_PARITY_EN", ELEM_OUTPUT_INT, {0xD429}, {0x1}}, \
    {"DDR_POISON_EN",    ELEM_OUTPUT_INT, {0xD42A}, {0x1}}, \
    {"DDR_PATROL_SCRUB", ELEM_OUTPUT_INT, {0xD42B}, {0x1}}, \
    {"DDR_TMON_EN",      ELEM_OUTPUT_INT, {0xD42C}, {0x1}}, \
    {"DDR_HIGH_TEMP",    ELEM_OUTPUT_INT, {0xD42D}, {0x1}}, \
    {"DDR_RESERVED_0",   ELEM_OUTPUT_INT, {0xD42E}, {0x1}}, \
    {"DDR_RESERVED_1",   ELEM_OUTPUT_INT, {0xD42F}, {0x1}}, \
    {"DDR_RESERVED_2",   ELEM_OUTPUT_INT, {0xD430}, {0x1}}, \
    {"DDR_FREQ",         ELEM_OUTPUT_INT, {0xD432}, {0x2}}, \
    {"AVS_CALCU_VOLT",   ELEM_OUTPUT_INT, {0xD444}, {0x4}}, \
    {"AVS_WORK_VOLT",    ELEM_OUTPUT_INT, {0xD448}, {0x4}}, \
    {"BBOX_BASE",        ELEM_OUTPUT_INT, {0xD460}, {0x8}}, \
    {"BBOX_SIZE",        ELEM_OUTPUT_INT, {0xD468}, {0x4}}, \
    {"BACKUP_BBOX_ADDR", ELEM_OUTPUT_INT, {0xD470}, {0x8}}, \
    {"BACKUP_BBOX_SIZE", ELEM_OUTPUT_INT, {0xD478}, {0x4}}, \
    {"PMBUS_CHECK0",     ELEM_OUTPUT_INT, {0xD4A0}, {0x2}}, \
    {"PMBUS_CHECK1",     ELEM_OUTPUT_INT, {0xD4A2}, {0x2}}, \
    {"PMBUS_CHECK2",     ELEM_OUTPUT_INT, {0xD4A4}, {0x2}}, \
    {"PMBUS_CHECK3",     ELEM_OUTPUT_INT, {0xD4A6}, {0x2}}, \
    {"PMBUS_CHECK4",     ELEM_OUTPUT_INT, {0xD4A8}, {0x2}}, \
    {"PMBUS_CHECK5",     ELEM_OUTPUT_INT, {0xD4AA}, {0x2}}, \
    {"LP_STARTUP_EXCEPTION",     ELEM_OUTPUT_INT, {0xD4B0}, {0x4}}, \
    {"**GIC*******",     ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"ENABLE[0]",        ELEM_OUTPUT_INT, {0x1280}, {0x4}}, \
    {"ENABLE[1]",        ELEM_OUTPUT_INT, {0x1284}, {0x4}}, \
    {"ENABLE[2]",        ELEM_OUTPUT_INT, {0x1288}, {0x4}}, \
    {"ENABLE[3]",        ELEM_OUTPUT_INT, {0x128C}, {0x4}}, \
    {"ENABLE[4]",        ELEM_OUTPUT_INT, {0x1290}, {0x4}}, \
    {"ENABLE[5]",        ELEM_OUTPUT_INT, {0x1294}, {0x4}}, \
    {"ENABLE[6]",        ELEM_OUTPUT_INT, {0x1298}, {0x4}}, \
    {"ENABLE[7]",        ELEM_OUTPUT_INT, {0x129C}, {0x4}}, \
    {"ENABLE[8]",        ELEM_OUTPUT_INT, {0x12A0}, {0x4}}, \
    {"ENABLE[9]",        ELEM_OUTPUT_INT, {0x12A4}, {0x4}}, \
    {"ENABLE[10]",       ELEM_OUTPUT_INT, {0x12A8}, {0x4}}, \
    {"ENABLE[11]",       ELEM_OUTPUT_INT, {0x12AC}, {0x4}}, \
    {"ENABLE[12]",       ELEM_OUTPUT_INT, {0x12B0}, {0x4}}, \
    {"PENDING[0]",       ELEM_OUTPUT_INT, {0x12B4}, {0x4}}, \
    {"PENDING[1]",       ELEM_OUTPUT_INT, {0x12B8}, {0x4}}, \
    {"PENDING[2]",       ELEM_OUTPUT_INT, {0x12BC}, {0x4}}, \
    {"**IPC*******",     ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"IPC_MBX",          ELEM_OUTPUT_INT, {0x15080}, {0x4}}, \
    {"IPC_SRC",          ELEM_OUTPUT_INT, {0x15084}, {0x4}}, \
    {"IPC_MODE",         ELEM_OUTPUT_INT, {0x15088}, {0x4}}, \
    {"IPC_ICLR",         ELEM_OUTPUT_INT, {0x1508c}, {0x4}}, \
    {"IPC_DATA0",        ELEM_OUTPUT_INT, {0x15090}, {0x4}}, \
    {"IPC_DATA1",        ELEM_OUTPUT_INT, {0x15094}, {0x4}}, \
    {"IPC_DATA2",        ELEM_OUTPUT_INT, {0x15098}, {0x4}}, \
    {"IPC_DATA3",        ELEM_OUTPUT_INT, {0x1509c}, {0x4}}, \
    {"IPC_DATA4",        ELEM_OUTPUT_INT, {0x150a0}, {0x4}}, \
    {"IPC_DATA5",        ELEM_OUTPUT_INT, {0x150a4}, {0x4}}, \
    {"IPC_DATA6",        ELEM_OUTPUT_INT, {0x150a8}, {0x4}}, \
    {"IPC_DATA7",        ELEM_OUTPUT_INT, {0x150ac}, {0x4}}, \
    {"IPC_Q0_DATA0",     ELEM_OUTPUT_INT, {0x150c0}, {0x4}}, \
    {"IPC_Q0_DATA1",     ELEM_OUTPUT_INT, {0x150c4}, {0x4}}, \
    {"IPC_Q0_DATA2",     ELEM_OUTPUT_INT, {0x150c8}, {0x4}}, \
    {"IPC_Q0_DATA3",     ELEM_OUTPUT_INT, {0x150cc}, {0x4}}, \
    {"IPC_Q0_DATA4",     ELEM_OUTPUT_INT, {0x150d0}, {0x4}}, \
    {"IPC_Q0_DATA5",     ELEM_OUTPUT_INT, {0x150d4}, {0x4}}, \
    {"IPC_Q0_DATA6",     ELEM_OUTPUT_INT, {0x150d8}, {0x4}}, \
    {"IPC_Q0_DATA7",     ELEM_OUTPUT_INT, {0x150dc}, {0x4}}, \
    {"IPC_Q0_SYSCNT",    ELEM_OUTPUT_INT, {0x150e0}, {0x4}}, \
    {"IPC_Q1_DATA0",     ELEM_OUTPUT_INT, {0x150e4}, {0x4}}, \
    {"IPC_Q1_DATA1",     ELEM_OUTPUT_INT, {0x150e8}, {0x4}}, \
    {"IPC_Q1_DATA2",     ELEM_OUTPUT_INT, {0x150ec}, {0x4}}, \
    {"IPC_Q1_DATA3",     ELEM_OUTPUT_INT, {0x150f0}, {0x4}}, \
    {"IPC_Q1_DATA4",     ELEM_OUTPUT_INT, {0x150f4}, {0x4}}, \
    {"IPC_Q1_DATA5",     ELEM_OUTPUT_INT, {0x150f8}, {0x4}}, \
    {"IPC_Q1_DATA6",     ELEM_OUTPUT_INT, {0x150fc}, {0x4}}, \
    {"IPC_Q1_DATA7",     ELEM_OUTPUT_INT, {0x15100}, {0x4}}, \
    {"IPC_Q1_SYSCNT",    ELEM_OUTPUT_INT, {0x15104}, {0x4}}, \
    {"IPC_Q2_DATA0",     ELEM_OUTPUT_INT, {0x15108}, {0x4}}, \
    {"IPC_Q2_DATA1",     ELEM_OUTPUT_INT, {0x1510c}, {0x4}}, \
    {"IPC_Q2_DATA2",     ELEM_OUTPUT_INT, {0x15110}, {0x4}}, \
    {"IPC_Q2_DATA3",     ELEM_OUTPUT_INT, {0x15114}, {0x4}}, \
    {"IPC_Q2_DATA4",     ELEM_OUTPUT_INT, {0x15118}, {0x4}}, \
    {"IPC_Q2_DATA5",     ELEM_OUTPUT_INT, {0x1511c}, {0x4}}, \
    {"IPC_Q2_DATA6",     ELEM_OUTPUT_INT, {0x15120}, {0x4}}, \
    {"IPC_Q2_DATA7",     ELEM_OUTPUT_INT, {0x15124}, {0x4}}, \
    {"IPC_Q2_SYSCNT",    ELEM_OUTPUT_INT, {0x15128}, {0x4}}, \
    {"IPC_Q3_DATA0",     ELEM_OUTPUT_INT, {0x1512c}, {0x4}}, \
    {"IPC_Q3_DATA1",     ELEM_OUTPUT_INT, {0x15130}, {0x4}}, \
    {"IPC_Q3_DATA2",     ELEM_OUTPUT_INT, {0x15134}, {0x4}}, \
    {"IPC_Q3_DATA3",     ELEM_OUTPUT_INT, {0x15138}, {0x4}}, \
    {"IPC_Q3_DATA4",     ELEM_OUTPUT_INT, {0x1513c}, {0x4}}, \
    {"IPC_Q3_DATA5",     ELEM_OUTPUT_INT, {0x15140}, {0x4}}, \
    {"IPC_Q3_DATA6",     ELEM_OUTPUT_INT, {0x15144}, {0x4}}, \
    {"IPC_Q3_DATA7",     ELEM_OUTPUT_INT, {0x15148}, {0x4}}, \
    {"IPC_Q3_SYSCNT",    ELEM_OUTPUT_INT, {0x1514c}, {0x4}}, \
    {"IPC_Q4_DATA0",     ELEM_OUTPUT_INT, {0x15150}, {0x4}}, \
    {"IPC_Q4_DATA1",     ELEM_OUTPUT_INT, {0x15154}, {0x4}}, \
    {"IPC_Q4_DATA2",     ELEM_OUTPUT_INT, {0x15158}, {0x4}}, \
    {"IPC_Q4_DATA3",     ELEM_OUTPUT_INT, {0x1515c}, {0x4}}, \
    {"IPC_Q4_DATA4",     ELEM_OUTPUT_INT, {0x15160}, {0x4}}, \
    {"IPC_Q4_DATA5",     ELEM_OUTPUT_INT, {0x15164}, {0x4}}, \
    {"IPC_Q4_DATA6",     ELEM_OUTPUT_INT, {0x15168}, {0x4}}, \
    {"IPC_Q4_DATA7",     ELEM_OUTPUT_INT, {0x1516c}, {0x4}}, \
    {"IPC_Q4_SYSCNT",    ELEM_OUTPUT_INT, {0x15170}, {0x4}}, \
    {"IPC_Q5_DATA0",     ELEM_OUTPUT_INT, {0x15174}, {0x4}}, \
    {"IPC_Q5_DATA1",     ELEM_OUTPUT_INT, {0x15178}, {0x4}}, \
    {"IPC_Q5_DATA2",     ELEM_OUTPUT_INT, {0x1517c}, {0x4}}, \
    {"IPC_Q5_DATA3",     ELEM_OUTPUT_INT, {0x15180}, {0x4}}, \
    {"IPC_Q5_DATA4",     ELEM_OUTPUT_INT, {0x15184}, {0x4}}, \
    {"IPC_Q5_DATA5",     ELEM_OUTPUT_INT, {0x15188}, {0x4}}, \
    {"IPC_Q5_DATA6",     ELEM_OUTPUT_INT, {0x1518c}, {0x4}}, \
    {"IPC_Q5_DATA7",     ELEM_OUTPUT_INT, {0x15190}, {0x4}}, \
    {"IPC_Q5_SYSCNT",    ELEM_OUTPUT_INT, {0x15194}, {0x4}}, \
    {"IPC_Q6_DATA0",     ELEM_OUTPUT_INT, {0x15198}, {0x4}}, \
    {"IPC_Q6_DATA1",     ELEM_OUTPUT_INT, {0x1519C}, {0x4}}, \
    {"IPC_Q6_DATA2",     ELEM_OUTPUT_INT, {0x151A0}, {0x4}}, \
    {"IPC_Q6_DATA3",     ELEM_OUTPUT_INT, {0x151A4}, {0x4}}, \
    {"IPC_Q6_DATA4",     ELEM_OUTPUT_INT, {0x151A8}, {0x4}}, \
    {"IPC_Q6_DATA5",     ELEM_OUTPUT_INT, {0x151AC}, {0x4}}, \
    {"IPC_Q6_DATA6",     ELEM_OUTPUT_INT, {0x151B0}, {0x4}}, \
    {"IPC_Q6_DATA7",     ELEM_OUTPUT_INT, {0x151B4}, {0x4}}, \
    {"IPC_Q6_SYSCNT",    ELEM_OUTPUT_INT, {0x151B8}, {0x4}}, \
    {"IPC_Q7_DATA0",     ELEM_OUTPUT_INT, {0x151BC}, {0x4}}, \
    {"IPC_Q7_DATA1",     ELEM_OUTPUT_INT, {0x151C0}, {0x4}}, \
    {"IPC_Q7_DATA2",     ELEM_OUTPUT_INT, {0x151C4}, {0x4}}, \
    {"IPC_Q7_DATA3",     ELEM_OUTPUT_INT, {0x151C8}, {0x4}}, \
    {"IPC_Q7_DATA4",     ELEM_OUTPUT_INT, {0x151CC}, {0x4}}, \
    {"IPC_Q7_DATA5",     ELEM_OUTPUT_INT, {0x151D0}, {0x4}}, \
    {"IPC_Q7_DATA6",     ELEM_OUTPUT_INT, {0x151D4}, {0x4}}, \
    {"IPC_Q7_DATA7",     ELEM_OUTPUT_INT, {0x151D8}, {0x4}}, \
    {"IPC_Q7_SYSCNT",    ELEM_OUTPUT_INT, {0x151DC}, {0x4}}, \
    {"IPC_Q8_DATA0",     ELEM_OUTPUT_INT, {0x151E0}, {0x4}}, \
    {"IPC_Q8_DATA1",     ELEM_OUTPUT_INT, {0x151E4}, {0x4}}, \
    {"IPC_Q8_DATA2",     ELEM_OUTPUT_INT, {0x151E8}, {0x4}}, \
    {"IPC_Q8_DATA3",     ELEM_OUTPUT_INT, {0x151EC}, {0x4}}, \
    {"IPC_Q8_DATA4",     ELEM_OUTPUT_INT, {0x151F0}, {0x4}}, \
    {"IPC_Q8_DATA5",     ELEM_OUTPUT_INT, {0x151F4}, {0x4}}, \
    {"IPC_Q8_DATA6",     ELEM_OUTPUT_INT, {0x151F8}, {0x4}}, \
    {"IPC_Q8_DATA7",     ELEM_OUTPUT_INT, {0x151FC}, {0x4}}, \
    {"IPC_Q8_SYSCNT",    ELEM_OUTPUT_INT, {0x15200}, {0x4}}, \
    {"IPC_Q9_DATA0",     ELEM_OUTPUT_INT, {0x15204}, {0x4}}, \
    {"IPC_Q9_DATA1",     ELEM_OUTPUT_INT, {0x15208}, {0x4}}, \
    {"IPC_Q9_DATA2",     ELEM_OUTPUT_INT, {0x1520C}, {0x4}}, \
    {"IPC_Q9_DATA3",     ELEM_OUTPUT_INT, {0x15210}, {0x4}}, \
    {"IPC_Q9_DATA4",     ELEM_OUTPUT_INT, {0x15214}, {0x4}}, \
    {"IPC_Q9_DATA5",     ELEM_OUTPUT_INT, {0x15218}, {0x4}}, \
    {"IPC_Q9_DATA6",     ELEM_OUTPUT_INT, {0x1521C}, {0x4}}, \
    {"IPC_Q9_DATA7",     ELEM_OUTPUT_INT, {0x15220}, {0x4}}, \
    {"IPC_Q9_SYSCNT",    ELEM_OUTPUT_INT, {0x15224}, {0x4}}, \
    {"***RAS*******",    ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"RAS_DATA0",        ELEM_OUTPUT_INT, {0x13c80}, {0x4}}, \
    {"RAS_DATA1",        ELEM_OUTPUT_INT, {0x13c88}, {0x4}}, \
    {"***DDR_REG_DUMP*******", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"DDR_REG_DUMP0", ELEM_OUTPUT_INT, {0x15580}, {0x8}}, \
    {"DDR_REG_DUMP1", ELEM_OUTPUT_INT, {0x15588}, {0x8}}, \
    {"DDR_REG_DUMP2", ELEM_OUTPUT_INT, {0x15590}, {0x8}}, \
    {"DDR_REG_DUMP3", ELEM_OUTPUT_INT, {0x15598}, {0x8}}, \
    {"DDR_REG_DUMP4", ELEM_OUTPUT_INT, {0x155A0}, {0x8}}, \
    {"DDR_REG_DUMP5", ELEM_OUTPUT_INT, {0x155A8}, {0x8}}, \
    {"DDR_REG_DUMP6", ELEM_OUTPUT_INT, {0x155B0}, {0x8}}, \
    {"DDR_REG_DUMP7", ELEM_OUTPUT_INT, {0x155B8}, {0x8}}, \
}

#define DATA_MODEL_LPFW_SRAM MODEL_VECTOR(LPFW_SRAM) = { \
    {"****exc****reg**", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"reset_reason",     ELEM_OUTPUT_INT, {0x00}, {0x4}}, \
    {"slice",            ELEM_OUTPUT_INT, {0x08}, {0x4}}, \
    {"rtc",              ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"REGSP",            ELEM_OUTPUT_INT, {0x18}, {0x8}}, \
    {"REGPC",            ELEM_OUTPUT_INT, {0x20}, {0x8}}, \
    {"REGELR",           ELEM_OUTPUT_INT, {0x28}, {0x8}}, \
    {"REGCPSR",          ELEM_OUTPUT_INT, {0x30}, {0x8}}, \
    {"REGSPSR",          ELEM_OUTPUT_INT, {0x38}, {0x8}}, \
    {"ESR",              ELEM_OUTPUT_INT, {0x40}, {0x8}}, \
    {"FAR",              ELEM_OUTPUT_INT, {0x48}, {0x8}}, \
    {"excTrace",         ELEM_OUTPUT_INT, {0x50}, {0x1}}, \
    {"ddrExc",           ELEM_OUTPUT_INT, {0x51}, {0x1}}, \
    {"irqId",            ELEM_OUTPUT_INT, {0x52}, {0x2}}, \
    {"taskId",           ELEM_OUTPUT_INT, {0x54}, {0x4}}, \
    {"**backup**reg***", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"reg_backup_index", ELEM_OUTPUT_INT, {0x820}, {0x4}}, \
    {"reason_0",         ELEM_OUTPUT_INT, {0x824}, {0x4}}, \
    {"reason_1",         ELEM_OUTPUT_INT, {0x828}, {0x4}}, \
    {"reason_2",         ELEM_OUTPUT_INT, {0x82C}, {0x4}}, \
    {"x0",               ELEM_OUTPUT_INT, {0x830}, {0x8}}, \
    {"x1",               ELEM_OUTPUT_INT, {0x848}, {0x8}}, \
    {"x2",               ELEM_OUTPUT_INT, {0x850}, {0x8}}, \
    {"x3",               ELEM_OUTPUT_INT, {0x858}, {0x8}}, \
    {"x4",               ELEM_OUTPUT_INT, {0x860}, {0x8}}, \
    {"x5",               ELEM_OUTPUT_INT, {0x868}, {0x8}}, \
    {"x6",               ELEM_OUTPUT_INT, {0x870}, {0x8}}, \
    {"x7",               ELEM_OUTPUT_INT, {0x878}, {0x8}}, \
    {"x8",               ELEM_OUTPUT_INT, {0x880}, {0x8}}, \
    {"x9",               ELEM_OUTPUT_INT, {0x888}, {0x8}}, \
    {"x10",              ELEM_OUTPUT_INT, {0x890}, {0x8}}, \
    {"x11",              ELEM_OUTPUT_INT, {0x898}, {0x8}}, \
    {"x12",              ELEM_OUTPUT_INT, {0x8A0}, {0x8}}, \
    {"x13",              ELEM_OUTPUT_INT, {0x8A8}, {0x8}}, \
    {"x14",              ELEM_OUTPUT_INT, {0x8B0}, {0x8}}, \
    {"x15",              ELEM_OUTPUT_INT, {0x8B8}, {0x8}}, \
    {"x16",              ELEM_OUTPUT_INT, {0x8C0}, {0x8}}, \
    {"x17",              ELEM_OUTPUT_INT, {0x8C8}, {0x8}}, \
    {"x18",              ELEM_OUTPUT_INT, {0x8D0}, {0x8}}, \
    {"x19",              ELEM_OUTPUT_INT, {0x8D8}, {0x8}}, \
    {"x20",              ELEM_OUTPUT_INT, {0x8E0}, {0x8}}, \
    {"x21",              ELEM_OUTPUT_INT, {0x8E8}, {0x8}}, \
    {"x22",              ELEM_OUTPUT_INT, {0x8F0}, {0x8}}, \
    {"x23",              ELEM_OUTPUT_INT, {0x8F8}, {0x8}}, \
    {"x24",              ELEM_OUTPUT_INT, {0x900}, {0x8}}, \
    {"x25",              ELEM_OUTPUT_INT, {0x908}, {0x8}}, \
    {"x26",              ELEM_OUTPUT_INT, {0x910}, {0x8}}, \
    {"x27",              ELEM_OUTPUT_INT, {0x918}, {0x8}}, \
    {"x28",              ELEM_OUTPUT_INT, {0x920}, {0x8}}, \
    {"x29",              ELEM_OUTPUT_INT, {0x928}, {0x8}}, \
    {"x30",              ELEM_OUTPUT_INT, {0x930}, {0x8}}, \
    {"XZR",              ELEM_OUTPUT_INT, {0x938}, {0x8}}, \
    {"ESR",              ELEM_OUTPUT_INT, {0x940}, {0x8}}, \
    {"FAR",              ELEM_OUTPUT_INT, {0x948}, {0x8}}, \
    {"SPSR",             ELEM_OUTPUT_INT, {0x950}, {0x8}}, \
    {"ELR",              ELEM_OUTPUT_INT, {0x958}, {0x8}}, \
    {"PC",               ELEM_OUTPUT_INT, {0x960}, {0x8}}, \
    {"SP",               ELEM_OUTPUT_INT, {0x968}, {0x8}}, \
    {"CPSR",             ELEM_OUTPUT_INT, {0x970}, {0x8}}, \
    {"**GIC*******",     ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"ENABLE[0]",        ELEM_OUTPUT_INT, {0x2820}, {0x4}}, \
    {"ENABLE[1]",        ELEM_OUTPUT_INT, {0x2824}, {0x4}}, \
    {"ENABLE[2]",        ELEM_OUTPUT_INT, {0x2828}, {0x4}}, \
    {"ENABLE[3]",        ELEM_OUTPUT_INT, {0x282C}, {0x4}}, \
    {"ENABLE[4]",        ELEM_OUTPUT_INT, {0x2830}, {0x4}}, \
    {"ENABLE[5]",        ELEM_OUTPUT_INT, {0x2834}, {0x4}}, \
    {"ENABLE[6]",        ELEM_OUTPUT_INT, {0x2838}, {0x4}}, \
    {"ENABLE[7]",        ELEM_OUTPUT_INT, {0x283C}, {0x4}}, \
    {"ENABLE[8]",        ELEM_OUTPUT_INT, {0x2840}, {0x4}}, \
    {"ENABLE[9]",        ELEM_OUTPUT_INT, {0x2844}, {0x4}}, \
    {"ENABLE[10]",       ELEM_OUTPUT_INT, {0x2848}, {0x4}}, \
    {"ENABLE[11]",       ELEM_OUTPUT_INT, {0x284C}, {0x4}}, \
    {"ENABLE[12]",       ELEM_OUTPUT_INT, {0x2850}, {0x4}}, \
    {"PENDING[0]",       ELEM_OUTPUT_INT, {0x2854}, {0x4}}, \
    {"PENDING[1]",       ELEM_OUTPUT_INT, {0x2858}, {0x4}}, \
    {"PENDING[2]",       ELEM_OUTPUT_INT, {0x285C}, {0x4}}, \
    {"**IPC*******",     ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"IPC_MBX",          ELEM_OUTPUT_INT, {0x1000}, {0x4}}, \
    {"IPC_SRC",          ELEM_OUTPUT_INT, {0x1004}, {0x4}}, \
    {"IPC_MODE",         ELEM_OUTPUT_INT, {0x1008}, {0x4}}, \
    {"IPC_ICLR",         ELEM_OUTPUT_INT, {0x100c}, {0x4}}, \
    {"IPC_DATA0",        ELEM_OUTPUT_INT, {0x1010}, {0x4}}, \
    {"IPC_DATA1",        ELEM_OUTPUT_INT, {0x1014}, {0x4}}, \
    {"IPC_DATA2",        ELEM_OUTPUT_INT, {0x1018}, {0x4}}, \
    {"IPC_DATA3",        ELEM_OUTPUT_INT, {0x101c}, {0x4}}, \
    {"IPC_DATA4",        ELEM_OUTPUT_INT, {0x1020}, {0x4}}, \
    {"IPC_DATA5",        ELEM_OUTPUT_INT, {0x1024}, {0x4}}, \
    {"IPC_DATA6",        ELEM_OUTPUT_INT, {0x1028}, {0x4}}, \
    {"IPC_DATA7",        ELEM_OUTPUT_INT, {0x102c}, {0x4}}, \
    {"IPC_Q0_DATA0",     ELEM_OUTPUT_INT, {0x1040}, {0x4}}, \
    {"IPC_Q0_DATA1",     ELEM_OUTPUT_INT, {0x1044}, {0x4}}, \
    {"IPC_Q0_DATA2",     ELEM_OUTPUT_INT, {0x1048}, {0x4}}, \
    {"IPC_Q0_DATA3",     ELEM_OUTPUT_INT, {0x104c}, {0x4}}, \
    {"IPC_Q0_DATA4",     ELEM_OUTPUT_INT, {0x1050}, {0x4}}, \
    {"IPC_Q0_DATA5",     ELEM_OUTPUT_INT, {0x1054}, {0x4}}, \
    {"IPC_Q0_DATA6",     ELEM_OUTPUT_INT, {0x1058}, {0x4}}, \
    {"IPC_Q0_DATA7",     ELEM_OUTPUT_INT, {0x105c}, {0x4}}, \
    {"IPC_Q0_SYSCNT",    ELEM_OUTPUT_INT, {0x1060}, {0x4}}, \
    {"IPC_Q1_DATA0",     ELEM_OUTPUT_INT, {0x1064}, {0x4}}, \
    {"IPC_Q1_DATA1",     ELEM_OUTPUT_INT, {0x1068}, {0x4}}, \
    {"IPC_Q1_DATA2",     ELEM_OUTPUT_INT, {0x106c}, {0x4}}, \
    {"IPC_Q1_DATA3",     ELEM_OUTPUT_INT, {0x1070}, {0x4}}, \
    {"IPC_Q1_DATA4",     ELEM_OUTPUT_INT, {0x1074}, {0x4}}, \
    {"IPC_Q1_DATA5",     ELEM_OUTPUT_INT, {0x1078}, {0x4}}, \
    {"IPC_Q1_DATA6",     ELEM_OUTPUT_INT, {0x107c}, {0x4}}, \
    {"IPC_Q1_DATA7",     ELEM_OUTPUT_INT, {0x1080}, {0x4}}, \
    {"IPC_Q1_SYSCNT",    ELEM_OUTPUT_INT, {0x1084}, {0x4}}, \
    {"IPC_Q2_DATA0",     ELEM_OUTPUT_INT, {0x1088}, {0x4}}, \
    {"IPC_Q2_DATA1",     ELEM_OUTPUT_INT, {0x108c}, {0x4}}, \
    {"IPC_Q2_DATA2",     ELEM_OUTPUT_INT, {0x1190}, {0x4}}, \
    {"IPC_Q2_DATA3",     ELEM_OUTPUT_INT, {0x1194}, {0x4}}, \
    {"IPC_Q2_DATA4",     ELEM_OUTPUT_INT, {0x1198}, {0x4}}, \
    {"IPC_Q2_DATA5",     ELEM_OUTPUT_INT, {0x119c}, {0x4}}, \
    {"IPC_Q2_DATA6",     ELEM_OUTPUT_INT, {0x11A0}, {0x4}}, \
    {"IPC_Q2_DATA7",     ELEM_OUTPUT_INT, {0x11A4}, {0x4}}, \
    {"IPC_Q2_SYSCNT",    ELEM_OUTPUT_INT, {0x11A8}, {0x4}}, \
    {"IPC_Q3_DATA0",     ELEM_OUTPUT_INT, {0x11Ac}, {0x4}}, \
    {"IPC_Q3_DATA1",     ELEM_OUTPUT_INT, {0x11B0}, {0x4}}, \
    {"IPC_Q3_DATA2",     ELEM_OUTPUT_INT, {0x11B4}, {0x4}}, \
    {"IPC_Q3_DATA3",     ELEM_OUTPUT_INT, {0x11B8}, {0x4}}, \
    {"IPC_Q3_DATA4",     ELEM_OUTPUT_INT, {0x11Bc}, {0x4}}, \
    {"IPC_Q3_DATA5",     ELEM_OUTPUT_INT, {0x11C0}, {0x4}}, \
    {"IPC_Q3_DATA6",     ELEM_OUTPUT_INT, {0x11C4}, {0x4}}, \
    {"IPC_Q3_DATA7",     ELEM_OUTPUT_INT, {0x11C8}, {0x4}}, \
    {"IPC_Q3_SYSCNT",    ELEM_OUTPUT_INT, {0x11Cc}, {0x4}}, \
    {"IPC_Q4_DATA0",     ELEM_OUTPUT_INT, {0x11D0}, {0x4}}, \
    {"IPC_Q4_DATA1",     ELEM_OUTPUT_INT, {0x11D4}, {0x4}}, \
    {"IPC_Q4_DATA2",     ELEM_OUTPUT_INT, {0x11D8}, {0x4}}, \
    {"IPC_Q4_DATA3",     ELEM_OUTPUT_INT, {0x11Dc}, {0x4}}, \
    {"IPC_Q4_DATA4",     ELEM_OUTPUT_INT, {0x11E0}, {0x4}}, \
    {"IPC_Q4_DATA5",     ELEM_OUTPUT_INT, {0x11E4}, {0x4}}, \
    {"IPC_Q4_DATA6",     ELEM_OUTPUT_INT, {0x11E8}, {0x4}}, \
    {"IPC_Q4_DATA7",     ELEM_OUTPUT_INT, {0x11Ec}, {0x4}}, \
    {"IPC_Q4_SYSCNT",    ELEM_OUTPUT_INT, {0x11F0}, {0x4}}, \
    {"IPC_Q5_DATA0",     ELEM_OUTPUT_INT, {0x11F4}, {0x4}}, \
    {"IPC_Q5_DATA1",     ELEM_OUTPUT_INT, {0x11F8}, {0x4}}, \
    {"IPC_Q5_DATA2",     ELEM_OUTPUT_INT, {0x11Fc}, {0x4}}, \
    {"IPC_Q5_DATA3",     ELEM_OUTPUT_INT, {0x1200}, {0x4}}, \
    {"IPC_Q5_DATA4",     ELEM_OUTPUT_INT, {0x1204}, {0x4}}, \
    {"IPC_Q5_DATA5",     ELEM_OUTPUT_INT, {0x1208}, {0x4}}, \
    {"IPC_Q5_DATA6",     ELEM_OUTPUT_INT, {0x120c}, {0x4}}, \
    {"IPC_Q5_DATA7",     ELEM_OUTPUT_INT, {0x1210}, {0x4}}, \
    {"IPC_Q5_SYSCNT",    ELEM_OUTPUT_INT, {0x1214}, {0x4}}, \
    {"IPC_Q6_DATA0",     ELEM_OUTPUT_INT, {0x1218}, {0x4}}, \
    {"IPC_Q6_DATA1",     ELEM_OUTPUT_INT, {0x121C}, {0x4}}, \
    {"IPC_Q6_DATA2",     ELEM_OUTPUT_INT, {0x1220}, {0x4}}, \
    {"IPC_Q6_DATA3",     ELEM_OUTPUT_INT, {0x1224}, {0x4}}, \
    {"IPC_Q6_DATA4",     ELEM_OUTPUT_INT, {0x1228}, {0x4}}, \
    {"IPC_Q6_DATA5",     ELEM_OUTPUT_INT, {0x122C}, {0x4}}, \
    {"IPC_Q6_DATA6",     ELEM_OUTPUT_INT, {0x1230}, {0x4}}, \
    {"IPC_Q6_DATA7",     ELEM_OUTPUT_INT, {0x1234}, {0x4}}, \
    {"IPC_Q6_SYSCNT",    ELEM_OUTPUT_INT, {0x1238}, {0x4}}, \
    {"IPC_Q7_DATA0",     ELEM_OUTPUT_INT, {0x123C}, {0x4}}, \
    {"IPC_Q7_DATA1",     ELEM_OUTPUT_INT, {0x1240}, {0x4}}, \
    {"IPC_Q7_DATA2",     ELEM_OUTPUT_INT, {0x1244}, {0x4}}, \
    {"IPC_Q7_DATA3",     ELEM_OUTPUT_INT, {0x1248}, {0x4}}, \
    {"IPC_Q7_DATA4",     ELEM_OUTPUT_INT, {0x124C}, {0x4}}, \
    {"IPC_Q7_DATA5",     ELEM_OUTPUT_INT, {0x1250}, {0x4}}, \
    {"IPC_Q7_DATA6",     ELEM_OUTPUT_INT, {0x1254}, {0x4}}, \
    {"IPC_Q7_DATA7",     ELEM_OUTPUT_INT, {0x1258}, {0x4}}, \
    {"IPC_Q7_SYSCNT",    ELEM_OUTPUT_INT, {0x125C}, {0x4}}, \
    {"IPC_Q8_DATA0",     ELEM_OUTPUT_INT, {0x1260}, {0x4}}, \
    {"IPC_Q8_DATA1",     ELEM_OUTPUT_INT, {0x1264}, {0x4}}, \
    {"IPC_Q8_DATA2",     ELEM_OUTPUT_INT, {0x1268}, {0x4}}, \
    {"IPC_Q8_DATA3",     ELEM_OUTPUT_INT, {0x126C}, {0x4}}, \
    {"IPC_Q8_DATA4",     ELEM_OUTPUT_INT, {0x1270}, {0x4}}, \
    {"IPC_Q8_DATA5",     ELEM_OUTPUT_INT, {0x1274}, {0x4}}, \
    {"IPC_Q8_DATA6",     ELEM_OUTPUT_INT, {0x1278}, {0x4}}, \
    {"IPC_Q8_DATA7",     ELEM_OUTPUT_INT, {0x127C}, {0x4}}, \
    {"IPC_Q8_SYSCNT",    ELEM_OUTPUT_INT, {0x1280}, {0x4}}, \
    {"IPC_Q9_DATA0",     ELEM_OUTPUT_INT, {0x1284}, {0x4}}, \
    {"IPC_Q9_DATA1",     ELEM_OUTPUT_INT, {0x1288}, {0x4}}, \
    {"IPC_Q9_DATA2",     ELEM_OUTPUT_INT, {0x128C}, {0x4}}, \
    {"IPC_Q9_DATA3",     ELEM_OUTPUT_INT, {0x1290}, {0x4}}, \
    {"IPC_Q9_DATA4",     ELEM_OUTPUT_INT, {0x1294}, {0x4}}, \
    {"IPC_Q9_DATA5",     ELEM_OUTPUT_INT, {0x1298}, {0x4}}, \
    {"IPC_Q9_DATA6",     ELEM_OUTPUT_INT, {0x129C}, {0x4}}, \
    {"IPC_Q9_DATA7",     ELEM_OUTPUT_INT, {0x12A0}, {0x4}}, \
    {"IPC_Q9_SYSCNT",    ELEM_OUTPUT_INT, {0x12A4}, {0x4}}, \
    {"***DDR_REG_DUMP*******", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"DDR_REG_DUMP0", ELEM_OUTPUT_INT, {0x3C60}, {0x8}}, \
    {"DDR_REG_DUMP1", ELEM_OUTPUT_INT, {0x3C68}, {0x8}}, \
    {"DDR_REG_DUMP2", ELEM_OUTPUT_INT, {0x3C70}, {0x8}}, \
    {"DDR_REG_DUMP3", ELEM_OUTPUT_INT, {0x3C78}, {0x8}}, \
    {"DDR_REG_DUMP4", ELEM_OUTPUT_INT, {0x3C80}, {0x8}}, \
    {"DDR_REG_DUMP5", ELEM_OUTPUT_INT, {0x3C88}, {0x8}}, \
    {"DDR_REG_DUMP6", ELEM_OUTPUT_INT, {0x3C90}, {0x8}}, \
    {"DDR_REG_DUMP7", ELEM_OUTPUT_INT, {0x3C98}, {0x8}}, \
}

#define DATA_MODEL_LPFW_PMU MODEL_VECTOR(LPFW_PMU) = { \
    {"cpuid",   ELEM_OUTPUT_INT, {0x0000}, {0x1}}, \
    {"slaveid", ELEM_OUTPUT_INT, {0x0001}, {0x1}}, \
    {"loopid",  ELEM_OUTPUT_INT, {0x0002}, {0x1}}, \
    {"79H",     ELEM_OUTPUT_HEX, {0x0004}, {0x2}}, \
    {"7AH",     ELEM_OUTPUT_HEX, {0x0006}, {0x1}}, \
    {"7BH",     ELEM_OUTPUT_HEX, {0x0007}, {0x1}}, \
    {"7CH",     ELEM_OUTPUT_HEX, {0x0008}, {0x1}}, \
    {"7DH",     ELEM_OUTPUT_HEX, {0x0009}, {0x1}}, \
    {"7EH",     ELEM_OUTPUT_HEX, {0x000A}, {0x1}}, \
    {"80H",     ELEM_OUTPUT_HEX, {0x000B}, {0x1}}, \
    {"88H",     ELEM_OUTPUT_HEX, {0x000C}, {0x2}}, \
    {"89H",     ELEM_OUTPUT_HEX, {0x000E}, {0x2}}, \
    {"8BH",     ELEM_OUTPUT_HEX, {0x0010}, {0x2}}, \
    {"8CH",     ELEM_OUTPUT_HEX, {0x0012}, {0x2}}, \
    {"8DH",     ELEM_OUTPUT_HEX, {0x0014}, {0x2}}, \
    {"96H",     ELEM_OUTPUT_HEX, {0x0016}, {0x2}}, \
    {"97H",     ELEM_OUTPUT_HEX, {0x0018}, {0x2}}, \
    {"20H",     ELEM_OUTPUT_HEX, {0x001A}, {0x1}}, \
    {"21H",     ELEM_OUTPUT_HEX, {0x001C}, {0x2}}, \
}

/* TEE module */
#define DATA_MODEL_TEE MODEL_VECTOR(TEE) = { \
    {"tee info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* TF module */
#define DATA_MODEL_TF MODEL_VECTOR(TF) = { \
    {"x0",           ELEM_OUTPUT_HEX, {0x0},   {0x8}}, \
    {"x1",           ELEM_OUTPUT_HEX, {0x8},   {0x8}}, \
    {"x30",          ELEM_OUTPUT_HEX, {0x10},  {0x8}}, \
    {"x2",           ELEM_OUTPUT_HEX, {0x18},  {0x8}}, \
    {"x3",           ELEM_OUTPUT_HEX, {0x20},  {0x8}}, \
    {"x4",           ELEM_OUTPUT_HEX, {0x28},  {0x8}}, \
    {"x5",           ELEM_OUTPUT_HEX, {0x30},  {0x8}}, \
    {"x6",           ELEM_OUTPUT_HEX, {0x38},  {0x8}}, \
    {"x7",           ELEM_OUTPUT_HEX, {0x40},  {0x8}}, \
    {"x8",           ELEM_OUTPUT_HEX, {0x48},  {0x8}}, \
    {"x9",           ELEM_OUTPUT_HEX, {0x50},  {0x8}}, \
    {"x10",          ELEM_OUTPUT_HEX, {0x58},  {0x8}}, \
    {"x11",          ELEM_OUTPUT_HEX, {0x60},  {0x8}}, \
    {"x12",          ELEM_OUTPUT_HEX, {0x68},  {0x8}}, \
    {"x13",          ELEM_OUTPUT_HEX, {0x70},  {0x8}}, \
    {"x14",          ELEM_OUTPUT_HEX, {0x78},  {0x8}}, \
    {"x15",          ELEM_OUTPUT_HEX, {0x80},  {0x8}}, \
    {"x16",          ELEM_OUTPUT_HEX, {0x88},  {0x8}}, \
    {"x17",          ELEM_OUTPUT_HEX, {0x90},  {0x8}}, \
    {"x18",          ELEM_OUTPUT_HEX, {0x98},  {0x8}}, \
    {"x19",          ELEM_OUTPUT_HEX, {0xA0},  {0x8}}, \
    {"x20",          ELEM_OUTPUT_HEX, {0xA8},  {0x8}}, \
    {"x21",          ELEM_OUTPUT_HEX, {0xB0},  {0x8}}, \
    {"x22",          ELEM_OUTPUT_HEX, {0xB8},  {0x8}}, \
    {"x23",          ELEM_OUTPUT_HEX, {0xC0},  {0x8}}, \
    {"x24",          ELEM_OUTPUT_HEX, {0xC8},  {0x8}}, \
    {"x25",          ELEM_OUTPUT_HEX, {0xD0},  {0x8}}, \
    {"x26",          ELEM_OUTPUT_HEX, {0xD8},  {0x8}}, \
    {"x27",          ELEM_OUTPUT_HEX, {0xE0},  {0x8}}, \
    {"x28",          ELEM_OUTPUT_HEX, {0xE8},  {0x8}}, \
    {"x29",          ELEM_OUTPUT_HEX, {0xF0},  {0x8}}, \
    {"scr_el3",      ELEM_OUTPUT_HEX, {0xF8},  {0x8}}, \
    {"sctlr_el3",    ELEM_OUTPUT_HEX, {0x100}, {0x8}}, \
    {"cptr_el3",     ELEM_OUTPUT_HEX, {0x108}, {0x8}}, \
    {"tcr_el3",      ELEM_OUTPUT_HEX, {0x110}, {0x8}}, \
    {"daif",         ELEM_OUTPUT_HEX, {0x118}, {0x8}}, \
    {"mair_el3",     ELEM_OUTPUT_HEX, {0x120}, {0x8}}, \
    {"spsr_el3",     ELEM_OUTPUT_HEX, {0x128}, {0x8}}, \
    {"elr_el3",      ELEM_OUTPUT_HEX, {0x130}, {0x8}}, \
    {"ttbr0_el3",    ELEM_OUTPUT_HEX, {0x138}, {0x8}}, \
    {"esr_el3",      ELEM_OUTPUT_HEX, {0x140}, {0x8}}, \
    {"far_el3",      ELEM_OUTPUT_HEX, {0x148}, {0x8}}, \
}

/* DVPP module */
#define DATA_MODEL_DVPP MODEL_VECTOR(DVPP) = { \
    {"dvpp info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* DRIVE module */
#define DATA_MODEL_DRIVER MODEL_VECTOR(DRIVER) = { \
    {"driver info", ELEM_OUTPUT_STR_NL, {0x0}, {0x20000}}, \
}

/* TS module */
#define DATA_MODEL_TS MODEL_VECTOR(TS) = { \
    {"ts info", ELEM_OUTPUT_CHAR, {0x0}, {0x100000}}, \
}

/* TS module, start */
#define DATA_MODEL_TS_START MODEL_VECTOR(TS_START) = { \
    {"ts start info", ELEM_OUTPUT_STR_NL, {0x0}, {0xC800}}, \
}

/* AP module, early print */
#define DATA_MODEL_AP_EPRINT MODEL_VECTOR(AP_EPRINT) = { \
    {"early print info", ELEM_OUTPUT_STR_NL, {0x0}, {0x400}}, \
}

/* BIOS module */
#define DATA_MODEL_BIOS MODEL_VECTOR(BIOS) = { \
    {"bios info", ELEM_OUTPUT_STR_NL, {0x0}, {0x50000}}, \
}

/* BIOS module, sram */
#define DATA_MODEL_BIOS_SRAM MODEL_VECTOR(BIOS_SRAM) = { \
    {"LPM3_WAKE_UP_STATUS",             ELEM_OUTPUT_INT, {0x0},   {0x4}}, \
    {"DEBUG_TIME_POWERUP_DONE",         ELEM_OUTPUT_INT, {0x28},  {0x4}}, \
    {"DEBUG_TIME_PERSTHIGH_DONE",       ELEM_OUTPUT_INT, {0x2C},  {0x4}}, \
    {"DEBUG_TIME_PCIEPHY_DONE",         ELEM_OUTPUT_INT, {0x30},  {0x4}}, \
    {"DEBUG_TIME_PHY_FIRMWARE_DONE",    ELEM_OUTPUT_INT, {0x34},  {0x4}}, \
    {"DEBUG_TIME_PCIECTRL_DONE",        ELEM_OUTPUT_INT, {0x38},  {0x4}}, \
    {"DEBUG_TIME_IMG_DONE",             ELEM_OUTPUT_INT, {0x3C},  {0x4}}, \
    {"DEBUG_TIME_SECURE_DONE",          ELEM_OUTPUT_INT, {0x40},  {0x4}}, \
    {"DEBUG_VERSION_ADDR",              ELEM_OUTPUT_HEX, {0x50},  {0x10}}, \
    {"XLOADER_RESET_REG",               ELEM_OUTPUT_INT, {0x200}, {0x4}}, \
    {"XLOADER_KEY_POINT",               ELEM_OUTPUT_INT, {0x204}, {0x4}}, \
    {"XLOADER_TIME_POWERUP_DONE",       ELEM_OUTPUT_INT, {0x228}, {0x4}}, \
    {"XLOADER_TIME_PERSTHIGH_DONE",     ELEM_OUTPUT_INT, {0x22C}, {0x4}}, \
    {"XLOADER_TIME_PCIEPHY_DONE",       ELEM_OUTPUT_INT, {0x230}, {0x4}}, \
    {"XLOADER_TIME_PHY_FIRMWARE_DONE",  ELEM_OUTPUT_INT, {0x234}, {0x4}}, \
    {"XLOADER_TIME_PCIECTRL_DONE",      ELEM_OUTPUT_INT, {0x238}, {0x4}}, \
    {"XLOADER_TIME_PCIE_DETECT_DONE",   ELEM_OUTPUT_INT, {0x23C}, {0x4}}, \
    {"UEFI_LAST_KEYPOINT",              ELEM_OUTPUT_INT, {0x320}, {0x4}}, \
    {"SD_LOAD_FILE_STATUS",             ELEM_OUTPUT_INT, {0x350}, {0x4}}, \
}

/* NETWORK module */
#define DATA_MODEL_NETWORK MODEL_VECTOR(NETWORK) = { \
    {"network info", ELEM_OUTPUT_STR, {0x0}, {0x20000}}, \
}

#define DATA_MODEL_IMU_BOOT_LOG MODEL_VECTOR(IMU_BOOT_LOG) = { \
    {"imu log buffer", ELEM_FEATURE_LOOPBUF,      {1},       {6}}, \
    {"buf_read",       ELEM_CTRL_LPBF_READ,     {0x0},     {0x4}}, \
    {"buf_len",        ELEM_CTRL_LPBF_SIZE,     {0x4},     {0x4}}, \
    {"log_level",      ELEM_OUTPUT_INT,         {0x8},     {0x4}}, \
    {"rollback",       ELEM_CTRL_LPBF_ROLLBK,  {0x10},     {0x4}}, \
    {"buf_write",      ELEM_CTRL_LPBF_WRITE,   {0x40},     {0x4}}, \
    {"buf_head_len",   ELEM_CTRL_LPBF_HEAD,    {0x80},     {0x4}}, \
    {"imu log data",   ELEM_FEATURE_CHARLOG,      {1},       {1}}, \
    {"imu log",        ELEM_OUTPUT_STR_NL,     {0x80}, {0xFFF80}}, \
}

#define DATA_MODEL_IMU_UEFI_BOOT MODEL_VECTOR(IMU_UEFI_BOOT) = { \
    {"imu log buffer", ELEM_FEATURE_LOOPBUF,      {1},       {6}}, \
    {"buf_read",       ELEM_CTRL_LPBF_READ,     {0x0},     {0x4}}, \
    {"buf_len",        ELEM_CTRL_LPBF_SIZE,     {0x4},     {0x4}}, \
    {"log_level",      ELEM_OUTPUT_INT,         {0x8},     {0x4}}, \
    {"rollback",       ELEM_CTRL_LPBF_ROLLBK,  {0x10},     {0x4}}, \
    {"buf_write",      ELEM_CTRL_LPBF_WRITE,   {0x40},     {0x4}}, \
    {"buf_head_len",   ELEM_CTRL_LPBF_HEAD,    {0x80},     {0x4}}, \
    {"imu log data",   ELEM_FEATURE_CHARLOG,      {1},       {1}}, \
    {"imu log",        ELEM_OUTPUT_STR_NL,     {0x80}, {0x2FFF80}}, \
}

#define DATA_MODEL_IMU_RUN_LOG MODEL_VECTOR(IMU_RUN_LOG) = { \
    {"imu log buffer", ELEM_FEATURE_LOOPBUF,      {1},       {6}}, \
    {"buf_read",       ELEM_CTRL_LPBF_READ,     {0x0},     {0x4}}, \
    {"buf_len",        ELEM_CTRL_LPBF_SIZE,     {0x4},     {0x4}}, \
    {"log_level",      ELEM_OUTPUT_INT,         {0x8},     {0x4}}, \
    {"rollback",       ELEM_CTRL_LPBF_ROLLBK,  {0x10},     {0x4}}, \
    {"buf_write",      ELEM_CTRL_LPBF_WRITE,   {0x40},     {0x4}}, \
    {"buf_head_len",   ELEM_CTRL_LPBF_HEAD,    {0x80},     {0x4}}, \
    {"imu log data",   ELEM_FEATURE_CHARLOG,      {1},       {1}}, \
    {"imu log",        ELEM_OUTPUT_STR_NL,     {0x80}, {0x3FFF80}}, \
}

// lpfw exception with imu log
#define DATA_MODEL_LPFW_LOG MODEL_VECTOR(LPFW_LOG) = { \
    {"IMU BOOT LOG", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x100000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_IMU_BOOT_LOG}, {0x1}}, \
    {"IMU RUN LOG", ELEM_CTRL_TABLE_GOTO, {0x400000}, {0x400000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_IMU_RUN_LOG}, {0x1}}, \
}

// bbox kbox info
#define DATA_MODEL_BBOX_KBOX MODEL_VECTOR(BBOX_KBOX) = { \
    {"CONSOLE START",  ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"[console info]", ELEM_OUTPUT_STR_NL, {0x0},     {0x10000}}, \
    {"CONSOLE END",    ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"MESSAGE START",  ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"[message info]", ELEM_OUTPUT_STR_NL, {0x10000}, {0x40000}}, \
    {"MESSAGE END",    ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"if panic",        ELEM_CTRL_COMPARE, {0x50000}, {0x1}}, \
    {"",            ELEM_CTRL_CMP_JUMP_EQ, {0x0},     {0x3}}, \
    {"PANIC START",    ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"[panic info]",   ELEM_OUTPUT_STR_NL, {0x50000}, {0x8000}}, \
    {"PANIC END",      ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"if emerge",       ELEM_CTRL_COMPARE, {0x58000}, {0x1}}, \
    {"",            ELEM_CTRL_CMP_JUMP_EQ, {0x0},     {0x3}}, \
    {"EMERGE START",   ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"[emerge info]",  ELEM_OUTPUT_STR_NL, {0x58000}, {0x8000}}, \
    {"EMERGE END",     ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"if die",          ELEM_CTRL_COMPARE, {0x60000}, {0x1}}, \
    {"",            ELEM_CTRL_CMP_JUMP_EQ, {0x0},     {0x3}}, \
    {"DIE START",      ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
    {"[die info]",     ELEM_OUTPUT_STR_NL, {0x60000}, {0x20000}}, \
    {"DIE END",        ELEM_OUTPUT_DIVIDE, {0x0},     {0x2D}}, \
}

/**
 *  the whole space is 512k, used for histroy data record
 *  the struct distribution is as follows:
 *  +-------------------+
 *  | head info(1k)     |     region:                    area:                   module block:
 *  +-------------------+     +--------------------+     +-----------------+     +-----------------+
 *  | boot region       |---->| first area         |---->| module block    |---->| block head      |
 *  +-------------------+     +--------------------+     +-----------------+     +-----------------+
 *  | run region        |     | second area        |     | module block    |     | block data      |
 *  +-------------------+     +--------------------+     +-----------------+     +-----------------+
 *  | reserved          |     | ......             |     | ......          |
 *  +-------------------+     +--------------------+     +-----------------+
 */
#define DATA_MODEL_HDR_BOOT_BIOS MODEL_VECTOR(HDR_BOOT_BIOS) = { \
    {"magic",            ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version",          ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id",        ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if",               ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used",          ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code",         ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason",           ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index",  ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"bsbc point",       ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"bsbc exc point",   ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"hboot1 point",     ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"hboot1 exc point", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"hboot2 point",     ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"hboot2 exc point", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"[BIOS info]",      ELEM_OUTPUT_STR_NL, {0x480}, {0x2780}}, \
}

#define DATA_MODEL_HDR_BOOT_DDR MODEL_VECTOR(HDR_BOOT_DDR) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"magic_begin", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"init_keypoint", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"ldo8_vol", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"buck3_status", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"buck3_vol", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"buck5_status", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"buck5_vol", ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"wr_test_result", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"rint_status[0]", ELEM_OUTPUT_INT, {0x3C}, {0x4}}, \
    {"rint_status[1]", ELEM_OUTPUT_INT, {0x40}, {0x4}}, \
    {"rint_status[2]", ELEM_OUTPUT_INT, {0x44}, {0x4}}, \
    {"rint_status[3]", ELEM_OUTPUT_INT, {0x48}, {0x4}}, \
    {"rint_status[4]", ELEM_OUTPUT_INT, {0x4C}, {0x4}}, \
    {"rint_status[5]", ELEM_OUTPUT_INT, {0x50}, {0x4}}, \
    {"rint_status[6]", ELEM_OUTPUT_INT, {0x54}, {0x4}}, \
    {"rint_status[7]", ELEM_OUTPUT_INT, {0x58}, {0x4}}, \
    {"SOC_SCTRL_DDRRETENTION_ADDR", ELEM_OUTPUT_INT, {0x5C}, {0x4}}, \
    {"SOC_SCTRL_DDRRETENTIONCLR_ADDR", ELEM_OUTPUT_INT, {0x60}, {0x4}}, \
    {"SOC_SCTRL_DRAMRETENTION_ADDR", ELEM_OUTPUT_INT, {0x64}, {0x4}}, \
    {"SC_DDRC_0_3_RESET_REQ", ELEM_OUTPUT_INT, {0x68}, {0x4}}, \
    {"SC_DDRC_4_7_RESET_REQ", ELEM_OUTPUT_INT, {0x6C}, {0x4}}, \
    {"SC_DDRC_0_3_PACK_RESET_REQ", ELEM_OUTPUT_INT, {0x70}, {0x4}}, \
    {"SC_DDRC_4_7_PACK_RESET_REQ", ELEM_OUTPUT_INT, {0x74}, {0x4}}, \
    {"SC_DDRC_EXMBIST0_REGS_RESET_REQ", ELEM_OUTPUT_INT, {0x78}, {0x4}}, \
    {"SC_DDRC_EXMBIST1_REGS_RESET_REQ", ELEM_OUTPUT_INT, {0x7C}, {0x4}}, \
    {"SOC_SCTRL_DDRC_0_3_AO_RST_ADDR", ELEM_OUTPUT_INT, {0x80}, {0x4}}, \
    {"SOC_SCTRL_DDRC_4_7_AO_RST_ADDR", ELEM_OUTPUT_INT, {0x84}, {0x4}}, \
    {"SOC_PMCTRL_PPLLBYPASS0_ADDR", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"SOC_PMCTRL_PPLLBYPASS1_ADDR", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"SOC_PMCTRL_PPLL3FCTRL_ADDR", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"SOC_PMCTRL_PPLL3FCTRL_FRAC_ADDR", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"SOC_PMCTRL_PPLL4FCTRL_ADDR", ELEM_OUTPUT_INT, {0x98}, {0x4}}, \
    {"SOC_PMCTRL_PPLL4FCTRL_FRAC_ADDR", ELEM_OUTPUT_INT, {0x9C}, {0x4}}, \
    {"SOC_PMCTRL_PPLLOCKSTATUS_ADDR", ELEM_OUTPUT_INT, {0x100}, {0x4}}, \
    {"SC_DDRC_0_3_BYPASS_MODE_CTRL", ELEM_OUTPUT_INT, {0x104}, {0x4}}, \
    {"SC_DDRC_4_7_BYPASS_MODE_CTRL", ELEM_OUTPUT_INT, {0x108}, {0x4}}, \
    {"SC_PLL_PROF_CFG1", ELEM_OUTPUT_INT, {0x10C}, {0x4}}, \
}

#define DATA_MODEL_HDR_BOOT_TEE MODEL_VECTOR(HDR_BOOT_TEE) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[BOOT FATAL INFO SIZE]", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"[BOOT FATAL INFO]", ELEM_OUTPUT_STR_NL,   {0x20}, {0x7E0}}, \
    {"[run point tail]", ELEM_OUTPUT_INT,      {0x800}, {0x4}}, \
    {"[boot point info]", ELEM_OUTPUT_HEX,     {0x804}, {0x20}}, \
    {"[run point info]", ELEM_OUTPUT_HEX,      {0x884}, {0x20}}, \
    {"[last log size]", ELEM_OUTPUT_INT,       {0xC00}, {0x4}}, \
    {"[last log data]", ELEM_OUTPUT_STR_NL,    {0xC04}, {0x3FC}}, \
}

#define DATA_MODEL_HDR_BOOT_ATF MODEL_VECTOR(HDR_BOOT_ATF) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[ATF info]", ELEM_OUTPUT_STR_NL, {0x1C}, {0xFE4}}, \
}

#define DATA_MODEL_HDR_BOOT_AREA MODEL_VECTOR(HDR_BOOT_AREA) = { \
    {"BIOS INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_BIOS", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x3000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_BIOS}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"DDR INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_DDR", ELEM_CTRL_TABLE_GOTO, {0x3000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_DDR}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"TEE INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_TEE", ELEM_CTRL_TABLE_GOTO, {0x4000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_TEE}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"ATF INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_ATF", ELEM_CTRL_TABLE_GOTO, {0x5000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_ATF}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
}

#define DATA_MODEL_HDR_RUN_OS MODEL_VECTOR(HDR_RUN_OS) = { \
    {"magic", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[OS info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"event_flag", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"dump_flag", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"err num", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"[OS log]", ELEM_OUTPUT_STR_NL, {0x100}, {0xF00}}, \
}

#define DATA_MODEL_HDR_RUN_LPFW MODEL_VECTOR(HDR_RUN_LPFW) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0x200}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[LPFW log]", ELEM_OUTPUT_STR_NL, {0x40}, {0x400}}, \
}

#define DATA_MODEL_HDR_RUN_TEE MODEL_VECTOR(HDR_RUN_TEE) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[RUN FATAL INFO SIZE]", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"[RUN FATAL INFO]", ELEM_OUTPUT_STR_NL, {0x20},   {0x7E0}}, \
}

#define DATA_MODEL_HDR_RUN_ATF MODEL_VECTOR(HDR_RUN_ATF) = {\
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0xFF}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[ATF info]", ELEM_OUTPUT_STR_NL, {0x1C}, {0x7E4}}, \
}

#define DATA_MODEL_HDR_RUN_AREA MODEL_VECTOR(HDR_RUN_AREA) = { \
    {"TEE INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_TEE", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_TEE}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"ATF INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_ATF", ELEM_CTRL_TABLE_GOTO, {0x800}, {0x800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_ATF}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"LPFW INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_LPFW", ELEM_CTRL_TABLE_GOTO, {0x1000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_LPFW}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"OS INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_OS", ELEM_CTRL_TABLE_GOTO, {0x2000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_OS}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
}

#define DATA_MODEL_HDR_BOOT MODEL_VECTOR(HDR_BOOT) = { \
    {"area 0", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 1", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x7800}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 2", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0xF000}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 3", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x16800}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 4", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x1E000}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 5", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x25800}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 6", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_AREA", ELEM_CTRL_TABLE_GOTO, {0x2D000}, {0x7800}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_AREA}, {0x1}}, \
}

#define DATA_MODEL_HDR_RUN MODEL_VECTOR(HDR_RUN) = { \
    {"area 0", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 1", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x3C00}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 2", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x7800}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 3", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0xB400}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 4", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0xF000}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 5", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x12C00}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"area 6", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_AREA", ELEM_CTRL_TABLE_GOTO, {0x16800}, {0x3C00}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_AREA}, {0x1}}, \
}

#define DATA_MODEL_HDR_BOOT_INFO MODEL_VECTOR(HDR_BOOT_INFO) = { \
    {"region offset", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"region size", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region config", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"total area", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"history area", ELEM_OUTPUT_INT, {0xC}, {0x4}}, \
    {"error area", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"area config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  used module count", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"module config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  module 0 offset", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"  module 0 size", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 1 offset", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"  module 1 size", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 2 offset", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"  module 2 size", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 3 offset", ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"  module 3 size", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region control", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"area index", ELEM_OUTPUT_INT, {0x6C}, {0x4}}, \
    {"error area count", ELEM_OUTPUT_INT, {0x70}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 0 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x74}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x78}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x7C}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0x80}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0x84}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 1 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0x98}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0x9C}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xA0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 2 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xA4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xA8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xAC}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xB0}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xB4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xB8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 3 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xBC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xC0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xC4}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xC8}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xCC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xD0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 4 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xD4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xD8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xDC}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xE0}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xE4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xE8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 5 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xEC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xF0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xF4}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0xF8}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0xFC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x100}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 6 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x104}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x108}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x10C}, {0x4}}, \
    {"  module id", ELEM_OUTPUT_INT, {0x110}, {0x4}}, \
    {"  exception id", ELEM_OUTPUT_INT, {0x114}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x118}, {0x4}}, \
}

#define DATA_MODEL_HDR_RUN_INFO MODEL_VECTOR(HDR_RUN_INFO) = { \
    {"region offset", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"region size", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region config", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"total area", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"history area", ELEM_OUTPUT_INT, {0xC}, {0x4}}, \
    {"error area", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"area config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  used module count", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"module config:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  module 0 offset", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"  module 0 size", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 1 offset", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"  module 1 size", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 2 offset", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"  module 2 size", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"  module 3 offset", ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"  module 3 size", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"region control", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"area index", ELEM_OUTPUT_INT, {0x6C}, {0x4}}, \
    {"error area count", ELEM_OUTPUT_INT, {0x70}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 0 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x74}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x78}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x7C}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 1 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xA0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 2 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xA4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xA8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xAC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xB8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 3 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xBC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xC0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xC4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xD0}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 4 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xD4}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xD8}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xDC}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0xE8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 5 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0xEC}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0xF0}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0xF4}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x100}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[area 6 control info]", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"  flag", ELEM_OUTPUT_INT, {0x104}, {0x4}}, \
    {"  tag", ELEM_OUTPUT_INT, {0x108}, {0x4}}, \
    {"  exception type", ELEM_OUTPUT_INT, {0x10C}, {0x4}}, \
    {"  reset number", ELEM_OUTPUT_INT, {0x118}, {0x4}}, \
}

#define DATA_MODEL_HDR MODEL_VECTOR(HDR) = { \
    {"head info", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"magic", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"reset count", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"boot region", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_BOOT_INFO", ELEM_CTRL_TABLE_GOTO, {0XC}, {0x168}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_INFO}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"HDR_BOOT", ELEM_CTRL_TABLE_GOTO, {0x400}, {0xA000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"run region", ELEM_OUTPUT_DIVIDE, {0x0}, {0x3D}}, \
    {"HDR_RUN_INFO", ELEM_CTRL_TABLE_GOTO, {0x170}, {0x164}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_INFO}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"HDR_RUN", ELEM_CTRL_TABLE_GOTO, {0x4B400}, {0xA000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN}, {0x1}}, \
}

#endif // BBOX_DDR_DATA_CLOUD_H
