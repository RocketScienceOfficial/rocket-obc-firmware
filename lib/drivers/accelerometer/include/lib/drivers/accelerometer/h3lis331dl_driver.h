#ifndef _H3LIS331DL_DRIVER_H
#define _H3LIS331DL_DRIVER_H

#include "lib/drivers/utils/gpio_utils.h"
#include "lib/maths/vector.h"
#include <stdbool.h>

/**
 * @brief H3lis331dl configuration
 */
typedef struct h3lis331dl_config
{
    gpio_utils_communication_config_t gpioConfig; /** GPIO communication configuration */
    float rangeFactor;                            /** Range factor */
} h3lis331dl_config_t;

/**
 * @brief H3lis331dl power mode
 */
typedef enum h3lis331dl_power_mode
{
    H3LIS331DL_POWER_DOWN = 0x00,
    H3LIS331DL_POWER_NORMAL = 0x01,
    H3LIS331DL_LOW_POWER_1 = 0x02,
    H3LIS331DL_LOW_POWER_2 = 0x03,
    H3LIS331DL_LOW_POWER_3 = 0x04,
    H3LIS331DL_LOW_POWER_4 = 0x05,
    H3LIS331DL_LOW_POWER_5 = 0x06,
} h3lis331dl_power_mode_t;

/**
 * @brief H3lis331dl output data rate
 */
typedef enum h3lis331dl_odr
{
    H3LIS331DL_ODR_50HZ = 0x00,
    H3LIS331DL_ODR_100HZ = 0x01,
    H3LIS331DL_ODR_400HZ = 0x02,
    H3LIS331DL_ODR_1000HZ = 0x03,
} h3lis331dl_odr_t;

/**
 * @brief H3lis331dl high pass filter cut off frequency coefficient
 */
typedef enum h3lis331dl_hpfc
{
    H3LIS331DL_HPF_8 = 0x00,
    H3LIS331DL_HPF_16 = 0x01,
    H3LIS331DL_HPF_32 = 0x02,
    H3LIS331DL_HPF_64 = 0x03,
} h3lis331dl_hpfc_t;

/**
 * @brief H3lis331dl range
 */
typedef enum h3lis331dl_range
{
    H3LIS331DL_RANGE_100G = 0x00,
    H3LIS331DL_RANGE_200G = 0x01,
    H3LIS331DL_RANGE_400G = 0x03,
} h3lis331dl_range_t;

/**
 * @brief Initializes H3lis331dl with SPI
 *
 * @param config H3lis331dl configuration
 * @param spi SPI
 * @param cs CS
 */
void h3lis331dl_init_spi(h3lis331dl_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Initializes H3lis331dl with I2C
 *
 * @param config H3lis331dl configuration
 * @param i2c I2C
 */
void h3lis331dl_init_i2c(h3lis331dl_config_t *config, hal_i2c_instance_t i2c);

/**
 * @brief Checks if H3lis331dl WHO_AM_I register is valid
 *
 * @param config H3lis331dl configuration
 * @param valid True if valid
 */
void h3lis331dl_validate_id(h3lis331dl_config_t *config, bool *valid);

/**
 * @brief Sets power
 *
 * @param config H3lis331dl configuration
 * @param power Power
 */
void h3lis331dl_set_power_mode(h3lis331dl_config_t *config, h3lis331dl_power_mode_t power);

/**
 * @brief Sets output data rate
 *
 * @param config H3lis331dl configuration
 * @param odr Output data rate
 */
void h3lis331dl_set_odr(h3lis331dl_config_t *config, h3lis331dl_odr_t odr);

/**
 * @brief Sets high pass filter cut off frequency coefficient
 *
 * @param config H3lis331dl configuration
 * @param hpcf High pass filter cut off frequency coefficient
 */
void h3lis331dl_set_hpfc(h3lis331dl_config_t *config, h3lis331dl_hpfc_t hpcf);

/**
 * @brief Sets range
 *
 * @param config H3lis331dl configuration
 * @param range Range
 */
void h3lis331dl_set_range(h3lis331dl_config_t *config, h3lis331dl_range_t range);

/**
 * @brief Reads H3lis331dl
 *
 * @param config H3lis331dl configuration
 * @param accel H3lis331dl acceleration
 */
void h3lis331dl_read(h3lis331dl_config_t *config, vec3_t *accel);

#endif