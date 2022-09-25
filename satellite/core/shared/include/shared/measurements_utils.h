#pragma once

#include "drivers/barometer/bmp280_driver.h"
#include "drivers/accelerometer/mpu6050_driver.h"
#include "tools/typedefs.h"

typedef enum ComponentsStates
{
    COMPONENT_BAROMETER = 1 << 0,
    COMPONENT_ACCELEROMETER = 1 << 1,
} ComponentsStates;

typedef struct MeasurementData
{
    BMP280Data barometerData;
    MPU6050Data accelerometerData;
    INT32 componentsStatus;
} MeasurementData;