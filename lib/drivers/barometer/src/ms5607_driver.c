#include "lib/drivers/barometer/ms5607_driver.h"
#include "hal/time_tracker.h"
#include <stdbool.h>
#include <stddef.h>

#define RESET 0x1E
#define CONVERT_D1_OSR_256 0x40
#define CONVERT_D1_OSR_512 0x42
#define CONVERT_D1_OSR_1024 0x44
#define CONVERT_D1_OSR_2048 0x46
#define CONVERT_D1_OSR_4096 0x48
#define CONVERT_D2_OSR_256 0x50
#define CONVERT_D2_OSR_512 0x52
#define CONVERT_D2_OSR_1024 0x54
#define CONVERT_D2_OSR_2048 0x56
#define CONVERT_D2_OSR_4096 0x58
#define ADC_READ 0x00
#define PROM_READ 0xA0

static void _ms5607_reset(const ms5607_config_t *config);
static void _ms5607_read_coefficents(ms5607_config_t *config);
static uint32_t _ms5607_read_raw_value(const ms5607_config_t *config, bool pressure);
static void _ms5607_convert_raw_values(const ms5607_prom_data_t *coeffs, uint32_t d1, uint32_t d2, int *pressure, float *temperature);

void ms5607_init_spi(ms5607_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->spi = spi;
    config->cs = cs;

    hal_spi_init_cs(cs);

    _ms5607_reset(config);
    _ms5607_read_coefficents(config);
}

void ms5607_set_osr(ms5607_config_t *config, ms5607_osr_t press, ms5607_osr_t temp)
{
    config->pressOSR = press;
    config->tempOSR = temp;
}

void ms5607_read(const ms5607_config_t *config, int *pressure, float *temperature)
{
    uint32_t d1 = _ms5607_read_raw_value(config, true);
    uint32_t d2 = _ms5607_read_raw_value(config, false);

    _ms5607_convert_raw_values(&config->coeffs, d1, d2, pressure, temperature);
}

static void _ms5607_reset(const ms5607_config_t *config)
{
    uint8_t data = RESET;

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, &data, 1);
    hal_spi_cs_deselect(config->cs);

    hal_time_sleep_ms(5);
}

void _ms5607_read_coefficents(ms5607_config_t *config)
{
    ms5607_prom_data_t coeffs = {0};

    for (size_t i = 0; i < 8; i++)
    {
        uint8_t addr = PROM_READ + 2 * i;
        uint8_t data[2] = {0};

        hal_spi_cs_select(config->cs);
        hal_spi_write(config->spi, &addr, 1);
        hal_spi_read(config->spi, 0, data, 2);
        hal_spi_cs_deselect(config->cs);

        ((uint16_t *)&coeffs)[i] = data[0] << 8 | data[1];
    }

    // TODO: Calculate CRC-4 and validate it

    config->coeffs = coeffs;
}

static uint32_t _ms5607_read_raw_value(const ms5607_config_t *config, bool pressure)
{
    uint8_t data = pressure ? config->pressOSR : config->tempOSR + CONVERT_D2_OSR_256 - CONVERT_D1_OSR_256;

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, &data, 1);
    hal_spi_cs_deselect(config->cs);

    // TODO: Remove sleep

    uint8_t osr = pressure ? config->pressOSR : config->tempOSR;
    hal_time_sleep_ms(osr == MS5607_OSR_4096 ? 10 : osr == MS5607_OSR_2048 || osr == MS5607_OSR_1024 ? 5
                                                       : osr == MS5607_OSR_512                                     ? 2
                                                       : osr == MS5607_OSR_256                                     ? 1
                                                                                                                          : 0);

    uint8_t newData = ADC_READ;
    uint8_t buffer[3];

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, &newData, 1);    
    hal_spi_read(config->spi, 0, buffer, sizeof(buffer));
    hal_spi_cs_deselect(config->cs);

    uint32_t d = (uint32_t)((uint32_t)buffer[0] << 16 | (uint32_t)buffer[1] << 8 | (uint32_t)buffer[2]);

    return d;
}

static void _ms5607_convert_raw_values(const ms5607_prom_data_t *coeffs, uint32_t d1, uint32_t d2, int *pressure, float *temperature)
{
    int32_t dT = (int32_t)d2 - ((int32_t)coeffs->c5 << 8);
    int32_t temp = 2000 + (((int64_t)dT * coeffs->c6) >> 23);
    int64_t off = ((int64_t)coeffs->c2 << 17) + (((int64_t)coeffs->c4 * dT) >> 6);
    int64_t sens = ((int64_t)coeffs->c1 << 16) + (((int64_t)coeffs->c3 * dT) >> 7);

    if (temp < 2000)
    {
        int32_t t2 = ((int64_t)dT * (int64_t)dT) >> 31;
        int32_t temp_2000 = temp - 2000;
        int64_t off2 = (61 * (int64_t)temp_2000 * (int64_t)temp_2000) >> 4;
        int64_t sens2 = 2 * (int64_t)temp_2000 * (int64_t)temp_2000;

        if (temp < -1500)
        {
            int32_t temp_1500 = temp + 1500;

            off2 += 15 * (int64_t)temp_1500 * (int64_t)temp_1500;
            sens2 += 8 * (int64_t)temp_1500 * (int64_t)temp_1500;
        }

        temp -= t2;
        off -= off2;
        sens -= sens2;
    }

    int32_t p = ((((int64_t)d1 * sens) >> 21) - off) >> 15;

    *temperature = (float)temp / 100.0f;
    *pressure = p;
}