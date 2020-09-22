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

#ifndef BBOX_DDR_DATA_MINI_H
#define BBOX_DDR_DATA_MINI_H

#include "bbox_ddr_data.h"

/* each Module need define as follows */
/* LPM3 module */
#define DATA_MODEL_LPM3_START MODEL_VECTOR(LPM3_START) = { \
    {"start_step 1", ELEM_OUTPUT_INT, {0x0}, {0x1}}, \
    {"start_step 2", ELEM_OUTPUT_INT, {0x1}, {0x1}}, \
    {"start_step 3", ELEM_OUTPUT_INT, {0x2}, {0x1}}, \
    {"start_step 4", ELEM_OUTPUT_INT, {0x3}, {0x1}}, \
    {"start_step 5", ELEM_OUTPUT_INT, {0x4}, {0x1}}, \
    {"start_step 6", ELEM_OUTPUT_INT, {0x5}, {0x1}}, \
    {"start_step 7", ELEM_OUTPUT_INT, {0x6}, {0x1}}, \
    {"start_step 8", ELEM_OUTPUT_INT, {0x7}, {0x1}}, \
    {"start_step 9", ELEM_OUTPUT_INT, {0x8}, {0x1}}, \
    {"start_step 10", ELEM_OUTPUT_INT, {0x9}, {0x1}}, \
    {"start_step 11", ELEM_OUTPUT_INT, {0xa}, {0x1}}, \
    {"start_step 12", ELEM_OUTPUT_INT, {0xb}, {0x1}}, \
    {"start_step 13", ELEM_OUTPUT_INT, {0xc}, {0x1}}, \
    {"start_step 14", ELEM_OUTPUT_INT, {0xd}, {0x1}}, \
    {"start_step 15", ELEM_OUTPUT_INT, {0xe}, {0x1}}, \
    {"start_step 16", ELEM_OUTPUT_INT, {0xf}, {0x1}}, \
    {"start_step 17", ELEM_OUTPUT_INT, {0x10}, {0x1}}, \
    {"start_step 18", ELEM_OUTPUT_INT, {0x11}, {0x1}}, \
    {"start_step 19", ELEM_OUTPUT_INT, {0x12}, {0x1}}, \
    {"start_step 20", ELEM_OUTPUT_INT, {0x13}, {0x1}}, \
    {"start_step 21", ELEM_OUTPUT_INT, {0x14}, {0x1}}, \
    {"start_step 22", ELEM_OUTPUT_INT, {0x15}, {0x1}}, \
    {"start_step 23", ELEM_OUTPUT_INT, {0x16}, {0x1}}, \
    {"start_step 24", ELEM_OUTPUT_INT, {0x17}, {0x1}}, \
    {"start_step 25", ELEM_OUTPUT_INT, {0x18}, {0x1}}, \
    {"start_step 26", ELEM_OUTPUT_INT, {0x19}, {0x1}}, \
    {"start_step 27", ELEM_OUTPUT_INT, {0x1a}, {0x1}}, \
}

#define DATA_MODEL_LPM3  MODEL_VECTOR(LPM3) = { \
    {"****exc****reg**", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"reset_reason", ELEM_OUTPUT_INT, {0x80}, {0x4}}, \
    {"slice", ELEM_OUTPUT_INT, {0x84}, {0x4}}, \
    {"rtc", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"r13", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"lr1", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"pc", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"xpsr", ELEM_OUTPUT_INT, {0x98}, {0x4}}, \
    {"cfsr", ELEM_OUTPUT_INT, {0x9C}, {0x4}}, \
    {"hfsr", ELEM_OUTPUT_INT, {0xa0}, {0x4}}, \
    {"bfar", ELEM_OUTPUT_INT, {0xa4}, {0x4}}, \
    {"exc_trace", ELEM_OUTPUT_INT, {0xa8}, {0x1}}, \
    {"ddr_exc", ELEM_OUTPUT_INT, {0xa9}, {0x1}}, \
    {"irq_id", ELEM_OUTPUT_INT, {0xaa}, {0x2}}, \
    {"task_id", ELEM_OUTPUT_INT, {0xac}, {0x4}}, \
    {"**backup**reg***", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"reg_backup_index", ELEM_OUTPUT_INT, {0x200}, {0x4}}, \
    {"reason_0", ELEM_OUTPUT_INT, {0x204}, {0x4}}, \
    {"reason_1", ELEM_OUTPUT_INT, {0x208}, {0x4}}, \
    {"reason_2", ELEM_OUTPUT_INT, {0x20C}, {0x4}}, \
    {"r0", ELEM_OUTPUT_INT, {0x210}, {0x4}}, \
    {"r1", ELEM_OUTPUT_INT, {0x214}, {0x4}}, \
    {"r2", ELEM_OUTPUT_INT, {0x218}, {0x4}}, \
    {"r3", ELEM_OUTPUT_INT, {0x21c}, {0x4}}, \
    {"r4", ELEM_OUTPUT_INT, {0x220}, {0x4}}, \
    {"r5", ELEM_OUTPUT_INT, {0x224}, {0x4}}, \
    {"r6", ELEM_OUTPUT_INT, {0x228}, {0x4}}, \
    {"r7", ELEM_OUTPUT_INT, {0x22c}, {0x4}}, \
    {"r8", ELEM_OUTPUT_INT, {0x230}, {0x4}}, \
    {"r9", ELEM_OUTPUT_INT, {0x234}, {0x4}}, \
    {"r10", ELEM_OUTPUT_INT, {0x238}, {0x4}}, \
    {"r11", ELEM_OUTPUT_INT, {0x23c}, {0x4}}, \
    {"r12", ELEM_OUTPUT_INT, {0x240}, {0x4}}, \
    {"r13", ELEM_OUTPUT_INT, {0x244}, {0x4}}, \
    {"msp", ELEM_OUTPUT_INT, {0x248}, {0x4}}, \
    {"psp", ELEM_OUTPUT_INT, {0x24c}, {0x4}}, \
    {"lr0_ctrl", ELEM_OUTPUT_INT, {0x250}, {0x4}}, \
    {"lr1", ELEM_OUTPUT_INT, {0x254}, {0x4}}, \
    {"pc", ELEM_OUTPUT_INT, {0x258}, {0x4}}, \
    {"xpsr", ELEM_OUTPUT_INT, {0x25c}, {0x4}}, \
    {"primask", ELEM_OUTPUT_INT, {0x260}, {0x4}}, \
    {"basepri", ELEM_OUTPUT_INT, {0x264}, {0x4}}, \
    {"faultmask", ELEM_OUTPUT_INT, {0x268}, {0x4}}, \
    {"control", ELEM_OUTPUT_INT, {0x26c}, {0x4}}, \
    {"**runtime*******", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"RT_BBX_MAGIC_NUM", ELEM_OUTPUT_INT, {0x7940}, {0x4}}, \
    {"RT_BBX_SIZE", ELEM_OUTPUT_INT, {0x7948}, {0x4}}, \
    {"TSENSOR_A55", ELEM_OUTPUT_INT, {0x7960}, {0x1}}, \
    {"TSENSOR_PERI", ELEM_OUTPUT_INT, {0x7961}, {0x1}}, \
    {"TSENSOR_AIC0", ELEM_OUTPUT_INT, {0x7962}, {0x1}}, \
    {"TSENSOR_AIC1", ELEM_OUTPUT_INT, {0x7963}, {0x1}}, \
    {"DDR_T_GRADE", ELEM_OUTPUT_INT, {0x7964}, {0x1}}, \
    {"EDP_SCALE_0", ELEM_OUTPUT_INT, {0x7965}, {0x1}}, \
    {"EDP_SCALE_1", ELEM_OUTPUT_INT, {0x7966}, {0x1}}, \
    {"TMP_STATUS", ELEM_OUTPUT_INT, {0x7967}, {0x1}}, \
    {"TMP_CTRL_ST", ELEM_OUTPUT_INT, {0x7968}, {0x1}}, \
    {"AIC_FREQ_ST", ELEM_OUTPUT_INT, {0x7969}, {0x1}}, \
    {"A55_FREQ_ST", ELEM_OUTPUT_INT, {0x796A}, {0x1}}, \
    {"AIC_NUM_ST", ELEM_OUTPUT_INT, {0x796B}, {0x1}}, \
    {"TMP_RST", ELEM_OUTPUT_INT, {0x796C}, {0x1}}, \
    {"TMP_HIGH", ELEM_OUTPUT_INT, {0x796D}, {0x1}}, \
    {"TMP_NOR", ELEM_OUTPUT_INT, {0x796E}, {0x1}}, \
    {"TMP_PERIOD", ELEM_OUTPUT_INT, {0x796F}, {0x1}}, \
    {"T_RST_STATUS", ELEM_OUTPUT_INT, {0x797D}, {0x1}}, \
    {"T_ERR_TSENSOR", ELEM_OUTPUT_INT, {0x797E}, {0x1}}, \
    {"T_ERR_EFUSE", ELEM_OUTPUT_INT, {0x797F}, {0x1}}, \
    {"**NV*******", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"LPNV_MAGIC",ELEM_OUTPUT_INT, {0x7980}, {0x4}}, \
    {"LP_LPALL_NV",ELEM_OUTPUT_INT, {0x7984}, {0x1}}, \
    {"LP_AVS_NV",ELEM_OUTPUT_INT, {0x7985}, {0x1}}, \
    {"LP_SVFD_NV",ELEM_OUTPUT_INT, {0x7986}, {0x1}}, \
    {"LP_PLLMOD_SEL_NV",ELEM_OUTPUT_INT, {0x7987}, {0x1}}, \
    {"LP_DEEP_SLEEP_NV",ELEM_OUTPUT_INT, {0x7988}, {0x1}}, \
    {"LP_HIMNTN_NV",ELEM_OUTPUT_INT, {0x7989}, {0x1}}, \
    {"LP_LOGBUF_CTRL",ELEM_OUTPUT_INT, {0x798A}, {0x1}}, \
    {"LP_WDT_RST_NV",ELEM_OUTPUT_INT, {0x798B}, {0x1}}, \
    {"LP_RDRLOG_CTRL",ELEM_OUTPUT_INT, {0x798C}, {0x1}}, \
    {"THERMAL_EN_NV", ELEM_OUTPUT_INT, {0x798D}, {0x1}}, \
    {"TMP_HWRST_EN_NV", ELEM_OUTPUT_INT, {0x798E}, {0x1}}, \
    {"TMP_GCTRL_EN_NV", ELEM_OUTPUT_INT, {0x798F}, {0x1}}, \
    {"TMP_GCTRL_SCALE_NV", ELEM_OUTPUT_INT, {0x7990}, {0x1}}, \
    {"TMP_RST_NV", ELEM_OUTPUT_INT, {0x7991}, {0x1}}, \
    {"TMP_HIGH_NV", ELEM_OUTPUT_INT, {0x7992}, {0x1}}, \
    {"TMP_NOR_NV", ELEM_OUTPUT_INT, {0x7993}, {0x1}}, \
    {"TMP_PERIOD_NV", ELEM_OUTPUT_INT, {0x7994}, {0x1}}, \
    {"DDR_ALL_NV", ELEM_OUTPUT_INT, {0x7995}, {0x1}}, \
    {"DDR_THERMAL_NV", ELEM_OUTPUT_INT, {0x7996}, {0x1}}, \
    {"DDR_EXMBIST_NV", ELEM_OUTPUT_INT, {0x7997}, {0x1}}, \
    {"DDR_SWAP_NV", ELEM_OUTPUT_INT, {0x7998}, {0x1}}, \
    {"DDR_IECC_NV", ELEM_OUTPUT_INT, {0x7999}, {0x1}}, \
    {"DDR_PASR_NV", ELEM_OUTPUT_INT, {0x799A}, {0x1}}, \
    {"DDR_UDIS_NV", ELEM_OUTPUT_INT, {0x799B}, {0x1}}, \
    {"DDR_TDIS_NV", ELEM_OUTPUT_INT, {0x799C}, {0x1}}, \
    {"DDR_FREQ_NV", ELEM_OUTPUT_INT, {0x799D}, {0x1}}, \
    {"**DDR_RUNTIME***", ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"DDR_STATUS",      ELEM_OUTPUT_INT, {0x79C0}, {0x2}}, \
    {"INITFREQ",        ELEM_OUTPUT_INT, {0x79C2}, {0x1}}, \
    {"DDR_TMP_PERIOD",  ELEM_OUTPUT_INT, {0x79C3}, {0x1}}, \
    {"DDR_PD_PRD",      ELEM_OUTPUT_INT, {0x79C4}, {0x2}}, \
    {"DDR_ASREF_PRD",   ELEM_OUTPUT_INT, {0x79C6}, {0x2}}, \
    {"DDR_FREQ_LOAD",   ELEM_OUTPUT_HEX, {0x79C8}, {0x10}}, \
    {"DDR_MIN",         ELEM_OUTPUT_INT, {0x79D8}, {0x1}}, \
    {"DDR_MAX",         ELEM_OUTPUT_INT, {0x79D9}, {0x1}}, \
    {"DDR_LAST",        ELEM_OUTPUT_INT, {0x79DA}, {0x1}}, \
    {"DDR_CURRENT",     ELEM_OUTPUT_INT, {0x79DB}, {0x1}}, \
    {"DDR_TARGET",      ELEM_OUTPUT_INT, {0x79DC}, {0x1}}, \
    {"DDR_DN_LIMIT",    ELEM_OUTPUT_INT, {0x79DD}, {0x1}}, \
    {"DDR_UP_LIMIT",    ELEM_OUTPUT_INT, {0x79DE}, {0x1}}, \
    {"DDR_PLL",         ELEM_OUTPUT_INT, {0x79DF}, {0x1}}, \
    {"DDR_LAST_PLL",    ELEM_OUTPUT_INT, {0x79E0}, {0x4}}, \
    {"CMD_CNT",         ELEM_OUTPUT_HEX, {0x79E4}, {0x10}}, \
    {"DATA_CNT",        ELEM_OUTPUT_INT, {0x79F4}, {0x4}}, \
    {"**AVS*******",    ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"AICORE0_HIGHTEMP_VOLT",  ELEM_OUTPUT_INT, {0x7AE0}, {0x4}}, \
    {"AICORE1_HIGHTEMP_VOLT",  ELEM_OUTPUT_INT, {0x7AE4}, {0x4}}, \
    {"CPU_HIGHTEMP_VOLT",      ELEM_OUTPUT_INT, {0x7AE8}, {0x4}}, \
    {"AIMEMORY_HIGHTEMP_VOLT", ELEM_OUTPUT_INT, {0x7AEC}, {0x4}}, \
    {"PERI_HIGHTEMP_VOLT",     ELEM_OUTPUT_INT, {0x7AF0}, {0x4}}, \
    {"AICORE0_CUR_VOLT",       ELEM_OUTPUT_INT, {0x7AF4}, {0x4}}, \
    {"AICORE1_CUR_VOLT",       ELEM_OUTPUT_INT, {0x7AF8}, {0x4}}, \
    {"CPU_CUR_VOLT",           ELEM_OUTPUT_INT, {0x7AFC}, {0x4}}, \
    {"AIMEMORY_CUR_VOLT",      ELEM_OUTPUT_INT, {0x7B00}, {0x4}}, \
    {"PERI_CUR_VOLT",          ELEM_OUTPUT_INT, {0x7B04}, {0x4}}, \
    {"AICORE0_SVFD_VOLT",      ELEM_OUTPUT_INT, {0x7B08}, {0x4}}, \
    {"AICORE1_SVFD_VOLT",      ELEM_OUTPUT_INT, {0x7B0C}, {0x4}}, \
    {"AICORE0_SVFD_CPM",       ELEM_OUTPUT_INT, {0x7B10}, {0x2}}, \
    {"AICORE1_SVFD_CPM",       ELEM_OUTPUT_INT, {0x7B12}, {0x2}}, \
    {"AICORE0_NOTIFY_ST",      ELEM_OUTPUT_INT, {0x7B14}, {0x1}}, \
    {"AICORE1_NOTIFY_ST",      ELEM_OUTPUT_INT, {0x7B15}, {0x1}}, \
    {"CPU_NOTIFY_ST",          ELEM_OUTPUT_INT, {0x7B16}, {0x1}}, \
    {"AIMEMORY_NOTIFY_ST",     ELEM_OUTPUT_INT, {0x7B17}, {0x1}}, \
    {"PERI_NOTIFY_ST",         ELEM_OUTPUT_INT, {0x7B18}, {0x1}}, \
    {"AICORE0_TZONE",          ELEM_OUTPUT_INT, {0x7B19}, {0x1}}, \
    {"AICORE1_TZONE",          ELEM_OUTPUT_INT, {0x7B1A}, {0x1}}, \
    {"CPU_TZONE",              ELEM_OUTPUT_INT, {0x7B1B}, {0x1}}, \
    {"AIMEMORY_TZONE",         ELEM_OUTPUT_INT, {0x7B1C}, {0x1}}, \
    {"PERI_TZONE",             ELEM_OUTPUT_INT, {0x7B1D}, {0x1}}, \
    {"VOLT_RISE_TEMP",         ELEM_OUTPUT_INT, {0x7B1E}, {0x1}}, \
    {"VOLT_DECREASE_TEMP",     ELEM_OUTPUT_INT, {0x7B1F}, {0x1}}, \
    {"**efuse*******",         ELEM_OUTPUT_STR, {0x00}, {0x8}}, \
    {"TSENSOR_EFUSE",          ELEM_OUTPUT_INT, {0x7B28}, {0x8}}, \
}

#define DATA_MODEL_LPM3_SRAM  MODEL_VECTOR(LPM3_SRAM) = { \
    {"magic_begin",             ELEM_OUTPUT_INT, {0x5000}, {0x4}}, \
    {"slice_time",              ELEM_OUTPUT_INT, {0x5004}, {0x4}}, \
    {"mod_reason",              ELEM_OUTPUT_INT, {0x5008}, {0x4}}, \
    {"ddr_freq_id",             ELEM_OUTPUT_INT, {0x500C}, {0x4}}, \
    {"uce_exc",                 ELEM_OUTPUT_INT, {0x5010}, {0x4}}, \
    {"reserved0-2",             ELEM_OUTPUT_INT, {0x5014}, {0x4}}, \
    {"reg_save_addr",           ELEM_OUTPUT_INT, {0x5020}, {0x4}}, \
    {"DDRRETENTION",            ELEM_OUTPUT_INT, {0x5024}, {0x4}}, \
    {"DDRRETENTIONCLR",         ELEM_OUTPUT_INT, {0x5028}, {0x4}}, \
    {"DRAMRETENTION",           ELEM_OUTPUT_INT, {0x502C}, {0x4}}, \
    {"DDRC_0_3_RESET",          ELEM_OUTPUT_INT, {0x5030}, {0x4}}, \
    {"DDRC_4_7_RESET",          ELEM_OUTPUT_INT, {0x5034}, {0x4}}, \
    {"DDRC_0_3_PACK_RESET",     ELEM_OUTPUT_INT, {0x5038}, {0x4}}, \
    {"DDRC_4_7_PACK_RESET",     ELEM_OUTPUT_INT, {0x503C}, {0x4}}, \
    {"DDRC_EXMBIST0_REGS_RESET",ELEM_OUTPUT_INT, {0x5040}, {0x4}}, \
    {"DDRC_EXMBIST1_REGS_RESET",ELEM_OUTPUT_INT, {0x5044}, {0x4}}, \
    {"DDRC_0_3_PACK_RESET",     ELEM_OUTPUT_INT, {0x5048}, {0x4}}, \
    {"DDRC_4_7_PACK_RESET",     ELEM_OUTPUT_INT, {0x504C}, {0x4}}, \
    {"SCTRL_DDRC_0_3_AO_RST",   ELEM_OUTPUT_INT, {0x5050}, {0x4}}, \
    {"SCTRL_DDRC_4_7_AO_RST",   ELEM_OUTPUT_INT, {0x5054}, {0x4}}, \
    {"PPLLBYPASS0",             ELEM_OUTPUT_INT, {0x5058}, {0x4}}, \
    {"PPLLBYPASS1",             ELEM_OUTPUT_INT, {0x505C}, {0x4}}, \
    {"PPLL3FCTRL",              ELEM_OUTPUT_INT, {0x5060}, {0x4}}, \
    {"PPLL3FCTRL_FRAC",         ELEM_OUTPUT_INT, {0x5064}, {0x4}}, \
    {"PPLL4FCTRL",              ELEM_OUTPUT_INT, {0x5068}, {0x4}}, \
    {"PPLL4FCTRL_FRAC",         ELEM_OUTPUT_INT, {0x506C}, {0x4}}, \
    {"PPLLOCKSTATUS",           ELEM_OUTPUT_INT, {0x5070}, {0x4}}, \
    {"DDRC_0_3_BYPASS_MODE",    ELEM_OUTPUT_INT, {0x5074}, {0x4}}, \
    {"DDRC_4_7_BYPASS_MODE",    ELEM_OUTPUT_INT, {0x5078}, {0x4}}, \
    {"PLL_PROF_CFG1",           ELEM_OUTPUT_INT, {0x507C}, {0x4}}, \
}

/* TEE module */
#define DATA_MODEL_TEE MODEL_VECTOR(TEE) = { \
    {"tee info", ELEM_OUTPUT_STR_NL, {0x0}, {0x10000}}, \
}

/* TF module */
#define DATA_MODEL_TF MODEL_VECTOR(TF) = { \
    {"tf info", ELEM_OUTPUT_STR_NL, {0x8}, {0xFFF8}}, \
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
    {"LPM3_WAKE_UP_STATUS",            ELEM_OUTPUT_INT, {0x0},  {0x4}}, \
    {"DEBUG_TIME_POWERUP_DONE",        ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"DEBUG_TIME_PERSTHIGH_DONE",      ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"DEBUG_TIME_PCIEPHY_DONE",        ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"DEBUG_TIME_PHY_FIRMWARE_DONE",   ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"DEBUG_TIME_PCIECTRL_DONE",       ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"DEBUG_TIME_IMG_DONE",            ELEM_OUTPUT_INT, {0x3C}, {0x4}}, \
    {"DEBUG_TIME_SECURE_DONE",         ELEM_OUTPUT_INT, {0x40}, {0x4}}, \
    {"DEBUG_VERSION_ADDR",             ELEM_OUTPUT_HEX, {0x50}, {0x10}}, \
    {"XLOADER_RESET_REG",              ELEM_OUTPUT_INT, {0x200}, {0x4}}, \
    {"XLOADER_KEY_POINT",              ELEM_OUTPUT_INT, {0x204}, {0x4}}, \
    {"XLOADER_TIME_POWERUP_DONE",      ELEM_OUTPUT_INT, {0x228}, {0x4}}, \
    {"XLOADER_TIME_PERSTHIGH_DONE",    ELEM_OUTPUT_INT, {0x22C}, {0x4}}, \
    {"XLOADER_TIME_PCIEPHY_DONE",      ELEM_OUTPUT_INT, {0x230}, {0x4}}, \
    {"XLOADER_TIME_PHY_FIRMWARE_DONE", ELEM_OUTPUT_INT, {0x234}, {0x4}}, \
    {"XLOADER_TIME_PCIECTRL_DONE",     ELEM_OUTPUT_INT, {0x238}, {0x4}}, \
    {"XLOADER_TIME_PCIE_DETECT_DONE",  ELEM_OUTPUT_INT, {0x23C}, {0x4}}, \
    {"UEFI_LAST_KEYPOINT",             ELEM_OUTPUT_INT, {0x320}, {0x4}}, \
    {"SD_LOAD_FILE_STATUS",            ELEM_OUTPUT_INT, {0x350}, {0x4}}, \
}


/* DDR_SRAM module */
#define DATA_MODEL_DDR_SRAM MODEL_VECTOR(DDR_SRAM) = {\
    {"magic_begin", ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"slice_time", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"mod_reason", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"ddr_freq_id", ELEM_OUTPUT_INT, {0xC}, {0x4}}, \
    {"ddr_status", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"uce_exc", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"SC_TSENSOR_INFO_ADDR", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"SC_TSENSOR_AICORE_LIMIT", ELEM_OUTPUT_INT, {0x1C}, {0x4}}, \
    {"SC_TSENSOR_MAX_TEMP", ELEM_OUTPUT_INT, {0x20}, {0x4}}, \
    {"channel_mask", ELEM_OUTPUT_INT, {0x24}, {0x4}}, \
    {"channel_num", ELEM_OUTPUT_INT, {0x28}, {0x4}}, \
    {"rank_num", ELEM_OUTPUT_INT, {0x2C}, {0x4}}, \
    {"ddr_size", ELEM_OUTPUT_INT, {0x30}, {0x4}}, \
    {"manufactery_id", ELEM_OUTPUT_INT, {0x34}, {0x4}}, \
    {"iecc_cerr_thresh[0]", ELEM_OUTPUT_INT, {0x38}, {0x4}}, \
    {"iecc_cerr_thresh[1]", ELEM_OUTPUT_INT, {0x3C}, {0x4}}, \
    {"iecc_cerr_thresh[2]", ELEM_OUTPUT_INT, {0x40}, {0x4}}, \
    {"iecc_cerr_thresh[3]", ELEM_OUTPUT_INT, {0x44}, {0x4}}, \
    {"iecc_cerr_thresh[4]", ELEM_OUTPUT_INT, {0x48}, {0x4}}, \
    {"iecc_cerr_thresh[5]", ELEM_OUTPUT_INT, {0x4C}, {0x4}}, \
    {"iecc_cerr_thresh[6]", ELEM_OUTPUT_INT, {0x50}, {0x4}}, \
    {"iecc_cerr_thresh[7]", ELEM_OUTPUT_INT, {0x54}, {0x4}}, \
    {"iecc_ctrl[0]", ELEM_OUTPUT_INT, {0x58}, {0x4}}, \
    {"iecc_ctrl[1]", ELEM_OUTPUT_INT, {0x5C}, {0x4}}, \
    {"iecc_ctrl[2]", ELEM_OUTPUT_INT, {0x60}, {0x4}}, \
    {"iecc_ctrl[3]", ELEM_OUTPUT_INT, {0x64}, {0x4}}, \
    {"iecc_ctrl[4]", ELEM_OUTPUT_INT, {0x68}, {0x4}}, \
    {"iecc_ctrl[5]", ELEM_OUTPUT_INT, {0x6C}, {0x4}}, \
    {"iecc_ctrl[6]", ELEM_OUTPUT_INT, {0x70}, {0x4}}, \
    {"iecc_ctrl[7]", ELEM_OUTPUT_INT, {0x74}, {0x4}}, \
    {"iecc_cerr_cnt[0]", ELEM_OUTPUT_INT, {0x78}, {0x4}}, \
    {"iecc_cerr_cnt[1]", ELEM_OUTPUT_INT, {0x7C}, {0x4}}, \
    {"iecc_cerr_cnt[2]", ELEM_OUTPUT_INT, {0x80}, {0x4}}, \
    {"iecc_cerr_cnt[3]", ELEM_OUTPUT_INT, {0x84}, {0x4}}, \
    {"iecc_cerr_cnt[4]", ELEM_OUTPUT_INT, {0x88}, {0x4}}, \
    {"iecc_cerr_cnt[5]", ELEM_OUTPUT_INT, {0x8C}, {0x4}}, \
    {"iecc_cerr_cnt[6]", ELEM_OUTPUT_INT, {0x90}, {0x4}}, \
    {"iecc_cerr_cnt[7]", ELEM_OUTPUT_INT, {0x94}, {0x4}}, \
    {"iecc_uerr_cnt[0]", ELEM_OUTPUT_INT, {0x98}, {0x4}}, \
    {"iecc_uerr_cnt[1]", ELEM_OUTPUT_INT, {0x9C}, {0x4}}, \
    {"iecc_uerr_cnt[2]", ELEM_OUTPUT_INT, {0xA0}, {0x4}}, \
    {"iecc_uerr_cnt[3]", ELEM_OUTPUT_INT, {0xA4}, {0x4}}, \
    {"iecc_uerr_cnt[4]", ELEM_OUTPUT_INT, {0xA8}, {0x4}}, \
    {"iecc_uerr_cnt[5]", ELEM_OUTPUT_INT, {0xAC}, {0x4}}, \
    {"iecc_uerr_cnt[6]", ELEM_OUTPUT_INT, {0xB0}, {0x4}}, \
    {"iecc_uerr_cnt[7]", ELEM_OUTPUT_INT, {0xB4}, {0x4}}, \
    {"magic_byte", ELEM_OUTPUT_INT, {0x100}, {0x1}}, \
    {"err_max", ELEM_OUTPUT_INT, {0x104}, {0x1}}, \
    {"irq_count", ELEM_OUTPUT_INT, {0x108}, {0x1}}, \
    {"index", ELEM_OUTPUT_INT, {0x10C}, {0x1}}, \
    {"rate[0].time", ELEM_OUTPUT_INT, {0x100}, {0x4}}, \
    {"rate[0].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x104}, {0x1}}, \
    {"rate[0].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x105}, {0x1}}, \
    {"rate[0].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x106}, {0x1}}, \
    {"rate[0].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x107}, {0x1}}, \
    {"rate[0].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x108}, {0x1}}, \
    {"rate[0].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x109}, {0x1}}, \
    {"rate[0].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x10A}, {0x1}}, \
    {"rate[0].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x10B}, {0x1}}, \
    {"rate[0].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x10C}, {0x1}}, \
    {"rate[0].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x10D}, {0x1}}, \
    {"rate[0].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x10E}, {0x1}}, \
    {"rate[0].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x10F}, {0x1}}, \
    {"rate[0].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x110}, {0x1}}, \
    {"rate[0].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x111}, {0x1}}, \
    {"rate[0].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x112}, {0x1}}, \
    {"rate[0].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x113}, {0x1}}, \
    {"rate[1].time", ELEM_OUTPUT_INT, {0x114}, {0x4}}, \
    {"rate[1].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x118}, {0x1}}, \
    {"rate[1].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x119}, {0x1}}, \
    {"rate[1].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x11A}, {0x1}}, \
    {"rate[1].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x11B}, {0x1}}, \
    {"rate[1].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x11C}, {0x1}}, \
    {"rate[1].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x11D}, {0x1}}, \
    {"rate[1].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x11E}, {0x1}}, \
    {"rate[1].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x11F}, {0x1}}, \
    {"rate[1].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x120}, {0x1}}, \
    {"rate[1].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x121}, {0x1}}, \
    {"rate[1].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x122}, {0x1}}, \
    {"rate[1].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x123}, {0x1}}, \
    {"rate[1].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x124}, {0x1}}, \
    {"rate[1].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x125}, {0x1}}, \
    {"rate[1].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x126}, {0x1}}, \
    {"rate[1].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x127}, {0x1}}, \
    {"rate[2].time", ELEM_OUTPUT_INT, {0x128}, {0x4}}, \
    {"rate[2].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x12C}, {0x1}}, \
    {"rate[2].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x12D}, {0x1}}, \
    {"rate[2].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x12E}, {0x1}}, \
    {"rate[2].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x12F}, {0x1}}, \
    {"rate[2].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x130}, {0x1}}, \
    {"rate[2].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x131}, {0x1}}, \
    {"rate[2].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x132}, {0x1}}, \
    {"rate[2].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x133}, {0x1}}, \
    {"rate[2].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x134}, {0x1}}, \
    {"rate[2].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x135}, {0x1}}, \
    {"rate[2].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x136}, {0x1}}, \
    {"rate[2].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x137}, {0x1}}, \
    {"rate[2].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x138}, {0x1}}, \
    {"rate[2].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x139}, {0x1}}, \
    {"rate[2].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x13A}, {0x1}}, \
    {"rate[2].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x13B}, {0x1}}, \
    {"rate[3].time", ELEM_OUTPUT_INT, {0x13C}, {0x4}}, \
    {"rate[3].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x140}, {0x1}}, \
    {"rate[3].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x141}, {0x1}}, \
    {"rate[3].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x142}, {0x1}}, \
    {"rate[3].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x143}, {0x1}}, \
    {"rate[3].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x144}, {0x1}}, \
    {"rate[3].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x145}, {0x1}}, \
    {"rate[3].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x146}, {0x1}}, \
    {"rate[3].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x147}, {0x1}}, \
    {"rate[3].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x148}, {0x1}}, \
    {"rate[3].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x149}, {0x1}}, \
    {"rate[3].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x14A}, {0x1}}, \
    {"rate[3].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x14B}, {0x1}}, \
    {"rate[3].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x14C}, {0x1}}, \
    {"rate[3].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x14D}, {0x1}}, \
    {"rate[3].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x14E}, {0x1}}, \
    {"rate[3].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x14F}, {0x1}}, \
    {"rate[4].time", ELEM_OUTPUT_INT, {0x150}, {0x4}}, \
    {"rate[4].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x154}, {0x1}}, \
    {"rate[4].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x155}, {0x1}}, \
    {"rate[4].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x156}, {0x1}}, \
    {"rate[4].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x157}, {0x1}}, \
    {"rate[4].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x158}, {0x1}}, \
    {"rate[4].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x159}, {0x1}}, \
    {"rate[4].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x15A}, {0x1}}, \
    {"rate[4].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x15B}, {0x1}}, \
    {"rate[4].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x15C}, {0x1}}, \
    {"rate[4].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x15D}, {0x1}}, \
    {"rate[4].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x15E}, {0x1}}, \
    {"rate[4].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x15F}, {0x1}}, \
    {"rate[4].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x160}, {0x1}}, \
    {"rate[4].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x161}, {0x1}}, \
    {"rate[4].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x162}, {0x1}}, \
    {"rate[4].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x163}, {0x1}}, \
    {"rate[5].time", ELEM_OUTPUT_INT, {0x164}, {0x4}}, \
    {"rate[5].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x168}, {0x1}}, \
    {"rate[5].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x169}, {0x1}}, \
    {"rate[5].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x16A}, {0x1}}, \
    {"rate[5].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x16B}, {0x1}}, \
    {"rate[5].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x16C}, {0x1}}, \
    {"rate[5].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x16D}, {0x1}}, \
    {"rate[5].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x16E}, {0x1}}, \
    {"rate[5].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x16F}, {0x1}}, \
    {"rate[5].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x170}, {0x1}}, \
    {"rate[5].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x171}, {0x1}}, \
    {"rate[5].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x172}, {0x1}}, \
    {"rate[5].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x173}, {0x1}}, \
    {"rate[5].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x174}, {0x1}}, \
    {"rate[5].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x175}, {0x1}}, \
    {"rate[5].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x176}, {0x1}}, \
    {"rate[5].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x177}, {0x1}}, \
    {"rate[6].time", ELEM_OUTPUT_INT, {0x178}, {0x4}}, \
    {"rate[6].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x17C}, {0x1}}, \
    {"rate[6].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x17D}, {0x1}}, \
    {"rate[6].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x17E}, {0x1}}, \
    {"rate[6].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x17F}, {0x1}}, \
    {"rate[6].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x180}, {0x1}}, \
    {"rate[6].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x181}, {0x1}}, \
    {"rate[6].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x182}, {0x1}}, \
    {"rate[6].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x183}, {0x1}}, \
    {"rate[6].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x184}, {0x1}}, \
    {"rate[6].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x185}, {0x1}}, \
    {"rate[6].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x186}, {0x1}}, \
    {"rate[6].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x187}, {0x1}}, \
    {"rate[6].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x188}, {0x1}}, \
    {"rate[6].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x189}, {0x1}}, \
    {"rate[6].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x18A}, {0x1}}, \
    {"rate[6].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x18B}, {0x1}}, \
    {"rate[7].time", ELEM_OUTPUT_INT, {0x18C}, {0x4}}, \
    {"rate[7].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x190}, {0x1}}, \
    {"rate[7].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x191}, {0x1}}, \
    {"rate[7].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x192}, {0x1}}, \
    {"rate[7].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x193}, {0x1}}, \
    {"rate[7].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x194}, {0x1}}, \
    {"rate[7].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x195}, {0x1}}, \
    {"rate[7].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x196}, {0x1}}, \
    {"rate[7].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x197}, {0x1}}, \
    {"rate[7].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x198}, {0x1}}, \
    {"rate[7].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x199}, {0x1}}, \
    {"rate[7].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x19A}, {0x1}}, \
    {"rate[7].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x19B}, {0x1}}, \
    {"rate[7].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x19C}, {0x1}}, \
    {"rate[7].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x19D}, {0x1}}, \
    {"rate[7].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x19E}, {0x1}}, \
    {"rate[7].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x1AF}, {0x1}}, \
    {"rate[8].time", ELEM_OUTPUT_INT, {0x1B0}, {0x4}}, \
    {"rate[8].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x1B4}, {0x1}}, \
    {"rate[8].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x1B5}, {0x1}}, \
    {"rate[8].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x1B6}, {0x1}}, \
    {"rate[8].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x1B7}, {0x1}}, \
    {"rate[8].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x1B8}, {0x1}}, \
    {"rate[8].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x1B9}, {0x1}}, \
    {"rate[8].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x1BA}, {0x1}}, \
    {"rate[8].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x1BB}, {0x1}}, \
    {"rate[8].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x1BC}, {0x1}}, \
    {"rate[8].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x1BD}, {0x1}}, \
    {"rate[8].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x1BE}, {0x1}}, \
    {"rate[8].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x1BF}, {0x1}}, \
    {"rate[8].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x1C0}, {0x1}}, \
    {"rate[8].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x1C1}, {0x1}}, \
    {"rate[8].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x1C2}, {0x1}}, \
    {"rate[8].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x1C3}, {0x1}}, \
    {"rate[9].time", ELEM_OUTPUT_INT, {0x1C4}, {0x4}}, \
    {"rate[9].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x1C8}, {0x1}}, \
    {"rate[9].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x1C9}, {0x1}}, \
    {"rate[9].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x1CA}, {0x1}}, \
    {"rate[9].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x1CB}, {0x1}}, \
    {"rate[9].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x1CC}, {0x1}}, \
    {"rate[9].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x1CD}, {0x1}}, \
    {"rate[9].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x1CE}, {0x1}}, \
    {"rate[9].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x1CF}, {0x1}}, \
    {"rate[9].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x1D0}, {0x1}}, \
    {"rate[9].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x1D1}, {0x1}}, \
    {"rate[9].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x1D2}, {0x1}}, \
    {"rate[9].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x1D3}, {0x1}}, \
    {"rate[9].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x1D4}, {0x1}}, \
    {"rate[9].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x1D5}, {0x1}}, \
    {"rate[9].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x1D6}, {0x1}}, \
    {"rate[9].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x1D7}, {0x1}}, \
    {"mrr4_0.count", ELEM_OUTPUT_INT, {0x1D8}, {0x1}}, \
    {"mrr4_0.happen", ELEM_OUTPUT_INT, {0x1D9}, {0x1}}, \
    {"shake_count.count", ELEM_OUTPUT_INT, {0x1DA}, {0x1}}, \
    {"shake_count.happen", ELEM_OUTPUT_INT, {0x1DB}, {0x1}}, \
    {"sfc_record0", ELEM_OUTPUT_INT, {0x1DC}, {0x1}}, \
    {"sfc_record1", ELEM_OUTPUT_INT, {0x1DD}, {0x1}}, \
    {"sfc_mr5", ELEM_OUTPUT_INT, {0x1DE}, {0x1}}, \
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

#define DATA_MODEL_HDR_RUN_LPM MODEL_VECTOR(HDR_RUN_LPM) = { \
    {"magic",  ELEM_OUTPUT_INT, {0x0}, {0x4}}, \
    {"version", ELEM_OUTPUT_INT, {0x4}, {0x4}}, \
    {"module id", ELEM_OUTPUT_INT, {0x8}, {0x4}}, \
    {"if", ELEM_CTRL_COMPARE, {0xC}, {0x4}}, \
    {"is used", ELEM_CTRL_CMP_JUMP_NE, {0x1}, {0x200}}, \
    {"err code", ELEM_OUTPUT_INT, {0x10}, {0x4}}, \
    {"reason", ELEM_OUTPUT_INT, {0x14}, {0x4}}, \
    {"hot reset index", ELEM_OUTPUT_INT, {0x18}, {0x4}}, \
    {"[LPM log]", ELEM_OUTPUT_STR_NL, {0x40}, {0x400}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, \
    {"[LPM data]:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"reset_reason", ELEM_OUTPUT_INT, {0x440}, {0x4}}, \
    {"slice", ELEM_OUTPUT_INT, {0x444}, {0x4}}, \
    {"rtc", ELEM_OUTPUT_INT, {0x448}, {0x4}}, \
    {"r13", ELEM_OUTPUT_INT, {0x44C}, {0x4}}, \
    {"lr1", ELEM_OUTPUT_INT, {0x450}, {0x4}}, \
    {"pc", ELEM_OUTPUT_INT, {0x454}, {0x4}}, \
    {"xpsr", ELEM_OUTPUT_INT, {0x458}, {0x4}}, \
    {"cfsr", ELEM_OUTPUT_INT, {0x45C}, {0x4}}, \
    {"hfsr", ELEM_OUTPUT_INT, {0x460}, {0x4}}, \
    {"bfar", ELEM_OUTPUT_INT, {0x464}, {0x4}}, \
    {"exc_trace", ELEM_OUTPUT_INT, {0x468}, {0x1}}, \
    {"ddr_exc", ELEM_OUTPUT_INT, {0x469}, {0x1}}, \
    {"irq_id", ELEM_OUTPUT_INT, {0x46A}, {0x2}}, \
    {"task_id", ELEM_OUTPUT_INT, {0x46C}, {0x4}}, \
    {"TSENSOR_A55", ELEM_OUTPUT_INT, {0x470}, {0x1}}, \
    {"TSENSOR_PERI", ELEM_OUTPUT_INT, {0x471}, {0x1}}, \
    {"TSENSOR_AIC0", ELEM_OUTPUT_INT, {0x472}, {0x1}}, \
    {"TSENSOR_AIC1", ELEM_OUTPUT_INT, {0x473}, {0x1}}, \
    {"DDR_T_GRADE", ELEM_OUTPUT_INT, {0x474}, {0x1}}, \
    {"EDP_SCALE_0", ELEM_OUTPUT_INT, {0x475}, {0x1}}, \
    {"EDP_SCALE_1", ELEM_OUTPUT_INT, {0x476}, {0x1}}, \
    {"TMP_STATUS", ELEM_OUTPUT_INT, {0x477}, {0x1}}, \
    {"TMP_CTRL_ST", ELEM_OUTPUT_INT, {0x478}, {0x1}}, \
    {"AIC_FREQ_ST", ELEM_OUTPUT_INT, {0x479}, {0x1}}, \
    {"A55_FREQ_ST", ELEM_OUTPUT_INT, {0x47a}, {0x1}}, \
    {"AIC_NUM_ST", ELEM_OUTPUT_INT, {0x47b}, {0x1}}, \
    {"TMP_RST", ELEM_OUTPUT_INT, {0x47c}, {0x1}}, \
    {"TMP_HIGH", ELEM_OUTPUT_INT, {0x47d}, {0x1}}, \
    {"TMP_NOR", ELEM_OUTPUT_INT, {0x47e}, {0x1}}, \
    {"TMP_PERIOD", ELEM_OUTPUT_INT, {0x47f}, {0x1}}, \
    {"T_RST_STATUS", ELEM_OUTPUT_INT, {0x48D}, {0x1}}, \
    {"T_ERR_TSENSOR", ELEM_OUTPUT_INT, {0x48e}, {0x1}}, \
    {"T_ERR_EFUSE", ELEM_OUTPUT_INT, {0x48f}, {0x1}}, \
    {"AICORE0_HIGHTEMP_VOLT",     ELEM_OUTPUT_INT, {0x490}, {0x4}}, \
    {"AICORE1_HIGHTEMP_VOLT",     ELEM_OUTPUT_INT, {0x494}, {0x4}}, \
    {"CPU_HIGHTEMP_VOLT",     ELEM_OUTPUT_INT, {0x498}, {0x4}}, \
    {"AIMEMORY_HIGHTEMP_VOLT",     ELEM_OUTPUT_INT, {0x49c}, {0x4}}, \
    {"PERI_HIGHTEMP_VOLT",     ELEM_OUTPUT_INT, {0x4a0}, {0x4}}, \
    {"AICORE0_CUR_VOLT",     ELEM_OUTPUT_INT, {0x4a4}, {0x4}}, \
    {"AICORE1_CUR_VOLT",     ELEM_OUTPUT_INT, {0x4a8}, {0x4}}, \
    {"CPU_CUR_VOLT",     ELEM_OUTPUT_INT, {0x4ac}, {0x4}}, \
    {"AIMEMORY_CUR_VOLT",     ELEM_OUTPUT_INT, {0x4b0}, {0x4}}, \
    {"PERI_CUR_VOLT",     ELEM_OUTPUT_INT, {0x4b4}, {0x4}}, \
    {"AICORE0_SVFD_VOLT",     ELEM_OUTPUT_INT, {0x4b8}, {0x4}}, \
    {"AICORE1_SVFD_VOLT",     ELEM_OUTPUT_INT, {0x4bc}, {0x4}}, \
    {"AICORE0_SVFD_CPM",     ELEM_OUTPUT_INT, {0x4c0}, {0x2}}, \
    {"AICORE1_SVFD_CPM",     ELEM_OUTPUT_INT, {0x4c2}, {0x2}}, \
    {"AICORE0_NOTIFY_ST",     ELEM_OUTPUT_INT, {0x4c4}, {0x1}}, \
    {"AICORE1_NOTIFY_ST",     ELEM_OUTPUT_INT, {0x4c5}, {0x1}}, \
    {"CPU_NOTIFY_ST",     ELEM_OUTPUT_INT, {0x4c6}, {0x1}}, \
    {"AIMEMORY_NOTIFY_ST",     ELEM_OUTPUT_INT, {0x4c7}, {0x1}}, \
    {"PERI_NOTIFY_ST",     ELEM_OUTPUT_INT, {0x4c8}, {0x1}}, \
    {"AICORE0_TZONE",     ELEM_OUTPUT_INT, {0x4c9}, {0x1}}, \
    {"AICORE1_TZONE",     ELEM_OUTPUT_INT, {0x4ca}, {0x1}}, \
    {"CPU_TZONE",     ELEM_OUTPUT_INT, {0x4cb}, {0x1}}, \
    {"AIMEMORY_TZONE",     ELEM_OUTPUT_INT, {0x4cc}, {0x1}}, \
    {"PERI_TZONE",     ELEM_OUTPUT_INT, {0x4cd}, {0x1}}, \
    {"VOLT_RISE_TEMP",     ELEM_OUTPUT_INT, {0x4ce}, {0x1}}, \
    {"VOLT_DECREASE_TEMP",     ELEM_OUTPUT_INT, {0x4cf}, {0x1}}, \
    {"NL", ELEM_OUTPUT_NL, {0x0}, {0x0}}, /* below for ddr */ \
    {"[DDR data]:", ELEM_OUTPUT_STR_CONST, {0x0}, {0x0}}, \
    {"magic_begin", ELEM_OUTPUT_INT, {0x800}, {0x4}}, \
    {"slice_time", ELEM_OUTPUT_INT, {0x804}, {0x4}}, \
    {"mod_reason", ELEM_OUTPUT_INT, {0x808}, {0x4}}, \
    {"ddr_freq_id", ELEM_OUTPUT_INT, {0x80C}, {0x4}}, \
    {"ddr_status", ELEM_OUTPUT_INT, {0x810}, {0x4}}, \
    {"uce_exc", ELEM_OUTPUT_INT, {0x814}, {0x4}}, \
    {"SC_TSENSOR_INFO_ADDR", ELEM_OUTPUT_INT, {0x818}, {0x4}}, \
    {"SC_TSENSOR_AICORE_LIMIT", ELEM_OUTPUT_INT, {0x81C}, {0x4}}, \
    {"SC_TSENSOR_MAX_TEMP", ELEM_OUTPUT_INT, {0x820}, {0x4}}, \
    {"channel_mask", ELEM_OUTPUT_INT, {0x824}, {0x4}}, \
    {"channel_num", ELEM_OUTPUT_INT, {0x828}, {0x4}}, \
    {"rank_num", ELEM_OUTPUT_INT, {0x82C}, {0x4}}, \
    {"ddr_size", ELEM_OUTPUT_INT, {0x830}, {0x4}}, \
    {"manufactery_id", ELEM_OUTPUT_INT, {0x834}, {0x4}}, \
    {"iecc_cerr_thresh[0]", ELEM_OUTPUT_INT, {0x838}, {0x4}}, \
    {"iecc_cerr_thresh[1]", ELEM_OUTPUT_INT, {0x83C}, {0x4}}, \
    {"iecc_cerr_thresh[2]", ELEM_OUTPUT_INT, {0x840}, {0x4}}, \
    {"iecc_cerr_thresh[3]", ELEM_OUTPUT_INT, {0x844}, {0x4}}, \
    {"iecc_cerr_thresh[4]", ELEM_OUTPUT_INT, {0x848}, {0x4}}, \
    {"iecc_cerr_thresh[5]", ELEM_OUTPUT_INT, {0x84C}, {0x4}}, \
    {"iecc_cerr_thresh[6]", ELEM_OUTPUT_INT, {0x850}, {0x4}}, \
    {"iecc_cerr_thresh[7]", ELEM_OUTPUT_INT, {0x854}, {0x4}}, \
    {"iecc_ctrl[0]", ELEM_OUTPUT_INT, {0x858}, {0x4}}, \
    {"iecc_ctrl[1]", ELEM_OUTPUT_INT, {0x85C}, {0x4}}, \
    {"iecc_ctrl[2]", ELEM_OUTPUT_INT, {0x860}, {0x4}}, \
    {"iecc_ctrl[3]", ELEM_OUTPUT_INT, {0x864}, {0x4}}, \
    {"iecc_ctrl[4]", ELEM_OUTPUT_INT, {0x868}, {0x4}}, \
    {"iecc_ctrl[5]", ELEM_OUTPUT_INT, {0x86C}, {0x4}}, \
    {"iecc_ctrl[6]", ELEM_OUTPUT_INT, {0x870}, {0x4}}, \
    {"iecc_ctrl[7]", ELEM_OUTPUT_INT, {0x874}, {0x4}}, \
    {"iecc_cerr_cnt[0]", ELEM_OUTPUT_INT, {0x878}, {0x4}}, \
    {"iecc_cerr_cnt[1]", ELEM_OUTPUT_INT, {0x87C}, {0x4}}, \
    {"iecc_cerr_cnt[2]", ELEM_OUTPUT_INT, {0x880}, {0x4}}, \
    {"iecc_cerr_cnt[3]", ELEM_OUTPUT_INT, {0x884}, {0x4}}, \
    {"iecc_cerr_cnt[4]", ELEM_OUTPUT_INT, {0x888}, {0x4}}, \
    {"iecc_cerr_cnt[5]", ELEM_OUTPUT_INT, {0x88C}, {0x4}}, \
    {"iecc_cerr_cnt[6]", ELEM_OUTPUT_INT, {0x890}, {0x4}}, \
    {"iecc_cerr_cnt[7]", ELEM_OUTPUT_INT, {0x894}, {0x4}}, \
    {"iecc_uerr_cnt[0]", ELEM_OUTPUT_INT, {0x898}, {0x4}}, \
    {"iecc_uerr_cnt[1]", ELEM_OUTPUT_INT, {0x89C}, {0x4}}, \
    {"iecc_uerr_cnt[2]", ELEM_OUTPUT_INT, {0x8A0}, {0x4}}, \
    {"iecc_uerr_cnt[3]", ELEM_OUTPUT_INT, {0x8A4}, {0x4}}, \
    {"iecc_uerr_cnt[4]", ELEM_OUTPUT_INT, {0x8A8}, {0x4}}, \
    {"iecc_uerr_cnt[5]", ELEM_OUTPUT_INT, {0x8AC}, {0x4}}, \
    {"iecc_uerr_cnt[6]", ELEM_OUTPUT_INT, {0x8B0}, {0x4}}, \
    {"iecc_uerr_cnt[7]", ELEM_OUTPUT_INT, {0x8B4}, {0x4}}, \
    {"magic_byte", ELEM_OUTPUT_INT, {0x900}, {0x1}}, \
    {"err_max", ELEM_OUTPUT_INT, {0x904}, {0x1}}, \
    {"irq_count", ELEM_OUTPUT_INT, {0x908}, {0x1}}, \
    {"index", ELEM_OUTPUT_INT, {0x90C}, {0x1}}, \
    {"rate[0].time", ELEM_OUTPUT_INT, {0x900}, {0x4}}, \
    {"rate[0].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x904}, {0x1}}, \
    {"rate[0].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x905}, {0x1}}, \
    {"rate[0].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x906}, {0x1}}, \
    {"rate[0].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x907}, {0x1}}, \
    {"rate[0].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x908}, {0x1}}, \
    {"rate[0].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x909}, {0x1}}, \
    {"rate[0].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x90A}, {0x1}}, \
    {"rate[0].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x90B}, {0x1}}, \
    {"rate[0].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x90C}, {0x1}}, \
    {"rate[0].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x90D}, {0x1}}, \
    {"rate[0].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x90E}, {0x1}}, \
    {"rate[0].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x90F}, {0x1}}, \
    {"rate[0].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x910}, {0x1}}, \
    {"rate[0].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x911}, {0x1}}, \
    {"rate[0].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x912}, {0x1}}, \
    {"rate[0].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x913}, {0x1}}, \
    {"rate[1].time", ELEM_OUTPUT_INT, {0x914}, {0x4}}, \
    {"rate[1].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x918}, {0x1}}, \
    {"rate[1].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x919}, {0x1}}, \
    {"rate[1].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x91A}, {0x1}}, \
    {"rate[1].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x91B}, {0x1}}, \
    {"rate[1].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x91C}, {0x1}}, \
    {"rate[1].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x91D}, {0x1}}, \
    {"rate[1].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x91E}, {0x1}}, \
    {"rate[1].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x91F}, {0x1}}, \
    {"rate[1].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x920}, {0x1}}, \
    {"rate[1].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x921}, {0x1}}, \
    {"rate[1].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x922}, {0x1}}, \
    {"rate[1].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x923}, {0x1}}, \
    {"rate[1].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x924}, {0x1}}, \
    {"rate[1].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x925}, {0x1}}, \
    {"rate[1].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x926}, {0x1}}, \
    {"rate[1].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x927}, {0x1}}, \
    {"rate[2].time", ELEM_OUTPUT_INT, {0x928}, {0x4}}, \
    {"rate[2].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x92C}, {0x1}}, \
    {"rate[2].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x92D}, {0x1}}, \
    {"rate[2].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x92E}, {0x1}}, \
    {"rate[2].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x92F}, {0x1}}, \
    {"rate[2].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x930}, {0x1}}, \
    {"rate[2].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x931}, {0x1}}, \
    {"rate[2].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x932}, {0x1}}, \
    {"rate[2].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x933}, {0x1}}, \
    {"rate[2].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x934}, {0x1}}, \
    {"rate[2].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x935}, {0x1}}, \
    {"rate[2].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x936}, {0x1}}, \
    {"rate[2].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x937}, {0x1}}, \
    {"rate[2].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x938}, {0x1}}, \
    {"rate[2].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x939}, {0x1}}, \
    {"rate[2].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x93A}, {0x1}}, \
    {"rate[2].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x93B}, {0x1}}, \
    {"rate[3].time", ELEM_OUTPUT_INT, {0x93C}, {0x4}}, \
    {"rate[3].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x940}, {0x1}}, \
    {"rate[3].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x941}, {0x1}}, \
    {"rate[3].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x942}, {0x1}}, \
    {"rate[3].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x943}, {0x1}}, \
    {"rate[3].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x944}, {0x1}}, \
    {"rate[3].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x945}, {0x1}}, \
    {"rate[3].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x946}, {0x1}}, \
    {"rate[3].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x947}, {0x1}}, \
    {"rate[3].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x948}, {0x1}}, \
    {"rate[3].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x949}, {0x1}}, \
    {"rate[3].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x94A}, {0x1}}, \
    {"rate[3].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x94B}, {0x1}}, \
    {"rate[3].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x94C}, {0x1}}, \
    {"rate[3].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x94D}, {0x1}}, \
    {"rate[3].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x94E}, {0x1}}, \
    {"rate[3].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x94F}, {0x1}}, \
    {"rate[4].time", ELEM_OUTPUT_INT, {0x950}, {0x4}}, \
    {"rate[4].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x954}, {0x1}}, \
    {"rate[4].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x955}, {0x1}}, \
    {"rate[4].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x956}, {0x1}}, \
    {"rate[4].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x957}, {0x1}}, \
    {"rate[4].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x958}, {0x1}}, \
    {"rate[4].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x959}, {0x1}}, \
    {"rate[4].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x95A}, {0x1}}, \
    {"rate[4].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x95B}, {0x1}}, \
    {"rate[4].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x95C}, {0x1}}, \
    {"rate[4].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x95D}, {0x1}}, \
    {"rate[4].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x95E}, {0x1}}, \
    {"rate[4].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x95F}, {0x1}}, \
    {"rate[4].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x960}, {0x1}}, \
    {"rate[4].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x961}, {0x1}}, \
    {"rate[4].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x962}, {0x1}}, \
    {"rate[4].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x963}, {0x1}}, \
    {"rate[5].time", ELEM_OUTPUT_INT, {0x964}, {0x4}}, \
    {"rate[5].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x968}, {0x1}}, \
    {"rate[5].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x969}, {0x1}}, \
    {"rate[5].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x96A}, {0x1}}, \
    {"rate[5].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x96B}, {0x1}}, \
    {"rate[5].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x96C}, {0x1}}, \
    {"rate[5].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x96D}, {0x1}}, \
    {"rate[5].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x96E}, {0x1}}, \
    {"rate[5].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x96F}, {0x1}}, \
    {"rate[5].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x970}, {0x1}}, \
    {"rate[5].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x971}, {0x1}}, \
    {"rate[5].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x972}, {0x1}}, \
    {"rate[5].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x973}, {0x1}}, \
    {"rate[5].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x974}, {0x1}}, \
    {"rate[5].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x975}, {0x1}}, \
    {"rate[5].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x976}, {0x1}}, \
    {"rate[5].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x977}, {0x1}}, \
    {"rate[6].time", ELEM_OUTPUT_INT, {0x978}, {0x4}}, \
    {"rate[6].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x97C}, {0x1}}, \
    {"rate[6].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x97D}, {0x1}}, \
    {"rate[6].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x97E}, {0x1}}, \
    {"rate[6].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x97F}, {0x1}}, \
    {"rate[6].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x980}, {0x1}}, \
    {"rate[6].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x981}, {0x1}}, \
    {"rate[6].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x982}, {0x1}}, \
    {"rate[6].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x983}, {0x1}}, \
    {"rate[6].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x984}, {0x1}}, \
    {"rate[6].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x985}, {0x1}}, \
    {"rate[6].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x986}, {0x1}}, \
    {"rate[6].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x987}, {0x1}}, \
    {"rate[6].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x988}, {0x1}}, \
    {"rate[6].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x989}, {0x1}}, \
    {"rate[6].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x98A}, {0x1}}, \
    {"rate[6].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x98B}, {0x1}}, \
    {"rate[7].time", ELEM_OUTPUT_INT, {0x98C}, {0x4}}, \
    {"rate[7].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x990}, {0x1}}, \
    {"rate[7].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x991}, {0x1}}, \
    {"rate[7].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x992}, {0x1}}, \
    {"rate[7].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x993}, {0x1}}, \
    {"rate[7].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x994}, {0x1}}, \
    {"rate[7].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x995}, {0x1}}, \
    {"rate[7].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x996}, {0x1}}, \
    {"rate[7].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x997}, {0x1}}, \
    {"rate[7].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x998}, {0x1}}, \
    {"rate[7].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x999}, {0x1}}, \
    {"rate[7].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x99A}, {0x1}}, \
    {"rate[7].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x99B}, {0x1}}, \
    {"rate[7].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x99C}, {0x1}}, \
    {"rate[7].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x99D}, {0x1}}, \
    {"rate[7].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x99E}, {0x1}}, \
    {"rate[7].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x9AF}, {0x1}}, \
    {"rate[8].time", ELEM_OUTPUT_INT, {0x9B0}, {0x4}}, \
    {"rate[8].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x9B4}, {0x1}}, \
    {"rate[8].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x9B5}, {0x1}}, \
    {"rate[8].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x9B6}, {0x1}}, \
    {"rate[8].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x9B7}, {0x1}}, \
    {"rate[8].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x9B8}, {0x1}}, \
    {"rate[8].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x9B9}, {0x1}}, \
    {"rate[8].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x9BA}, {0x1}}, \
    {"rate[8].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x9BB}, {0x1}}, \
    {"rate[8].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x9BC}, {0x1}}, \
    {"rate[8].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x9BD}, {0x1}}, \
    {"rate[8].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x9BE}, {0x1}}, \
    {"rate[8].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x9BF}, {0x1}}, \
    {"rate[8].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x9C0}, {0x1}}, \
    {"rate[8].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x9C1}, {0x1}}, \
    {"rate[8].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x9C2}, {0x1}}, \
    {"rate[8].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x9C3}, {0x1}}, \
    {"rate[9].time", ELEM_OUTPUT_INT, {0x9C4}, {0x4}}, \
    {"rate[9].rate_per_rank[0]", ELEM_OUTPUT_INT, {0x9C8}, {0x1}}, \
    {"rate[9].rate_per_rank[1]", ELEM_OUTPUT_INT, {0x9C9}, {0x1}}, \
    {"rate[9].rate_per_rank[2]", ELEM_OUTPUT_INT, {0x9CA}, {0x1}}, \
    {"rate[9].rate_per_rank[3]", ELEM_OUTPUT_INT, {0x9CB}, {0x1}}, \
    {"rate[9].rate_per_rank[4]", ELEM_OUTPUT_INT, {0x9CC}, {0x1}}, \
    {"rate[9].rate_per_rank[5]", ELEM_OUTPUT_INT, {0x9CD}, {0x1}}, \
    {"rate[9].rate_per_rank[6]", ELEM_OUTPUT_INT, {0x9CE}, {0x1}}, \
    {"rate[9].rate_per_rank[7]", ELEM_OUTPUT_INT, {0x9CF}, {0x1}}, \
    {"rate[9].rate_per_rank[8]", ELEM_OUTPUT_INT, {0x9D0}, {0x1}}, \
    {"rate[9].rate_per_rank[9]", ELEM_OUTPUT_INT, {0x9D1}, {0x1}}, \
    {"rate[9].rate_per_rank[A]", ELEM_OUTPUT_INT, {0x9D2}, {0x1}}, \
    {"rate[9].rate_per_rank[B]", ELEM_OUTPUT_INT, {0x9D3}, {0x1}}, \
    {"rate[9].rate_per_rank[C]", ELEM_OUTPUT_INT, {0x9D4}, {0x1}}, \
    {"rate[9].rate_per_rank[D]", ELEM_OUTPUT_INT, {0x9D5}, {0x1}}, \
    {"rate[9].rate_per_rank[E]", ELEM_OUTPUT_INT, {0x9D6}, {0x1}}, \
    {"rate[9].rate_per_rank[F]", ELEM_OUTPUT_INT, {0x9D7}, {0x1}}, \
    {"mrr4_0.count", ELEM_OUTPUT_INT, {0x9D8}, {0x1}}, \
    {"mrr4_0.happen", ELEM_OUTPUT_INT, {0x9D9}, {0x1}}, \
    {"shake_count.count", ELEM_OUTPUT_INT, {0x9DA}, {0x1}}, \
    {"shake_count.happen", ELEM_OUTPUT_INT, {0x9DB}, {0x1}}, \
    {"sfc_record0", ELEM_OUTPUT_INT, {0x9DC}, {0x1}}, \
    {"sfc_record1", ELEM_OUTPUT_INT, {0x9DD}, {0x1}}, \
    {"sfc_mr5", ELEM_OUTPUT_INT, {0x9DE}, {0x1}}, \
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
    {"LPM INFO", ELEM_OUTPUT_DIVIDE, {0x0}, {0x2D}}, \
    {"HDR_RUN_LPM", ELEM_CTRL_TABLE_GOTO, {0x1000}, {0x1000}}, \
    {"table_index", ELEM_CTRL_TABLE_RANGE, {PLAINTEXT_TABLE_HDR_RUN_LPM}, {0x1}}, \
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

#endif // BBOX_DDR_DATA_MINI_H
