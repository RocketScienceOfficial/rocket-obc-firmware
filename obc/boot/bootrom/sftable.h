/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SFTABLE_H
#define _SFTABLE_H

#define SF_TABLE_FADD        0x00
#define SF_TABLE_FSUB        0x04
#define SF_TABLE_FMUL        0x08
#define SF_TABLE_FDIV        0x0c
#define SF_TABLE_FCMP        0x10
#define SF_TABLE_FCMP_FLAGS  0x14
#define SF_TABLE_FSQRT       0x18
#define SF_TABLE_FLOAT2INT   0x1c
#define SF_TABLE_FLOAT2FIX   0x20
#define SF_TABLE_FLOAT2UINT  0x24
#define SF_TABLE_FLOAT2UFIX  0x28
#define SF_TABLE_INT2FLOAT   0x2c
#define SF_TABLE_FIX2FLOAT   0x30
#define SF_TABLE_UINT2FLOAT  0x34
#define SF_TABLE_UFIX2FLOAT  0x38
#define SF_TABLE_FCOS        0x3c
#define SF_TABLE_FSIN        0x40
#define SF_TABLE_FTAN        0x44
#define SF_TABLE_FATAN2      0x48
#define SF_TABLE_FEXP        0x4c
#define SF_TABLE_FLN         0x50

#define SF_TABLE_SIZE        0x54

#endif
