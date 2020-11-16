/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * @brief header file of compress weight
 *
 * @file compress_weight.h
 *
 * @version 1.0
 */
#ifndef COMPRESS_WEIGHT_H
#define COMPRESS_WEIGHT_H

#include "compress.h"

const int SHAPE_SIZE_WEIGHT = 4;

struct CompressOpConfig {
    int64_t wShape[SHAPE_SIZE_WEIGHT];
    size_t compressTilingK;
    size_t compressTilingN;
    struct CompressConfig compressConfig;
};

extern "C" CmpStatus CompressWeightsConv2D(const char *const input,
                                           char *const zipBuffer,
                                           char *const infoBuffer,
                                           CompressOpConfig *const param);
#endif // COMPRESS_WEIGHT_H
