#ifndef _MS5611_DRIVER_H
#define _MS5611_DRIVER_H

#include "hal/spi_driver.h"
#include "hal/time_tracker.h"
#include <stdint.h>

/**
 * @brief MS5611 OSR
 */
typedef enum ms5611_osr
{
    MS5611_OSR_256 = 0x40,
    MS5611_OSR_512 = 0x42,
    MS5611_OSR_1024 = 0x44,
    MS5611_OSR_2048 = 0x46,
    MS5611_OSR_4096 = 0x48,
} ms5611_osr_t;

/**
 * @brief MS5611 Prom Data (Coefficents)
 */
typedef struct ms5611_prom_data
{
    uint16_t s;
    uint16_t c1;
    uint16_t c2;
    uint16_t c3;
    uint16_t c4;
    uint16_t c5;
    uint16_t c6;
    uint16_t crc;
} ms5611_prom_data_t;

/**
 * @brief MS5611 Prom Data (Coefficents)
 */
typedef struct ms5611_config
{
    hal_spi_instance_t spi;
    hal_pin_number_t cs;
    ms5611_prom_data_t coeffs;
    ms5611_osr_t pressOSR;
    ms5611_osr_t tempOSR;
    uint32_t d1;
    hal_usec_t nextTime;
} ms5611_config_t;

/**
 * @brief Initalize MS5611 for SPI
 *
 * @param config Configuration
 * @param spi SPI
 * @param cs CS
 */
void ms5611_init_spi(ms5611_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Sets OSR
 *
 * @param config Configuration
 * @param press Pressure OSR
 * @param temp Temperature OSR
 */
void ms5611_set_osr(ms5611_config_t *config, ms5611_osr_t press, ms5611_osr_t temp);

/**
 * @brief Reads values from MS5611
 *
 * @param config Configuration
 * @param pressure Pointer to pressure
 * @param temperature Pointer to temperature
 * @return True if values were overwritten
 */
bool ms5611_read_non_blocking(ms5611_config_t *config, int *pressure, float *temperature);

#endif