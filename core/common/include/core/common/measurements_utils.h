#pragma once

#include <obc/api.h>

/**
 * @brief Single measurement data structure
 */
typedef struct MeasurementData
{
    FLOAT pos_x;
    FLOAT pos_y;
    FLOAT pos_z;
    FLOAT roll;
    FLOAT pitch;
    FLOAT yaw;
    FLOAT latitude;
    FLOAT longtitude;
    FLOAT altitude;
    FLOAT velocity;
} MeasurementData;

/**
 * @brief Initializes the measurements logger.
 */
VOID initializeMeasurementLogger();

/**
 * @brief Logs the measurement data.
 *
 * @param data The data to log.
 */
VOID logMeasurementData(MeasurementData *data);