/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Apache License Version 2.0.You may not use this file except in compliance with the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Apache License for more details at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * @brief compress header file
 *
 * @file compress.h
 *
 * @version 1.0
 */
#ifndef COMPRESS_H
#define COMPRESS_H

#include <uchar.h>

enum CmpStatus {
    RET_SUCCESS = 0,
    RET_ERROR = -1
};

struct CompressConfig {
    size_t inputSize; // length of data to compress
    size_t engineNum; // how many decompress engines
    size_t maxRatio; // how much size of a basic compression block, only 64 supported now (8x: 64 4x: 32)
    size_t channel; // channels of L2 or DDR. For load balance
    size_t fractalSize; // size of compressing block
    bool isTight; // whether compose compressed data tightly
    size_t init_offset;
};

CmpStatus CompressWeights(char* input,
                          const CompressConfig& compressConfig,
                          char* indexs,
                          char* output,
                          size_t& compressedLength);


#endif  // COMPRESS_H
