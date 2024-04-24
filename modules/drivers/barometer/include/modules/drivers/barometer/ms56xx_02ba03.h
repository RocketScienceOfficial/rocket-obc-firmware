#ifndef _MS56XX_02BA03_H
#define _MS56XX_02BA03_H

#include "modules/drivers/hal/spi_driver.h"
#include <stdint.h>

/**
 * @brief MS56XX-02BA03 OSR
 */
typedef enum ms56xx_02ba03_osr
{
    MS56XX_02BA03_OSR_256 = 0x40,
    MS56XX_02BA03_OSR_512 = 0x42,
    MS56XX_02BA03_OSR_1024 = 0x44,
    MS56XX_02BA03_OSR_2048 = 0x46,
    MS56XX_02BA03_OSR_4096 = 0x48,
} ms56xx_02ba03_osr_t;

/**
 * @brief MS56XX-02BA03 Prom Data (Coefficents)
 */
typedef struct ms56xx_02ba03_prom_data
{
    uint16_t s;
    uint16_t c1;
    uint16_t c2;
    uint16_t c3;
    uint16_t c4;
    uint16_t c5;
    uint16_t c6;
    uint16_t crc;
} ms56xx_02ba03_prom_data_t;

/**
 * @brief MS56XX-02BA03 Prom Data (Coefficents)
 */
typedef struct ms56xx_02ba03_config
{
    hal_spi_instance_t spi;
    hal_pin_number_t cs;
    ms56xx_02ba03_prom_data_t coeffs;
    ms56xx_02ba03_osr_t pressOSR;
    ms56xx_02ba03_osr_t tempOSR;
} ms56xx_02ba03_config_t;

/**
 * @brief Initalize MS56XX-02BA03 for SPI
 *
 * @param config Configuration
 * @param spi SPI
 * @param cs CS
 */
void ms56xx_02ba03_init_spi(ms56xx_02ba03_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Sets OSR
 *
 * @param config Configuration
 * @param press Pressure OSR
 * @param temp Temperature OSR
 */
void ms56xx_02ba03_set_osr(ms56xx_02ba03_config_t *config, ms56xx_02ba03_osr_t press, ms56xx_02ba03_osr_t temp);

/**
 * @brief Reads values from MS56XX-02BA03
 *
 * @param config Configuration
 * @param pressure Pointer to pressure
 * @param temperature Pointer to temperature
 */
void ms56xx_02ba03_read(const ms56xx_02ba03_config_t *config, int *pressure, float *temperature);

#endif