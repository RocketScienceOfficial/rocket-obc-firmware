#ifndef _BMI088_DRIVER_H
#define _BMI088_DRIVER_H

/**
 * REF: https://github.com/bolderflight/bmi088-arduino
 * REF: https://github.com/boschsensortec/BMI08x-Sensor-API
 */

#include "lib/drivers/utils/gpio_utils.h"
#include "lib/maths/vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief BMI088 accelerometer configuration
 */
typedef struct bmi088_accel_config
{
    gpio_utils_communication_config_t gpioConfig; /** GPIO communication configuration */
    float rangeFactor;                            /** Range factor */
} bmi088_accel_config_t;

/**
 * @brief BMI088 accelerometer range
 */
typedef enum bmi088_accel_range
{
    BMI088_ACCEL_RANGE_3G = 0x00,
    BMI088_ACCEL_RANGE_6G = 0x01,
    BMI088_ACCEL_RANGE_12G = 0x02,
    BMI088_ACCEL_RANGE_24G = 0x03,
} bmi088_accel_range_t;

/**
 * @brief BMI088 accelerometer ODR
 */
typedef enum bmi088_accel_odr
{
    BMI088_ACCEL_ODR_12_5HZ = 0x05,
    BMI088_ACCEL_ODR_25HZ = 0x06,
    BMI088_ACCEL_ODR_50HZ = 0x07,
    BMI088_ACCEL_ODR_100HZ = 0x08,
    BMI088_ACCEL_ODR_200HZ = 0x09,
    BMI088_ACCEL_ODR_400HZ = 0x0A,
    BMI088_ACCEL_ODR_800HZ = 0x0B,
    BMI088_ACCEL_ODR_1600HZ = 0x0C,
} bmi088_accel_odr_t;

/**
 * @brief BMI088 accelerometer OSR
 */
typedef enum bmi088_accel_osr
{
    BMI088_ACCEL_OSR_NORMAL = 0x0A,
    BMI088_ACCEL_OSR_2 = 0x09,
    BMI088_ACCEL_OSR_4 = 0x08,
} bmi088_accel_osr_t;

/**
 * @brief BMI088 gyrosopce configuration
 */
typedef struct bmi088_gyro_config
{
    gpio_utils_communication_config_t gpioConfig; /** GPIO communication configuration */
    float rangeFactor;                            /** Range factor */
} bmi088_gyro_config_t;

/**
 * @brief BMI088 gyroscope range
 */
typedef enum bmi088_gyro_range
{
    BMI088_GYRO_RANGE_2000DPS = 0x00,
    BMI088_GYRO_RANGE_1000DPS = 0x01,
    BMI088_GYRO_RANGE_500DPS = 0x02,
    BMI088_GYRO_RANGE_250DPS = 0x03,
    BMI088_GYRO_RANGE_125DPS = 0x04,
} bmi088_gyro_range_t;

/**
 * @brief BMI088 gyroscope ODR and Bandwidth
 */
typedef enum bmi088_gyro_bandwidth
{
    BMI088_GYRO_ODR_2000_BW_523HZ = 0x00,
    BMI088_GYRO_ODR_2000_BW_230HZ = 0x01,
    BMI088_GYRO_ODR_1000_BW_116HZ = 0x02,
    BMI088_GYRO_ODR_400_BW_47HZ = 0x03,
    BMI088_GYRO_ODR_200_BW_23HZ = 0x04,
    BMI088_GYRO_ODR_100_BW_12HZ = 0x05,
    BMI088_GYRO_ODR_200_BW_64HZ = 0x06,
    BMI088_GYRO_ODR_100_BW_32HZ = 0x07,
} bmi088_gyro_bandwidth_t;

/**
 * @brief Initialize BMI088 accelerometer
 *
 * @param config Configuration
 * @param spi SPI
 * @param cs CS
 */
void bmi088_accel_init_spi(bmi088_accel_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Initialize BMI088 accelerometer
 *
 * @param config Configuration
 * @param i2c I2C
 * @param sdo1Grounded Is SDO1 pulled to GND
 */
void bmi088_accel_init_i2c(bmi088_accel_config_t *config, hal_i2c_instance_t i2c, bool sdo1Grounded);

/**
 * @brief Set BMI088 accelerometer configuration
 *
 * @param config Configuration
 * @param odr ODR
 * @param osr OSR
 */
void bmi088_accel_set_conf(const bmi088_accel_config_t *config, bmi088_accel_odr_t odr, bmi088_accel_osr_t osr);

/**
 * @brief Set BMI088 accelerometer range
 *
 * @param config Configuration
 * @param range Range
 */
void bmi088_accel_set_range(bmi088_accel_config_t *config, bmi088_accel_range_t range);

/**
 * @brief Read BMI088 accelerometer data
 *
 * @param config Configuration
 * @param accel Acceleration in meters per second squared
 */
void bmi088_accel_read(const bmi088_accel_config_t *config, vec3_t *accel);

/**
 * @brief Initialize BMI088 gyroscope
 *
 * @param config Configuration
 * @param spi SPI
 * @param cs CS
 */
void bmi088_gyro_init_spi(bmi088_gyro_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Initialize BMI088 gyroscope
 *
 * @param config Configuration
 * @param i2c I2C
 * @param sdo1Grounded Is SDO1 pulled to GND
 */
void bmi088_gyro_init_i2c(bmi088_gyro_config_t *config, hal_i2c_instance_t i2c, bool sdo1Grounded);

/**
 * @brief Set BMI088 gyroscope bandwidth
 *
 * @param config Configuration
 * @param bw Bandwidth
 */
void bmi088_gyro_set_bandwidth(const bmi088_gyro_config_t *config, bmi088_gyro_bandwidth_t bw);

/**
 * @brief Set BMI088 gyroscope range
 *
 * @param config Configuration
 * @param range Range
 */
void bmi088_gyro_set_range(bmi088_gyro_config_t *config, bmi088_gyro_range_t range);

/**
 * @brief Read BMI088 gyroscope data
 *
 * @param config Configuration
 * @param gyro Angular velocity in radians per second
 */
void bmi088_gyro_read(const bmi088_gyro_config_t *config, vec3_t *gyro);

#endif