#pragma once

#include "utils/errors_types.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Raw accelerometer data
 */
typedef struct AccelerometerRawData
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t rot_x;
    int16_t rot_y;
    int16_t rot_z;
    int16_t temperature;
} AccelerometerRawData;

/**
 * @brief Converted accelerometer data
 */
typedef struct AccelerometerData
{
    float accel_x;
    float accel_y;
    float accel_z;
    float rot_x;
    float rot_y;
    float rot_z;
    float temperature;
} AccelerometerData;

/**
 * @brief Accelerometer config data. All changes are done internally, so keep it just for reference!
 */
typedef struct AccelerometerConfig
{
    int _i2c;
    float _accelLBS;
    float _gryoLBS;
} AccelerometerConfig;

/**
 * @brief Initialize the accelerometer
 *
 * @param config Accelerometer config data
 * @param i2c I2C (0 or 1)
 * @param sda SDA pin
 * @param scl SCL pin
 * @param lpFilter Low Pass filter (0 - 6)
 * @param accelSensLevel Accelerometer sensitivity level (0 - 3)
 * @param gyroSensLevel Gyroscope sensitivity level (0 - 3)
 * @return Result code.
 */
FUNCRESULT accelerometerInit(AccelerometerConfig *config, int i2c, int sda, int scl, int lpFilter, int accelSensLevel, int gyroSensLevel);

/**
 * @brief Checks if the accelerometer is initialized
 *
 * @param config Accelerometer config data
 * @param available Is Initialized
 * @return Result code.
 */
FUNCRESULT accelerometerCheck(AccelerometerConfig *config, bool *result);

/**
 * @brief Read raw accelerometer data
 *
 * @param config Accelerometer config data
 * @param data Returns raw accelerometer data
 * @return Result code.
 */
FUNCRESULT accelerometerReadRaw(AccelerometerConfig *config, AccelerometerRawData *data);

/**
 * @brief Read converted accelerometer data
 *
 * @param config Accelerometer config data
 * @param inData Raw accelerometer data
 * @param outData Returns converted accelerometer data
 * @return Result code.
 */
FUNCRESULT accelerometerConvertData(AccelerometerConfig *config, AccelerometerRawData *inData, AccelerometerData *outData);