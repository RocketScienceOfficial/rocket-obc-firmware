#pragma once

#include "typedefs.h"

typedef struct AvgFilterData
{
    FLOAT v1;
    FLOAT v2;
    BOOL v1Init;
    BOOL v2Init;
} AvgFilterData;

typedef struct ExpFilterData
{
    FLOAT v1;
    FLOAT b;
    BOOL v1Init;
    BOOL bInit;
} ExpFilterData;

VOID filterAvg(AvgFilterData *data, FLOAT *v);
VOID filterExp(ExpFilterData *data, FLOAT *v, FLOAT alpha, FLOAT beta);