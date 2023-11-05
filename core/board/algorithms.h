#pragma once

#include <obc/api.h>
#include "measurements.h"

/**
 * @brief Initializes all algorithms.
 */
VOID initAlgorithms(VOID);

/**
 * @brief Converts raw data to measurement data.
 *
 * @param raw Raw data.
 * @param data Measurement data.
 */
VOID convertRawData(RawMeasurementData *raw, MeasurementData *data);