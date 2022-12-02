#pragma once

#include <obc/api.h>

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