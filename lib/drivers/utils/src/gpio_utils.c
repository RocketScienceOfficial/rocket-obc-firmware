#include "lib/drivers/utils/gpio_utils.h"

void gpio_utils_write_reg_field(const gpio_utils_communication_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value)
{
    uint8_t data = gpio_utils_read_reg(config, address);
    uint8_t mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - length;
    mask >>= 8 - offset - length;

    data &= ~mask;
    data |= (value << offset);

    gpio_utils_write_reg(config, address, data);
}

void gpio_utils_write_reg(const gpio_utils_communication_config_t *config, uint8_t address, uint8_t value)
{
    address &= config->writeMask;

    if (!config->useSPI)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        hal_i2c_read(config->i2c, config->i2cAddress, &value, 1, false);
    }
    else
    {
        hal_spi_cs_select(config->cs);

        hal_spi_write(config->spi, &address, 1);
        hal_spi_write(config->spi, &value, 1);

        hal_spi_cs_deselect(config->cs);
    }
}

void gpio_utils_single_write(const gpio_utils_communication_config_t *config, uint8_t value)
{
    if (!config->useSPI)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &value, 1, false);
    }
    else
    {
        hal_spi_cs_select(config->cs);

        hal_spi_read(config->spi, 0, &value, 1);

        hal_spi_cs_deselect(config->cs);
    }
}

uint8_t gpio_utils_read_reg(const gpio_utils_communication_config_t *config, uint8_t address)
{
    address |= config->readMask;

    uint8_t data;

    if (!config->useSPI)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        hal_i2c_read(config->i2c, config->i2cAddress, &data, 1, false);
    }
    else
    {
        hal_spi_cs_select(config->cs);

        hal_spi_write(config->spi, &address, 1);
        hal_spi_read(config->spi, 0, &data, 1);

        hal_spi_cs_deselect(config->cs);
    }

    return data;
}

void gpio_utils_read_regs(const gpio_utils_communication_config_t *config, uint8_t address, uint8_t *buffer, size_t count)
{
    address |= config->multipleReadMask;

    if (!config->useSPI)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        hal_i2c_read(config->i2c, config->i2cAddress, buffer, count, false);
    }
    else
    {
        hal_spi_cs_select(config->cs);

        hal_spi_write(config->spi, &address, 1);
        hal_spi_read(config->spi, 0, buffer, count);

        hal_spi_cs_deselect(config->cs);
    }
}

uint8_t gpio_utils_single_read(const gpio_utils_communication_config_t *config)
{
    uint8_t data;

    if (!config->useSPI)
    {
        hal_i2c_read(config->i2c, config->i2cAddress, &data, 1, false);
    }
    else
    {
        hal_spi_cs_select(config->cs);

        hal_spi_read(config->spi, 0, &data, 1);

        hal_spi_cs_deselect(config->cs);
    }

    return data;
}