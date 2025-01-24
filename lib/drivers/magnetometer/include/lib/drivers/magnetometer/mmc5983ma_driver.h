#ifndef _MMC5983MA_DRIVER_H
#define _MMC5983MA_DRIVER_H

/**
 * REF: https://github.com/kriswiner/MMC5983MA/tree/master
 * REF: https://www.vectornav.com/resources/inertial-navigation-primer/specifications--and--error-budgets/specs-hsicalibration
 */

#include "lib/drivers/utils/gpio_utils.h"
#include "lib/maths/vector.h"
#include <stdbool.h>

/**
 * @brief MMC5983MA Magnetometer configuration
 */
typedef struct mmc5983ma_config
{
    gpio_utils_communication_config_t gpioConfig; /** The GPIO communication configuration */
} mmc5983ma_config_t;

/**
 * @brief MMC5983MA Magnetometer ODR
 */
typedef enum mmc5983_odr
{
    MMC5983MA_ODR_1HZ = 0x01,
    MMC5983MA_ODR_10HZ = 0x02,
    MMC5983MA_ODR_20HZ = 0x03,
    MMC5983MA_ODR_50HZ = 0x04,
    MMC5983MA_ODR_100HZ = 0x05,
    MMC5983MA_ODR_200HZ = 0x06,
    MMC5983MA_ODR_1000HZ = 0x07,
} mmc5983_odr_t;

/**
 * @brief MMC5983MA Magnetometer initialization using SPI
 *
 * @param config MMC5983MA configuration
 * @param spi SPI instance
 * @param cs CS pin
 */
void mmc5983ma_init_spi(mmc5983ma_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief MMC5983MA Magnetometer initialization using I2C
 *
 * @param config MMC5983MA configuration
 * @param i2c I2C instance
 */
void mmc5983ma_init_i2c(mmc5983ma_config_t *config, hal_i2c_instance_t i2c);

/**
 * @brief Check if product id is valid
 *
 * @param config MMC5983MA configuration
 * @return Validity
 */
bool mmc5983ma_validate_id(const mmc5983ma_config_t *config);

/**
 * @brief Set ODR of magnetometer
 *
 * @param config MMC5983MA configuration
 * @param odr ODR
 */
void mmc5983ma_set_continuous_mode_odr(const mmc5983ma_config_t *config, mmc5983_odr_t odr);

/**
 * @brief Read data from magnetometer
 *
 * @param config MMC5983MA configuration
 * @param mag Magnetic field in miliGauss
 */
void mmc5983ma_read(const mmc5983ma_config_t *config, vec3_t *mag);

/**
 * @brief Read temperature from magnetometer
 *
 * @param config MMC5983MA configuration
 * @param temp Temperature
 */
void mmc5983ma_read_temp(const mmc5983ma_config_t *config, float *temp);

#endif