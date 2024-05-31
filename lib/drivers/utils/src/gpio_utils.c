#include "lib/drivers/utils/gpio_utils.h"

bool gpio_utils_write_reg_field(gpio_utils_communication_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value)
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

    return true;
}

uint8_t gpio_utils_read_reg(gpio_utils_communication_config_t *config, uint8_t address)
{
    uint8_t data;

    address |= config->readMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        hal_i2c_read(config->i2c, config->i2cAddress, &data, 1, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        hal_spi_cs_select(config->cs);

        hal_spi_write(config->spi, &address, 1);
        hal_spi_read(config->spi, 0, &data, 1);

        hal_spi_cs_deselect(config->cs);
    }
    else
    {
        return 0;
    }

    return data;
}

void gpio_utils_read_regs(gpio_utils_communication_config_t *config, uint8_t address, uint8_t *buffer, size_t count)
{
    address |= config->multipleReadMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        hal_i2c_read(config->i2c, config->i2cAddress, buffer, count, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        hal_spi_cs_select(config->cs);

        hal_spi_write(config->spi, &address, 1);
        hal_spi_read(config->spi, 0, buffer, count);

        hal_spi_cs_deselect(config->cs);
    }
}

void gpio_utils_write_reg(gpio_utils_communication_config_t *config, uint8_t address, uint8_t value)
{
    address &= config->writeMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        hal_i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        hal_i2c_read(config->i2c, config->i2cAddress, &value, 1, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        hal_spi_cs_select(config->cs);

        hal_spi_write(config->spi, &address, 1);
        hal_spi_write(config->spi, &value, 1);

        hal_spi_cs_deselect(config->cs);
    }
}

uint8_t gpio_utils_single_read(gpio_utils_communication_config_t *config)
{
    uint8_t data;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        hal_i2c_read(config->i2c, config->i2cAddress, &data, 1, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        hal_spi_cs_select(config->cs);

        hal_spi_read(config->spi, 0, &data, 1);

        hal_spi_cs_deselect(config->cs);
    }
    else
    {
        return 0;
    }

    return data;
}