#ifndef _MS5607_DRIVER_H
#define _MS5607_DRIVER_H

#include "hal/spi_driver.h"
#include "hal/time_tracker.h"
#include <stdint.h>

/**
 * @brief MS5607 OSR
 */
typedef enum ms5607_osr
{
    MS5607_OSR_256 = 0x40,
    MS5607_OSR_512 = 0x42,
    MS5607_OSR_1024 = 0x44,
    MS5607_OSR_2048 = 0x46,
    MS5607_OSR_4096 = 0x48,
} ms5607_osr_t;

/**
 * @brief MS5607 Prom Data (Coefficents)
 */
typedef struct ms5607_prom_data
{
    uint16_t s;
    uint16_t c1;
    uint16_t c2;
    uint16_t c3;
    uint16_t c4;
    uint16_t c5;
    uint16_t c6;
    uint16_t crc;
} ms5607_prom_data_t;

/**
 * @brief MS5607 Prom Data (Coefficents)
 */
typedef struct ms5607_config
{
    hal_spi_instance_t spi;
    hal_pin_number_t cs;
    ms5607_prom_data_t coeffs;
    ms5607_osr_t pressOSR;
    ms5607_osr_t tempOSR;
    uint32_t d1;
    usec_t nextTime;
} ms5607_config_t;

/**
 * @brief Initalize MS5607 for SPI
 *
 * @param config Configuration
 * @param spi SPI
 * @param cs CS
 */
void ms5607_init_spi(ms5607_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Sets OSR
 *
 * @param config Configuration
 * @param press Pressure OSR
 * @param temp Temperature OSR
 */
void ms5607_set_osr(ms5607_config_t *config, ms5607_osr_t press, ms5607_osr_t temp);

/**
 * @brief Reads values from MS5607
 *
 * @param config Configuration
 * @param pressure Pointer to pressure
 * @param temperature Pointer to temperature
 * @return True if values were overwritten
 */
bool ms5607_read_non_blocking(ms5607_config_t *config, int *pressure, float *temperature);

#endif