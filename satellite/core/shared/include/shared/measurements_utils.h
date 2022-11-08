#pragma once

#include "drivers/barometer/bmp280_driver.h"
#include "drivers/accelerometer/mpu6050_driver.h"

/**
 * @brief Single measurement data structure.
 */
typedef struct MeasurementData
{
    BMP280Data barometerData;      // Barometer data.
    MPU6050Data accelerometerData; // Accelerometer data.
} MeasurementData;