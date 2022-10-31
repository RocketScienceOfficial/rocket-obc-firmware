#pragma once

#include "drivers/barometer/bmp280_driver.h"
#include "drivers/accelerometer/mpu6050_driver.h"

typedef struct MeasurementData
{
    BMP280Data barometerData;
    MPU6050Data accelerometerData;
} MeasurementData;