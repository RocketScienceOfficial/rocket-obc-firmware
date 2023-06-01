#pragma once

/**
 * REF: https://github.com/bolderflight/bmi088-arduino
 * REF: https://github.com/boschsensortec/BMI08x-Sensor-API
 */

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "maths/vector.h"

/**
 * @brief BMI088 accelerometer configuration
 */
typedef struct BMI088AccelConfig
{
    SPIInstance spi;     /** SPI */
    PinNumber cs;        /** CS */
    FLOAT rangeConstant; /** Range Constant */
} BMI088AccelConfig;

/**
 * @brief BMI088 accelerometer range
 */
typedef enum BMI088AccelRange
{
    BMI088_ACCEL_RANGE_3G = 0x00,
    BMI088_ACCEL_RANGE_6G = 0x01,
    BMI088_ACCEL_RANGE_12G = 0x02,
    BMI088_ACCEL_RANGE_24G = 0x03,
} BMI088AccelRange;

/**
 * @brief BMI088 accelerometer ODR
 */
typedef enum BMI088AccelODR
{
    BMI088_ACCEL_ODR_12_5HZ = 0x05,
    BMI088_ACCEL_ODR_25HZ = 0x06,
    BMI088_ACCEL_ODR_50HZ = 0x07,
    BMI088_ACCEL_ODR_100HZ = 0x08,
    BMI088_ACCEL_ODR_200HZ = 0x09,
    BMI088_ACCEL_ODR_400HZ = 0x0A,
    BMI088_ACCEL_ODR_800HZ = 0x0B,
    BMI088_ACCEL_ODR_1600HZ = 0x0C,
} BMI088AccelODR;

/**
 * @brief BMI088 accelerometer OSR
 */
typedef enum BMI088AccelOSR
{
    BMI088_ACCEL_OSR_NORMAL = 0x0A,
    BMI088_ACCEL_OSR_2 = 0x09,
    BMI088_ACCEL_OSR_4 = 0x08,
} BMI088AccelOSR;

/**
 * @brief BMI088 gyrosopce configuration
 */
typedef struct BMI088GyroConfig
{
    SPIInstance spi;     /** SPI */
    PinNumber cs;        /** CS */
    FLOAT rangeConstant; /** Range constant */
} BMI088GyroConfig;

/**
 * @brief BMI088 gyroscope range
 */
typedef enum BMI088GyroRange
{
    BMI088_GYRO_RANGE_2000DPS = 0x00,
    BMI088_GYRO_RANGE_1000DPS = 0x01,
    BMI088_GYRO_RANGE_500DPS = 0x02,
    BMI088_GYRO_RANGE_250DPS = 0x03,
    BMI088_GYRO_RANGE_125DPS = 0x04,
} BMI088GyroRange;

/**
 * @brief BMI088 gyroscope ODR and Bandwidth
 */
typedef enum BMI088GyroBandwidth
{
    BMI088_GYRO_ODR_2000_BW_523HZ = 0x00,
    BMI088_GYRO_ODR_2000_BW_230HZ = 0x01,
    BMI088_GYRO_ODR_1000_BW_116HZ = 0x02,
    BMI088_GYRO_ODR_400_BW_47HZ = 0x03,
    BMI088_GYRO_ODR_200_BW_23HZ = 0x04,
    BMI088_GYRO_ODR_100_BW_12HZ = 0x05,
    BMI088_GYRO_ODR_200_BW_64HZ = 0x06,
    BMI088_GYRO_ODR_100_BW_32HZ = 0x07,
} BMI088GyroBandwidth;

/**
 * @brief Initialize BMI088 accelerometer
 *
 * @param config Configuration
 * @param spi SPI
 * @param miso MISO
 * @param mosi MOSI
 * @param cs CS
 * @param sck SCK
 * @return Result code
 */
FUNCRESULT bmi088AccelInit(BMI088AccelConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Set BMI088 accelerometer configuration
 *
 * @param config Configuration
 * @param odr ODR
 * @param osr OSR
 * @return Result code
 */
FUNCRESULT bmi088AccelSetConf(BMI088AccelConfig *config, BMI088AccelODR odr, BMI088AccelOSR osr);

/**
 * @brief Set BMI088 accelerometer range
 *
 * @param config Configuration
 * @param range Range
 * @return Result code
 */
FUNCRESULT bmi088AccelSetRange(BMI088AccelConfig *config, BMI088AccelRange range);

/**
 * @brief Read BMI088 accelerometer data
 *
 * @param config Configuration
 * @param accel Acceleration in meters per second squared
 * @return Result code
 */
FUNCRESULT bmi088AccelRead(BMI088AccelConfig *config, vec3 *accel);

/**
 * @brief Soft reset BMI088 accelerometer
 *
 * @param config Configuration
 */
VOID __bmi088AccelSoftReset(BMI088AccelConfig *config);

/**
 * @brief Set BMI088 accelerometer mode
 *
 * @param config Configuration
 * @param active Active
 */
VOID __bmi088AccelSetMode(BMI088AccelConfig *config, BOOL active);

/**
 * @brief Set BMI088 accelerometer power
 *
 * @param config Configuration
 * @param on On
 */
VOID __bmi088AccelSetPower(BMI088AccelConfig *config, BOOL on);

/**
 * @brief Read BMI088 accelerometer register
 *
 * @param config Configuration
 * @param address Address
 * @return Register value
 */
BYTE __bmi088AccelReadReg(BMI088AccelConfig *config, BYTE address);

/**
 * @brief Read BMI088 accelerometer registers
 *
 * @param config Configuration
 * @param address Address
 * @param buffer Buffer
 * @param count Count
 */
VOID __bmi088AccelReadRegs(BMI088AccelConfig *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Write BMI088 accelerometer register
 *
 * @param config Configuration
 * @param address Address
 * @param data Data
 */
VOID __bmi088AccelWriteReg(BMI088AccelConfig *config, BYTE address, BYTE data);

/**
 * @brief Initialize BMI088 gyroscope
 *
 * @param config Configuration
 * @param spi SPI
 * @param miso MISO
 * @param mosi MOSI
 * @param cs CS
 * @param sck SCK
 * @return Result code
 */
FUNCRESULT bmi088GyroInit(BMI088GyroConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Set BMI088 gyroscope bandwidth
 *
 * @param config Configuration
 * @param bw Bandwidth
 * @return Result code
 */
FUNCRESULT bmi088GyroSetBandwidth(BMI088GyroConfig *config, BMI088GyroBandwidth bw);

/**
 * @brief Set BMI088 gyroscope range
 *
 * @param config Configuration
 * @param range Range
 * @return Result code
 */
FUNCRESULT bmi088GyroSetRange(BMI088GyroConfig *config, BMI088GyroRange range);

/**
 * @brief Read BMI088 gyroscope data
 *
 * @param config Configuration
 * @param gyro Angular velocity in radians per second
 * @return Result code
 */
FUNCRESULT bmi088GyroRead(BMI088GyroConfig *config, vec3 *gyro);

/**
 * @brief Soft reset BMI088 gyroscope
 *
 * @param config Configuration
 */
VOID __bmi088GyroSoftReset(BMI088GyroConfig *config);

/**
 * @brief Read BMI088 gyroscope register
 *
 * @param config Configuration
 * @param address Address
 * @return Register value
 */
BYTE __bmi088GyroReadReg(BMI088GyroConfig *config, BYTE address);

/**
 * @brief Read BMI088 gyroscope registers
 *
 * @param config Configuration
 * @param address Address
 * @param buffer Buffer
 * @param count Count
 */
VOID __bmi088GyroReadRegs(BMI088GyroConfig *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Write BMI088 gyroscope register
 *
 * @param config Configuration
 * @param address Address
 * @param data Data
 */
VOID __bmi088GyroWriteReg(BMI088GyroConfig *config, BYTE address, BYTE data);