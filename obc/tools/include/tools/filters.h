#pragma once

#include "typedefs.h"

/**
 * @brief Average filter structure.
 */
typedef struct AvgFilterData
{
    FLOAT v1;    /** First value. */
    FLOAT v2;    /** Second value. */
    BOOL v1Init; /** Is first value initialized. */
    BOOL v2Init; /** Is second value initialized. */
} AvgFilterData;

/**
 * @brief Exponential filter structure.
 */
typedef struct ExpFilterData
{
    FLOAT v1;    /** First value. */
    FLOAT b;     /** Filter coefficient. */
    BOOL v1Init; /** Is first value initialized. */
    BOOL bInit;  /** Is filter coefficient initialized. */
} ExpFilterData;

/**
 * @brief Average filter.
 *
 * @param data Filter data.
 * @param v Value to filter and the output value.
 */
VOID filterAvg(AvgFilterData *data, FLOAT *v);

/**
 * @brief Average weightend filter.
 *
 * @param data Filter data.
 * @param v Value to filter and the output value.
 */
VOID filterAvgWeight(AvgFilterData *data, FLOAT *v);

/**
 * @brief Exponential filter.
 *
 * @param data Filter data.
 * @param v Value to filter and the output value.
 * @param alpha Alpha value.
 * @param beta Beta value.
 */
VOID filterExp(ExpFilterData *data, FLOAT *v, FLOAT alpha, FLOAT beta);