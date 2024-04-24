#include "modules/drivers/adc/ads7038_q1_driver.h"

#define SEQUENCE_CFG 0x10
#define MANUAL_CH_SEL 0x11
#define AUTO_SEQ_CH_SEL 0x12

static uint8_t _readReg(const ads7038_q1_config_t *config, uint8_t addr);
static void _writeReg(const ads7038_q1_config_t *config, uint8_t addr, uint8_t val, uint8_t len, uint8_t offset);

void ads7038_q1_init(ads7038_q1_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs, uint8_t channelsMask, hal_voltage_level_t vRef)
{
    config->spi = spi;
    config->cs = cs;
    config->vRef = vRef;

    _writeReg(config, SEQUENCE_CFG, 1, 2, 0);
    _writeReg(config, AUTO_SEQ_CH_SEL, channelsMask, 8, 0);
}

void ads7038_q1_read_channels(const ads7038_q1_config_t *config, hal_voltage_level_t *values, size_t len)
{
    _writeReg(config, SEQUENCE_CFG, 1, 1, 4);

    for (size_t i = 0; i < len + 1; i++)
    {
        uint8_t newData[2];

        hal_spi_cs_select(config->cs);
        hal_spi_read(config->spi, 0, newData, sizeof(newData));
        hal_spi_cs_deselect(config->cs);

        if (i > 0)
        {
            uint16_t raw = newData[0] << 4 | newData[1] >> 4;

            values[i - 1] = config->vRef * raw / (1 << 12);
        }
    }

    _writeReg(config, SEQUENCE_CFG, 0, 1, 4);
}

static uint8_t _readReg(const ads7038_q1_config_t *config, uint8_t addr)
{
    uint8_t data[3] = {1 << 4, addr, 0};

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, data, sizeof(data));
    hal_spi_cs_deselect(config->cs);

    uint8_t newData;

    hal_spi_cs_select(config->cs);
    hal_spi_read(config->spi, 0, &newData, 1);
    hal_spi_cs_deselect(config->cs);

    return newData;
}

static void _writeReg(const ads7038_q1_config_t *config, uint8_t addr, uint8_t val, uint8_t len, uint8_t offset)
{
    uint8_t regValue = _readReg(config, addr);
    uint8_t mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - len;
    mask >>= 8 - offset - len;
    regValue &= ~mask;
    regValue |= (val << offset);

    uint8_t data[3] = {1 << 3, addr, regValue};

    hal_spi_cs_select(config->cs);
    hal_spi_write(config->spi, data, sizeof(data));
    hal_spi_cs_deselect(config->cs);
}