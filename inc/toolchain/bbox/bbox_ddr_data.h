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

#ifndef BBOX_DDR_DATA_H
#define BBOX_DDR_DATA_H

/* ELEM_OUTPUT_BIN
 * key:
 * 0x00000000 FFFF FFFF FFFF FFFF
 * 0x00000010 FFFF FFFF FFFF FFFF
 *
 * ELEM_OUTPUT_STR
 * key: qwertyuiop
 *
 * ELEM_OUTPUT_STR_NL
 * key:
 * qwertyuiop
 *
 * ELEM_OUTPUT_HEX
 * key: FFFFFFFFFFFFFFFF
 *
 * ELEM_OUTPUT_INT
 * key: 0xFF
 */
#define ELEM_OUTPUT_CHAR_LEN       1
#define ELEM_OUTPUT_SHORT_LEN      2
#define ELEM_OUTPUT_INT_LEN        4
#define ELEM_OUTPUT_LONG_LEN       8
#define ELEM_OUTPUT_HEX_MAX_LEN    32
#define ELEM_OUTPUT_DIVIDE_MAX_LEN 15
#define ELEMENT_NAME_MAX_LEN       32

enum ModelElementType {
    ELEM_OUTPUT_TYPE        = 0x0,
    ELEM_OUTPUT_BIN         = 0x1,      // name = func(offset, size);    整片二进制输出，按16进制显示
    ELEM_OUTPUT_STR         = 0x2,      // name = func(offset, maxSize); 换行字符串输出
    ELEM_OUTPUT_STR_NL      = 0x3,      // name = func(offset, maxSize); 不换行字符串输出
    ELEM_OUTPUT_HEX         = 0x4,      // name = func(offset, size);    每个字节按hex输出，最多输出16个字节
    ELEM_OUTPUT_INT         = 0x5,      // name = func(offset, size);    1,2,4,8字节整型输出
    ELEM_OUTPUT_CHAR        = 0x6,      // name = func(offset, size);    按长度，字符输出
    ELEM_OUTPUT_INT_CONST   = 0x7,      // name = value;                 整型:value(size)
    ELEM_OUTPUT_STR_CONST   = 0x8,      // name;                         字符串:value(0) size(0)
    ELEM_OUTPUT_NL          = 0x9,      // \n
    ELEM_OUTPUT_DIVIDE      = 0xa,      // ==========name==========
    ELEM_OUTPUT_MAX         = 0xFFF,

    ELEM_FEATURE_TYPE       = 0x1000,
    ELEM_FEATURE_TABLE      = 0x1001,
    ELEM_FEATURE_COMPARE    = 0x1002,
    ELEM_FEATURE_LOOPBUF    = 0x1003,
    ELEM_FEATURE_CHARLOG    = 0x1004,
    ELEM_FEATURE_STRUCTLOG  = 0x1005,
    ELEM_FEATURE_MAX        = 0x1FFF,

    ELEM_CTRL_TYPE          = 0x2000,
    ELEM_CTRL_TABLE         = 0x2000, // ELEM_FEATURE_TABLE 控制类
    ELEM_CTRL_TABLE_GOTO    = 0x2000, // (tableEnumType, 0);跳转表的PlaintextTableType enum类型值，非显示项
    ELEM_CTRL_TABLE_RANGE   = 0x2001, // (indexOffset, indexCnt);子表开始地址和长度，非显示项
    ELEM_CTRL_COMPARE       = 0x2100, // ELEM_FEATURE_COMPARE 控制类
    ELEM_CTRL_COM_VALUE     = 0x2100, // (offset, size);需要比较的值所在位置和长度，非显示项
    ELEM_CTRL_CMP_JUMP_NE   = 0x2101, // (compareValue, jumpIndex);如果不等于则跳转，非显示项
    ELEM_CTRL_CMP_JUMP_LE   = 0x2102, // (compareValue, jumpIndex);如果不大于则跳转，非显示项
    ELEM_CTRL_CMP_JUMP_LT   = 0x2103, // (compareValue, jumpIndex);如果小于则跳转，非显示项
    ELEM_CTRL_CMP_JUMP_GE   = 0x2104, // (compareValue, jumpIndex);如果不小于则跳转，非显示项
    ELEM_CTRL_CMP_JUMP_GT   = 0x2105, // (compareValue, jumpIndex);如果大于则跳转，非显示项
    ELEM_CTRL_CMP_JUMP_EQ   = 0x2106, // (compareValue, jumpIndex);如果等于则跳转，非显示项
    ELEM_CTRL_LOOPBUF       = 0x2200, // ELEM_FEATURE_LOOPBUF 控制类
    ELEM_CTRL_LPBF_HEAD     = 0x2200, // value;循环buffer头结构体长度，非显示项
    ELEM_CTRL_LPBF_READ     = 0x2201, // name: OutPutFunc(offset, size); 循环buffer读指针在结构体中偏移位置
    ELEM_CTRL_LPBF_WRITE    = 0x2203, // name: OutPutFunc(offset, size); 循环buffer写指针在结构体中偏移位置
    ELEM_CTRL_LPBF_SIZE     = 0x2202, // name: OutPutFunc(offset, size); 循环buffer总大小在结构体中偏移位置
    ELEM_CTRL_LPBF_SIZE_C   = 0x2202, // name: value; 循环buffer总长度，以固定值设置
    ELEM_CTRL_LPBF_ROLLBK   = 0x2203, // (offset, size); roll-back标记位，标记buffer是否翻转，非显示项
    ELEM_CTRL_MAX           = 0xFFFF,
};

enum ElemConditionType {
    ELEM_EQUAL   = 1 << 0, // 0x001
    ELEM_GRATER  = 1 << 1, // 0x010
    ELEM_LESS    = 1 << 2, // 0x100
};

struct ModelElement {
    char name[ELEMENT_NAME_MAX_LEN];
    unsigned int type;
    union {
        unsigned int arg1;
        unsigned int offset;
        unsigned int value;
        unsigned int index;
    };
    union {
        unsigned int arg2;
        unsigned int size;
        unsigned int maxSize;
        unsigned int mark;
        unsigned int indexOffset;
    };
};

#define MODEL_VECTOR(NAME)           struct ModelElement MODEL_VECTOR_OBJECT_##NAME[]
#define MODEL_VECTOR_OBJECT(NAME)    (&MODEL_VECTOR_OBJECT_##NAME[0])
#define MODEL_VECTOR_ITEM(NAME, i)   (&MODEL_VECTOR_OBJECT_##NAME[i])
#define MODEL_VECTOR_SIZE(NAME)      (sizeof(MODEL_VECTOR_OBJECT_##NAME) / sizeof(struct ModelElement))
#define DEFINE_DATA_MODEL(name) DATA_MODEL_##name

#define ELEMENT_CLASSIFY(type) ((type) & 0xFFFF)
#define OUTPUT_ELEMENT(type) (ELEMENT_CLASSIFY(type) > ELEM_OUTPUT_TYPE && ELEMENT_CLASSIFY(type) < ELEM_OUTPUT_MAX)
#define CTRL_ELEMENT(type) (ELEMENT_CLASSIFY(type) >= ELEM_CTRL_TYPE && ELEMENT_CLASSIFY(type) < ELEM_CTRL_MAX)
#define CMP_ELEMENT(type) (((type) & 0xFFF0) == ELEM_CTRL_COM_VALUE)
#define ELEM_CMP_CONDITION(type) ((type) & 0x000F)

#endif
