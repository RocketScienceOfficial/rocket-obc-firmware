#include "modules/drivers/barometer/ms56xx_02ba03.h"
#include "modules/drivers/hal/time_tracker.h"
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

static void _ms56xx_02ba03_reset(const ms56xx_02ba03_config_t *config);
static void _ms56xx_02ba03_read_coefficents(ms56xx_02ba03_config_t *config);
static uint32_t _ms56xx_02ba03_read_raw_value(const ms56xx_02ba03_config_t *config, bool pressure);
static void _ms56xx_02ba03_convert_raw_values(const ms56xx_02ba03_prom_data_t *coeffs, uint32_t d1, uint32_t d2, int *pressure, float *temperature);

void ms56xx_02ba03_init_spi(ms56xx_02ba03_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->spi = spi;
    config->cs = cs;

    _ms56xx_02ba03_reset(config);
    _ms56xx_02ba03_read_coefficents(config);
}

void ms56xx_02ba03_set_osr(ms56xx_02ba03_config_t *config, ms56xx_02ba03_osr_t press, ms56xx_02ba03_osr_t temp)
{
    config->pressOSR = press;
    config->tempOSR = temp;
}

void ms56xx_02ba03_read(const ms56xx_02ba03_config_t *config, int *pressure, float *temperature)
{
    uint32_t d1 = _ms56xx_02ba03_read_raw_value(config, true);
    uint32_t d2 = _ms56xx_02ba03_read_raw_value(config, false);

    _ms56xx_02ba03_convert_raw_values(&config->coeffs, d1, d2, pressure, temperature);
}

static void _ms56xx_02ba03_reset(const ms56xx_02ba03_config_t *config)
{
    uint8_t data = RESET;

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, &data, 1);
    hal_spi_cs_deselect(config->cs);

    hal_time_sleep_ms(5);
}

void _ms56xx_02ba03_read_coefficents(ms56xx_02ba03_config_t *config)
{
    ms56xx_02ba03_prom_data_t coeffs = {0};

    for (size_t i = 0; i < 8; i++)
    {
        uint8_t addr = PROM_READ + 2 * i;
        uint8_t data[2];

        hal_spi_cs_select(config->cs);
        hal_spi_write(config->spi, &addr, 1);
        hal_spi_read(config->spi, 0, data, 2);
        hal_spi_cs_deselect(config->cs);

        ((uint16_t *)&coeffs)[i] = data[1] | data[0] << 8;
    }

    // TODO: Calculate CRC-4 and validate it

    config->coeffs = coeffs;
}

static uint32_t _ms56xx_02ba03_read_raw_value(const ms56xx_02ba03_config_t *config, bool pressure)
{
    uint8_t data = pressure ? config->pressOSR : config->tempOSR + CONVERT_D2_OSR_256 - CONVERT_D1_OSR_256;

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, &data, 1);
    hal_spi_cs_deselect(config->cs);

    // TODO: Remove sleep

    uint8_t osr = pressure ? config->pressOSR : config->tempOSR;
    hal_time_sleep_ms(osr == MS56XX_02BA03_OSR_4096 ? 10 : osr == MS56XX_02BA03_OSR_2048 || osr == MS56XX_02BA03_OSR_1024 ? 5
                                                       : osr == MS56XX_02BA03_OSR_512                                     ? 2
                                                       : osr == MS56XX_02BA03_OSR_256                                     ? 1
                                                                                                                          : 0);

    uint8_t buffer[3];

    hal_spi_cs_select(config->cs);
    hal_spi_read(config->spi, 0, buffer, sizeof(buffer));
    hal_spi_cs_deselect(config->cs);

    uint32_t d = buffer[2] | buffer[1] << 8 | buffer[0] << 16;

    return d;
}

static void _ms56xx_02ba03_convert_raw_values(const ms56xx_02ba03_prom_data_t *coeffs, uint32_t d1, uint32_t d2, int *pressure, float *temperature)
{
    int32_t dT = d2 - coeffs->c5 * (1 << 8);
    int32_t temp = 2000 + dT * coeffs->c6 / (1 << 23);
    int64_t off = coeffs->c2 * (1 << 17) + coeffs->c4 * dT / (1 << 6);
    int64_t sens = coeffs->c1 * (1 << 16) + coeffs->c3 * dT / (1 << 7);

    if (temp < 2000)
    {
        int32_t t2 = dT * dT / (1 << 31);
        int32_t temp_2000 = (temp - 2000);
        int64_t off2 = 61 * temp_2000 * temp_2000 / (1 << 4);
        int64_t sens2 = 2 * temp_2000 * temp_2000;

        if (temp < -1500)
        {
            int32_t temp_1500 = (temp + 1500);

            off2 += 15 * temp_1500 * temp_1500;
            sens2 += 8 * temp_1500 * temp_1500;
        }

        temp -= t2;
        off -= off2;
        sens -= sens2;
    }

    int32_t p = ((d1 * sens) / ((1 << 21) - off)) / (1 << 15);

    *temperature = (float)temp / 100.0f;
    *pressure = p;
}