#ifndef _BMP581_DRIVER_H
#define _BMP581_DRIVER_H

#include "modules/drivers/utils/gpio_utils.h"
#include <stdbool.h>

/**
 * @brief BMP581 configuration
 */
typedef struct bmp581_config
{
    hal_gpio_communication_config_t gpioConfig; /** GPIO communication configuration */
} bmp581_config_t;

/**
 * @brief BMP581 data
 */
typedef struct bmp581_data
{
    float pressure;    /** Pressure in Pa */
    float temperature; /** Temperature in C */
} bmp581_data_t;

/**
 * @brief BMP581 OSR
 */
typedef enum bmp581_osr
{
    BMP581_OSR_1 = 0x00,
    BMP581_OSR_2 = 0x01,
    BMP581_OSR_4 = 0x02,
    BMP581_OSR_8 = 0x03,
    BMP581_OSR_16 = 0x04,
    BMP581_OSR_32 = 0x05,
    BMP581_OSR_64 = 0x06,
    BMP581_OSR_128 = 0x07,
} bmp581_osr_t;

/**
 * @brief BMP581 ODR
 */
typedef enum bmp581_odr
{
    BMP581_ODR_240 = 0x00,
    BMP581_ODR_220 = 0x01,
    BMP581_ODR_200 = 0x02,
    BMP581_ODR_180 = 0x03,
    BMP581_ODR_160 = 0x04,
    BMP581_ODR_150 = 0x05,
    BMP581_ODR_140 = 0x06,
    BMP581_ODR_130 = 0x07,
    BMP581_ODR_120 = 0x08,
    BMP581_ODR_110 = 0x09,
    BMP581_ODR_100 = 0x0A,
    BMP581_ODR_90 = 0x0B,
    BMP581_ODR_80 = 0x0C,
    BMP581_ODR_70 = 0x0D,
    BMP581_ODR_60 = 0x0E,
    BMP581_ODR_50 = 0x0F,
    BMP581_ODR_45 = 0x10,
    BMP581_ODR_40 = 0x11,
    BMP581_ODR_35 = 0x12,
    BMP581_ODR_30 = 0x13,
    BMP581_ODR_25 = 0x14,
    BMP581_ODR_20 = 0x15,
    BMP581_ODR_15 = 0x16,
    BMP581_ODR_10 = 0x17,
    BMP581_ODR_5 = 0x18,
    BMP581_ODR_4 = 0x19,
    BMP581_ODR_3 = 0x1A,
    BMP581_ODR_2 = 0x1B,
    BMP581_ODR_1 = 0x1C,
    BMP581_ODR_0500 = 0x1D,
    BMP581_ODR_0250 = 0x1E,
    BMP581_ODR_0125 = 0x1F,
} bmp581_odr_t;

/**
 * @brief BMP581 mode
 */
typedef enum bmp581_mode
{
    BMP581_MODE_SLEEP = 0x00,
    BMP581_MODE_NORMAL = 0x01,
    BMP581_MODE_FORCED = 0x02,
    BMP581_MODE_CONTINUOUS = 0x03,
} bmp581_mode_t;

/**
 * @brief Initializes BMP581 using SPI
 *
 * @param config BMP581 configuration
 * @param spi SPI instance
 * @param cs CS pin
 */
void bmp581_init_spi(bmp581_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Initializes BMP581 using I2C
 *
 * @param config BMP581 configuration
 * @param i2c I2C instance
 */
void bmp581_init_i2c(bmp581_config_t *config, hal_i2c_instance_t i2c);

/**
 * @brief Validates BMP581 ID
 *
 * @param config BMP581 configuration
 * @return Validation result
 */
bool bmp581_validate_id(bmp581_config_t *config);

/**
 * @brief Sets BMP581 OSR of temperature and pressure
 *
 * @param config BMP581 configuration
 * @param tempOSR OSR of temperature
 * @param pressOSR OSR of pressure
 * @param odr ODR of BMP581. Refer to datasheet for allowed values!
 */
void bmp581_set_odr_osr(bmp581_config_t *config, bmp581_osr_t tempOSR, bmp581_osr_t pressOSR, bmp581_odr_t odr);

/**
 * @brief Sets BMP581 mode
 *
 * @param config BMP581 configuration
 * @param mode BMP581 mode
 */
void bmp581_set_mode(bmp581_config_t *config, bmp581_mode_t mode);

/**
 * @brief Read BMP581 sensor data
 *
 * @param config BMP581 configuration
 * @param data BMP581 data pointer
 */
void bmp581_read(bmp581_config_t *config, bmp581_data_t *data);

#endif