#pragma once

#include <obc/api.h>

/**
 * @brief Single measurement data structure.
 */
typedef struct MeasurementData
{
    FLOAT accel_x;
    FLOAT accel_y;
    FLOAT accel_z;
    FLOAT vel_x;
    FLOAT vel_y;
    FLOAT vel_z;
    FLOAT pos_x;
    FLOAT pos_y;
    FLOAT pos_z;
    FLOAT roll;
    FLOAT pitch;
    FLOAT yaw;
    FLOAT latitude;
    FLOAT longtitude;
    FLOAT altitude;
    FLOAT temperature;
    FLOAT pressure;
    FLOAT humidity;
    INT32 radioSignalStrength;
    FLOAT cpuTemp;
    FLOAT batteryPercentage;
    FLOAT fuelPercentage;
} MeasurementData;