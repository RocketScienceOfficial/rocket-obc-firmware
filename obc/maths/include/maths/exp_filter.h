#pragma once

#include <obc/api.h>

/**
 * @brief Exponential filter structure
 */
typedef struct ExpFilterData
{
    FLOAT v1;    /** First value */
    FLOAT b;     /** Filter coefficient */
    BOOL v1Init; /** Is first value initialized */
    BOOL bInit;  /** Is filter coefficient initialized */
} ExpFilterData;

/**
 * @brief Exponential filter
 *
 * @param data Filter data
 * @param v Value to filter and the output value
 * @param alpha Alpha value
 * @param beta Beta value
 */
VOID filterExp(ExpFilterData *data, FLOAT *v, FLOAT alpha, FLOAT beta);