#include "modules/drivers/utils/gpio_utils.h"
#include "modules/logger/logger.h"

bool hal_gpio_write_reg_field(hal_gpio_communication_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value)
{
    uint8_t data = hal_gpio_read_reg(config, address);
    uint8_t mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - length;
    mask >>= 8 - offset - length;

    data &= ~mask;
    data |= (value << offset);

    hal_gpio_write_reg(config, address, data);

#if !NDEBUG
    uint8_t read = hal_gpio_read_reg(config, address);

    if (read != data)
    {
        OBC_ERR("Read-check of register failed. Read: %d, Wrote: %d", read, data);

        return false;
    }
#endif

    return true;
}

uint8_t hal_gpio_read_reg(hal_gpio_communication_config_t *config, uint8_t address)
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
        OBC_WARN("Unsupported gpio protocol");

        return 0;
    }

    return data;
}

void hal_gpio_read_regs(hal_gpio_communication_config_t *config, uint8_t address, uint8_t *buffer, size_t count)
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
    else
    {
        OBC_WARN("Unsupported gpio protocol");
    }
}

void hal_gpio_write_reg(hal_gpio_communication_config_t *config, uint8_t address, uint8_t value)
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
    else
    {
        OBC_WARN("Unsupported gpio protocol");
    }
}

uint8_t hal_gpio_single_read(hal_gpio_communication_config_t *config)
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
        OBC_WARN("Unsupported gpio protocol");

        return 0;
    }

    return data;
}