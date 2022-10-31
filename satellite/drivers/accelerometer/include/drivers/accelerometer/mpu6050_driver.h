#pragma once

#include "tools/typedefs.h"
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/i2c_driver.h"

/**
 * @brief Raw accelerometer data
 */
typedef struct MPU6050RawData
{
    INT16 accel_x;
    INT16 accel_y;
    INT16 accel_z;
    INT16 rot_x;
    INT16 rot_y;
    INT16 rot_z;
    INT16 temperature;
} MPU6050RawData;

/**
 * @brief Converted accelerometer data
 */
typedef struct MPU6050Data
{
    FLOAT accel_x;
    FLOAT accel_y;
    FLOAT accel_z;
    FLOAT rot_x;
    FLOAT rot_y;
    FLOAT rot_z;
    FLOAT temperature;
} MPU6050Data;

/**
 * @brief Accelerometer config data. All changes are done internally, so keep it just for reference!
 */
typedef struct MPU6050Config
{
    I2CInstance _i2c;
    FLOAT _accelLBS;
    FLOAT _gryoLBS;
} MPU6050Config;

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
FUNCRESULT mpu6050Init(MPU6050Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl, UINT8 lpFilter, UINT8 accelSensLevel, UINT8 gyroSensLevel);

/**
 * @brief Checks if the accelerometer is initialized
 *
 * @param config Accelerometer config data
 * @param available Is Initialized
 * @return Result code.
 */
FUNCRESULT mpu6050Check(MPU6050Config *config, BOOL *result);

/**
 * @brief Read raw accelerometer data
 *
 * @param config Accelerometer config data
 * @param data Returns raw accelerometer data
 * @return Result code.
 */
FUNCRESULT mpu6050ReadRaw(MPU6050Config *config, MPU6050RawData *data);

/**
 * @brief Read converted accelerometer data
 *
 * @param config Accelerometer config data
 * @param inData Raw accelerometer data
 * @param outData Returns converted accelerometer data
 * @return Result code.
 */
FUNCRESULT mpu6050ConvertData(MPU6050Config *config, MPU6050RawData *inData, MPU6050Data *outData);