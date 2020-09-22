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

#ifndef BBOX_DDR_DATA_MDC_H
#define BBOX_DDR_DATA_MDC_H

#include "bbox_ddr_data.h"

/* each Module need define as follows */
/* LPM module */
#define DATA_MODEL_LPM_START MODEL_VECTOR(LPM_START) = { \
    {"start_steps", ELEM_OUTPUT_STR_NL, {0x0}, {0x800}}, \
}

#define DATA_MODEL_LPM MODEL_VECTOR(LPM) = { \
    {"****exc****reg**",     ELEM_OUTPUT_STR,    {0x0},    {0x8}}, \
    {"fault_regs_dfsr",      ELEM_OUTPUT_INT,    {0x1410}, {0x4}}, \
    {"fault_regs_ifsr",      ELEM_OUTPUT_INT,    {0x1414}, {0x4}}, \
    {"fault_regs_adfsr",     ELEM_OUTPUT_INT,    {0x1418}, {0x4}}, \
    {"fault_regs_aifsr",     ELEM_OUTPUT_INT,    {0x141c}, {0x4}}, \
    {"fault_regs_dfar",      ELEM_OUTPUT_INT,    {0x1420}, {0x4}}, \
    {"fault_regs_ifar",      ELEM_OUTPUT_INT,    {0x1424}, {0x4}}, \
    {"usr_regs_r13",         ELEM_OUTPUT_INT,    {0x1428}, {0x4}}, \
    {"usr_regs_r14",         ELEM_OUTPUT_INT,    {0x142c}, {0x4}}, \
    {"svc_regs_regs_r13",    ELEM_OUTPUT_INT,    {0x1430}, {0x4}}, \
    {"svc_regs_regs_r14",    ELEM_OUTPUT_INT,    {0x1434}, {0x4}}, \
    {"svc_regs_regs_spsr",   ELEM_OUTPUT_INT,    {0x1438}, {0x4}}, \
    {"irq_regs_regs_r13",    ELEM_OUTPUT_INT,    {0x143c}, {0x4}}, \
    {"irq_regs_regs_r14",    ELEM_OUTPUT_INT,    {0x1440}, {0x4}}, \
    {"irq_regs_regs_spsr",   ELEM_OUTPUT_INT,    {0x1444}, {0x4}}, \
    {"fiq_regs_regs_r13",    ELEM_OUTPUT_INT,    {0x1448}, {0x4}}, \
    {"fiq_regs_regs_r14",    ELEM_OUTPUT_INT,    {0x144c}, {0x4}}, \
    {"fiq_regs_regs_spsr",   ELEM_OUTPUT_INT,    {0x1450}, {0x4}}, \
    {"und_regs_regs_r13",    ELEM_OUTPUT_INT,    {0x1454}, {0x4}}, \
    {"und_regs_regs_r14",    ELEM_OUTPUT_INT,    {0x1458}, {0x4}}, \
    {"und_regs_regs_spsr",   ELEM_OUTPUT_INT,    {0x145c}, {0x4}}, \
    {"abort_regs_regs_r13",  ELEM_OUTPUT_INT,    {0x1460}, {0x4}}, \
    {"abort_regs_regs_r14",  ELEM_OUTPUT_INT,    {0x1464}, {0x4}}, \
    {"abort_regs_regs_spsr", ELEM_OUTPUT_INT,    {0x1468}, {0x4}}, \
    {"data_regs_r1",         ELEM_OUTPUT_INT,    {0x146c}, {0x4}}, \
    {"data_regs_r2",         ELEM_OUTPUT_INT,    {0x1470}, {0x4}}, \
    {"data_regs_r3",         ELEM_OUTPUT_INT,    {0x1474}, {0x4}}, \
    {"data_regs_r4",         ELEM_OUTPUT_INT,    {0x1478}, {0x4}}, \
    {"data_regs_r5",         ELEM_OUTPUT_INT,    {0x147c}, {0x4}}, \
    {"data_regs_r6",         ELEM_OUTPUT_INT,    {0x1480}, {0x4}}, \
    {"data_regs_r7",         ELEM_OUTPUT_INT,    {0x1484}, {0x4}}, \
    {"data_regs_r8",         ELEM_OUTPUT_INT,    {0x1488}, {0x4}}, \
    {"data_regs_r9",         ELEM_OUTPUT_INT,    {0x148c}, {0x4}}, \
    {"data_regs_r10",        ELEM_OUTPUT_INT,    {0x1490}, {0x4}}, \
    {"data_regs_r11",        ELEM_OUTPUT_INT,    {0x1494}, {0x4}}, \
    {"data_regs_r12",        ELEM_OUTPUT_INT,    {0x1498}, {0x4}}, \
    {"data_regs_r13",        ELEM_OUTPUT_INT,    {0x149c}, {0x4}}, \
    {"data_regs_r14",        ELEM_OUTPUT_INT,    {0x14a0}, {0x4}}, \
    {"data_regs_r15",        ELEM_OUTPUT_INT,    {0x14a4}, {0x4}}, \
    {"prog_regs_cpsr",       ELEM_OUTPUT_INT,    {0x14a8}, {0x4}}, \
    {"prog_regs_spsr",       ELEM_OUTPUT_INT,    {0x14ac}, {0x4}}, \
    {"log",                  ELEM_OUTPUT_STR_NL, {0xDC80}, {0x400}}, \
}

#define DATA_MODEL_LPM_PMU MODEL_VECTOR(LPM_PMU) = { \
    {"cpuid",    ELEM_OUTPUT_INT, {0x0000}, {0x1}}, \
    {"2CCH",     ELEM_OUTPUT_HEX, {0x0001}, {0x1}}, \
    {"2CDH",     ELEM_OUTPUT_HEX, {0x0002}, {0x1}}, \
    {"2CEH",     ELEM_OUTPUT_HEX, {0x0003}, {0x1}}, \
    {"2CFH",     ELEM_OUTPUT_HEX, {0x0004}, {0x1}}, \
    {"2D0H",     ELEM_OUTPUT_HEX, {0x0005}, {0x1}}, \
    {"2D1H",     ELEM_OUTPUT_HEX, {0x0006}, {0x1}}, \
    {"2D2H",     ELEM_OUTPUT_HEX, {0x0007}, {0x1}}, \
    {"2D3H",     ELEM_OUTPUT_HEX, {0x0008}, {0x1}}, \
    {"2D4H",     ELEM_OUTPUT_HEX, {0x0009}, {0x1}}, \
    {"2D5H",     ELEM_OUTPUT_HEX, {0x000A}, {0x1}}, \
    {"2D6H",     ELEM_OUTPUT_HEX, {0x000B}, {0x1}}, \
    {"2D7H",     ELEM_OUTPUT_HEX, {0x000C}, {0x1}}, \
    {"2D8H",     ELEM_OUTPUT_HEX, {0x000D}, {0x1}}, \
    {"2D9H",     ELEM_OUTPUT_HEX, {0x000E}, {0x1}}, \
    {"2DAH",     ELEM_OUTPUT_HEX, {0x000F}, {0x1}}, \
    {"2DBH",     ELEM_OUTPUT_HEX, {0x0010}, {0x1}}, \
    {"2DCH",     ELEM_OUTPUT_HEX, {0x0011}, {0x1}}, \
    {"2DDH",     ELEM_OUTPUT_HEX, {0x0012}, {0x1}}, \
    {"2DEH",     ELEM_OUTPUT_HEX, {0x0013}, {0x1}}, \
    {"2DFH",     ELEM_OUTPUT_HEX, {0x0014}, {0x1}}, \
    {"2E0H",     ELEM_OUTPUT_HEX, {0x0015}, {0x1}}, \
    {"2E1H",     ELEM_OUTPUT_HEX, {0x0016}, {0x1}}, \
    {"2E2H",     ELEM_OUTPUT_HEX, {0x0017}, {0x1}}, \
    {"2E3H",     ELEM_OUTPUT_HEX, {0x0018}, {0x1}}, \
    {"2E4H",     ELEM_OUTPUT_HEX, {0x0019}, {0x1}}, \
    {"2E5H",     ELEM_OUTPUT_HEX, {0x001A}, {0x1}}, \
    {"2E6H",     ELEM_OUTPUT_HEX, {0x001B}, {0x1}}, \
    {"2E7H",     ELEM_OUTPUT_HEX, {0x001C}, {0x1}}, \
    {"slave0",   ELEM_OUTPUT_HEX, {0x0020}, {0x1}}, \
    {"E0",       ELEM_OUTPUT_HEX, {0x0021}, {0x1}}, \
    {"E1",       ELEM_OUTPUT_HEX, {0x0022}, {0x1}}, \
    {"E2",       ELEM_OUTPUT_HEX, {0x0023}, {0x1}}, \
    {"reserve",  ELEM_OUTPUT_HEX, {0x0024}, {0x1}}, \
    {"reserve",  ELEM_OUTPUT_HEX, {0x0025}, {0x1}}, \
    {"reserve",  ELEM_OUTPUT_HEX, {0x0026}, {0x1}}, \
    {"slave1",   ELEM_OUTPUT_HEX, {0x0040}, {0x1}}, \
    {"E0",       ELEM_OUTPUT_HEX, {0x0041}, {0x1}}, \
    {"E1",       ELEM_OUTPUT_HEX, {0x0042}, {0x1}}, \
    {"E2",       ELEM_OUTPUT_HEX, {0x0043}, {0x1}}, \
    {"reserve",  ELEM_OUTPUT_HEX, {0x0044}, {0x1}}, \
    {"reserve",  ELEM_OUTPUT_HEX, {0x0045}, {0x1}}, \
    {"reserve",  ELEM_OUTPUT_HEX, {0x0046}, {0x1}}, \
    {"aicSlave", ELEM_OUTPUT_HEX, {0x0060}, {0x1}}, \
    {"0x79",     ELEM_OUTPUT_HEX, {0x0061}, {0x2}}, \
    {"0x7A",     ELEM_OUTPUT_HEX, {0x0063}, {0x1}}, \
    {"0x7B",     ELEM_OUTPUT_HEX, {0x0064}, {0x1}}, \
    {"0x7C",     ELEM_OUTPUT_HEX, {0x0065}, {0x1}}, \
    {"0x7D",     ELEM_OUTPUT_HEX, {0x0066}, {0x1}}, \
    {"0x7E",     ELEM_OUTPUT_HEX, {0x0067}, {0x1}}, \
    {"dvppSlave", ELEM_OUTPUT_HEX, {0x0080}, {0x1}}, \
    {"0x79",     ELEM_OUTPUT_HEX, {0x0081}, {0x2}}, \
    {"0x7A",     ELEM_OUTPUT_HEX, {0x0083}, {0x1}}, \
    {"0x7B",     ELEM_OUTPUT_HEX, {0x0084}, {0x1}}, \
    {"0x7C",     ELEM_OUTPUT_HEX, {0x0085}, {0x1}}, \
    {"0x7D",     ELEM_OUTPUT_HEX, {0x0086}, {0x1}}, \
    {"0x7E",     ELEM_OUTPUT_HEX, {0x0087}, {0x1}}, \
    {"cpuSlave", ELEM_OUTPUT_HEX, {0x00A0}, {0x1}}, \
    {"0x79",     ELEM_OUTPUT_HEX, {0x00A1}, {0x2}}, \
    {"0x7A",     ELEM_OUTPUT_HEX, {0x00A3}, {0x1}}, \
    {"0x7B",     ELEM_OUTPUT_HEX, {0x00A4}, {0x1}}, \
    {"0x7C",     ELEM_OUTPUT_HEX, {0x00A5}, {0x1}}, \
    {"0x7D",     ELEM_OUTPUT_HEX, {0x00A6}, {0x1}}, \
    {"0x7E",     ELEM_OUTPUT_HEX, {0x00A7}, {0x1}}, \
}

/* (LPM)DDR module */
#define DATA_MODEL_DDR_SRAM MODEL_VECTOR(DDR_SRAM) = { \
    {"dram type",          ELEM_OUTPUT_INT,  {0x00},  {0x4}}, \
    {"dram size",          ELEM_OUTPUT_INT,  {0x04},  {0x4}}, \
    {"rank num",           ELEM_OUTPUT_INT,  {0x08},  {0x4}}, \
    {"chn bit map",        ELEM_OUTPUT_INT,  {0x0c},  {0x4}}, \
    {"manufacteryId[0]",   ELEM_OUTPUT_INT,  {0x10},  {0x4}}, \
    {"manufacteryId[1]",   ELEM_OUTPUT_INT,  {0x14},  {0x4}}, \
    {"manufacteryId[2]",   ELEM_OUTPUT_INT,  {0x18},  {0x4}}, \
    {"manufacteryId[3]",   ELEM_OUTPUT_INT,  {0x1c},  {0x4}}, \
    {"manufacteryId[4]",   ELEM_OUTPUT_INT,  {0x20},  {0x4}}, \
    {"manufacteryId[5]",   ELEM_OUTPUT_INT,  {0x24},  {0x4}}, \
    {"manufacteryId[6]",   ELEM_OUTPUT_INT,  {0x28},  {0x4}}, \
    {"manufacteryId[7]",   ELEM_OUTPUT_INT,  {0x2c},  {0x4}}, \
    {"manufacteryId[8]",   ELEM_OUTPUT_INT,  {0x30},  {0x4}}, \
    {"manufacteryId[9]",   ELEM_OUTPUT_INT,  {0x34},  {0x4}}, \
    {"manufacteryId[10]",  ELEM_OUTPUT_INT,  {0x38},  {0x4}}, \
    {"manufacteryId[11]",  ELEM_OUTPUT_INT,  {0x3c},  {0x4}}, \
    {"manufacteryId[12]",  ELEM_OUTPUT_INT,  {0x40},  {0x4}}, \
    {"manufacteryId[13]",  ELEM_OUTPUT_INT,  {0x44},  {0x4}}, \
    {"manufacteryId[14]",  ELEM_OUTPUT_INT,  {0x48},  {0x4}}, \
    {"manufacteryId[15]",  ELEM_OUTPUT_INT,  {0x4c},  {0x4}}, \
    {"manufacteryId[16]",  ELEM_OUTPUT_INT,  {0x50},  {0x4}}, \
    {"manufacteryId[17]",  ELEM_OUTPUT_INT,  {0x54},  {0x4}}, \
    {"manufacteryId[18]",  ELEM_OUTPUT_INT,  {0x58},  {0x4}}, \
    {"manufacteryId[19]",  ELEM_OUTPUT_INT,  {0x5c},  {0x4}}, \
    {"manufacteryId[20]",  ELEM_OUTPUT_INT,  {0x60},  {0x4}}, \
    {"manufacteryId[21]",  ELEM_OUTPUT_INT,  {0x64},  {0x4}}, \
    {"manufacteryId[22]",  ELEM_OUTPUT_INT,  {0x68},  {0x4}}, \
    {"manufacteryId[23]",  ELEM_OUTPUT_INT,  {0x6c},  {0x4}}, \
    {"iecc",               ELEM_OUTPUT_INT,  {0x70},  {0x4}}, \
    {"swap type",          ELEM_OUTPUT_INT,  {0x74},  {0x4}}, \
    {"freq",               ELEM_OUTPUT_INT,  {0x78},  {0x4}}, \
}

/* TEE module */
#define DATA_MODEL_TEE MODEL_VECTOR(TEE) = { \
    {"tee info", ELEM_OUTPUT_CHAR, {0x0}, {0x20000}}, \
}

/* DVPP module */
#define DATA_MODEL_DVPP MODEL_VECTOR(DVPP) = { \
    {"dvpp info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* DRIVE module */
#define DATA_MODEL_DRIVER MODEL_VECTOR(DRIVER) = { \
    {"driver info", ELEM_OUTPUT_STR_NL, {0x0}, {0x20000}}, \
}

/* TS module, start */
#define DATA_MODEL_TS_START MODEL_VECTOR(TS_START) = { \
    {"ts0 start info", ELEM_OUTPUT_STR_NL, {0x0}, {0x19000}}, \
    {"ts1 start info", ELEM_OUTPUT_STR_NL, {0x19000}, {0x19000}}, \
}

/* TS module */
#define DATA_MODEL_TS MODEL_VECTOR(TS) = { \
    {"ts info", ELEM_OUTPUT_STR_NL, {0x0}, {0xE6F00}}, \
}

/* HSM module */
#define DATA_MODEL_HSM MODEL_VECTOR(HSM) = { \
    {"hsm info", ELEM_OUTPUT_STR_NL, {0x0}, {0x1000}}, \
}

/* HSM module, start */
#define DATA_MODEL_HSM_START MODEL_VECTOR(HSM_START) = { \
    {"hsm start info", ELEM_OUTPUT_STR_NL, {0x0}, {0x1000}}, \
}

// lpm common log data
#define DATA_MODEL_LPM_LOG MODEL_VECTOR(LPM_LOG) = { \
    {"lpm log buffer", ELEM_FEATURE_LOOPBUF,      {1},       {6}}, \
    {"buf_read",       ELEM_CTRL_LPBF_READ,     {0x0},     {0x4}}, \
    {"buf_len",        ELEM_CTRL_LPBF_SIZE,     {0x4},     {0x4}}, \
    {"buf_write",      ELEM_CTRL_LPBF_WRITE,   {0x40},     {0x4}}, \
    {"log_level",      ELEM_OUTPUT_INT,        {0x44},     {0x4}}, \
    {"buf_head_len",   ELEM_CTRL_LPBF_HEAD,    {0x80},     {0x4}}, \
    {"lpm log data",   ELEM_FEATURE_CHARLOG,      {1},       {1}}, \
    {"lpm log",        ELEM_OUTPUT_STR_NL,     {0x80}, {0x1FF80}}, \
}

// hsm common log data
#define DATA_MODEL_HSM_LOG MODEL_VECTOR(HSM_LOG) = { \
    {"hsm log buffer", ELEM_FEATURE_LOOPBUF,        {1},    {6}}, \
    {"buf_read",       ELEM_CTRL_LPBF_READ,       {0x0},  {0x4}}, \
    {"buf_len",        ELEM_CTRL_LPBF_SIZE,       {0x4},  {0x4}}, \
    {"buf_write",      ELEM_CTRL_LPBF_WRITE,     {0x40},  {0x4}}, \
    {"log_level",      ELEM_OUTPUT_INT,          {0x44},  {0x4}}, \
    {"buf_head_len",   ELEM_CTRL_LPBF_HEAD,      {0x80},  {0x4}}, \
    {"hsm log data",   ELEM_FEATURE_CHARLOG,        {1},    {1}}, \
    {"hsm log",        ELEM_OUTPUT_STR_NL,       {0x80}, {0x1FF80}}, \
}

/* isp bbox data */
#define DATA_MODEL_ISP MODEL_VECTOR(ISP) = { \
    {"isp log", ELEM_OUTPUT_STR_NL,      {0},         {0x40000}}, \
}

/* isp module, start */
#define DATA_MODEL_ISP_START MODEL_VECTOR(ISP_START) = { \
    {"isp start info", ELEM_OUTPUT_STR_NL, {0x0}, {0x1000}}, \
}

/* DVPP module */
#define DATA_MODEL_DVPP MODEL_VECTOR(DVPP) = { \
    {"dvpp info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* DP module */
#define DATA_MODEL_DP MODEL_VECTOR(DP) = { \
    {"os kbox info", ELEM_OUTPUT_STR_NL, {0x3000}, {0x4FD000}}, \
}

/* safetyisland module */
#define DATA_MODEL_SAFETYISLAND MODEL_VECTOR(SAFETYISLAND) = { \
    {"safetyisland info", ELEM_OUTPUT_STR_NL, {0x0}, {0xC800}}, \
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

#define DATA_MODEL_HDR_BOOT_AREA MODEL_VECTOR(HDR_BOOT_AREA) = { \
    {"BIOS INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_BOOT_BIOS", ELEM_CTRL_TABLE_GOTO, {0x0}, {0x3000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_BOOT_BIOS}, {0x1}}, \
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
}

#endif // BBOX_DDR_DATA_MDC_H
