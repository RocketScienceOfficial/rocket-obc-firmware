#ifndef _LSM6DSO32_DRIVER_H
#define _LSM6DSO32_DRIVER_H

#include "modules/drivers/hal/gpio_utils.h"
#include "modules/maths/vector.h"
#include <stdbool.h>

/**
 * @brief LSM6DSO32 Accelerometer configuration
 */
typedef struct lsm6dso32_config
{
    gpio_communication_config_t gpioConfig; /** GPIO communication config */
    float accelRangeConstant;               /** Accelerometer range constant */
    float gyroRangeConstant;                /** Gyroscope range constant */
} lsm6dso32_config_t;

/**
 * @brief LSM6DSO32 Accelerometer and Gyroscope ODR
 */
typedef enum lsm6dso32_odr
{
    LSM6DSO32_ODR_12_5HZ = 0x01,
    LSM6DSO32_ODR_26HZ = 0x02,
    LSM6DSO32_ODR_52HZ = 0x03,
    LSM6DSO32_ODR_104HZ = 0x04,
    LSM6DSO32_ODR_208HZ = 0x05,
    LSM6DSO32_ODR_416HZ = 0x06,
    LSM6DSO32_ODR_833HZ = 0x07,
    LSM6DSO32_ODR_1660HZ = 0x08,
    LSM6DSO32_ODR_3330HZ = 0x09,
    LSM6DSO32_ODR_6660HZ = 0x0A,
} lsm6dso32_odr_t;

/**
 * @brief LSM6DSO32 Accelerometer range
 */
typedef enum lsm6dso32_accel_range
{
    LSM6DSO32_RANGE_4G = 0x00,
    LSM6DSO32_RANGE_8G = 0x02,
    LSM6DSO32_RANGE_16G = 0x03,
    LSM6DSO32_RANGE_32G = 0x01,
} lsm6dso32_accel_range_t;

/**
 * @brief LSM6DSO32 Gyroscope range
 */
typedef enum lsm6dso32_gyro_range
{
    LSM6DSO32_RANGE_125DPS = 0x04,
    LSM6DSO32_RANGE_250DPS = 0x00,
    LSM6DSO32_RANGE_500DPS = 0x01,
    LSM6DSO32_RANGE_1000DPS = 0x02,
    LSM6DSO32_RANGE_2000DPS = 0x03,
} lsm6dso32_gyro_range_t;

/**
 * @brief Initialize LSM6DSO accelerometer using SPI
 *
 * @param config LSM6DSO configuration
 * @param spi SPI instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param cs CS pin
 * @param sck SCK pin
 */
void lsm6dso32_init_spi(lsm6dso32_config_t *config, spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t cs, pin_number_t sck);

/**
 * @brief Initialize LSM6DSO accelerometer using I2C
 *
 * @param config LSM6DSO configuration
 * @param i2c I2C instance
 * @param sda SDA pin
 * @param scl SCL pin
 */
void lsm6dso32_init_i2c(lsm6dso32_config_t *config, i2c_instance_t i2c, pin_number_t sda, pin_number_t scl);

/**
 * @brief Validate LSM6DSO32 ID
 *
 * @param config LSM6DSO configuration
 * @param valid Validity of ID
 */
void lsm6dso32_validate_id(lsm6dso32_config_t *config, bool *valid);

/**
 * @brief Set LSM6DSO32 ODR
 *
 * @param config LSM6DSO configuration
 * @param accelODR Accelerometer ODR
 * @param gyroODR Gyroscope ODR
 */
void lsm6dso32_set_odr(lsm6dso32_config_t *config, lsm6dso32_odr_t accelODR, lsm6dso32_odr_t gyroODR);

/**
 * @brief Set LSM6DSO32 range
 *
 * @param config LSM6DSO configuration
 * @param accelRange Accelerometer range
 * @param gyroRange Gyroscope range
 */
void lsm6dso32_set_range(lsm6dso32_config_t *config, lsm6dso32_accel_range_t accelRange, lsm6dso32_gyro_range_t gyroRange);

/**
 * @brief Read LSM6DSO acceleration and angular rate
 *
 * @param config LSM6DSO configuration
 * @param pAcceleration Acceleration in meters per second squared
 * @param pGyro Angular rate in radians per second
 * @param pTemperature Temperature in degrees Celsius
 */
void lsm6dso32_read(lsm6dso32_config_t *config, vec3_t *pAcceleration, vec3_t *pGyro, float *pTemperature);

#endif