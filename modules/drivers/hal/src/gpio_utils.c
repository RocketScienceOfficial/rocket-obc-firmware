#include "modules/drivers/hal/gpio_utils.h"

bool gpio_write_reg_field(gpio_communication_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value)
{
    uint8_t data = gpio_read_reg(config, address);
    uint8_t mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - length;
    mask >>= 8 - offset - length;

    data &= ~mask;
    data |= (value << offset);

    gpio_write_reg(config, address, data);

#if OBC_DEBUG_MODE
    uint8_t read = gpio_read_reg(config, address);

    if (read != data)
    {
        return false;
    }
#endif

    return true;
}

uint8_t gpio_read_reg(gpio_communication_config_t *config, uint8_t address)
{
    uint8_t data;

    address |= config->readMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        i2c_read(config->i2c, config->i2cAddress, &data, 1, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpio_set_pin_state(config->cs, GPIO_LOW);

        spi_write(config->spi, &address, 1);
        spi_read(config->spi, 0, &data, 1);

        gpio_set_pin_state(config->cs, GPIO_HIGH);
    }

    return data;
}

void gpio_read_regs(gpio_communication_config_t *config, uint8_t address, uint8_t *buffer, size_t count)
{
    address |= config->multipleReadMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        i2c_read(config->i2c, config->i2cAddress, buffer, count, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpio_set_pin_state(config->cs, GPIO_LOW);

        spi_write(config->spi, &address, 1);
        spi_read(config->spi, 0, buffer, count);

        gpio_set_pin_state(config->cs, GPIO_HIGH);
    }
}

void gpio_write_reg(gpio_communication_config_t *config, uint8_t address, uint8_t value)
{
    address &= config->writeMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2c_write(config->i2c, config->i2cAddress, &address, 1, false);
        i2c_read(config->i2c, config->i2cAddress, &value, 1, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpio_set_pin_state(config->cs, GPIO_LOW);

        spi_write(config->spi, &address, 1);
        spi_write(config->spi, &value, 1);

        gpio_set_pin_state(config->cs, GPIO_HIGH);
    }
}

uint8_t gpio_single_read(gpio_communication_config_t *config)
{
    uint8_t data;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2c_read(config->i2c, config->i2cAddress, &data, 1, false);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpio_set_pin_state(config->cs, GPIO_LOW);

        spi_read(config->spi, 0, &data, 1);

        gpio_set_pin_state(config->cs, GPIO_HIGH);
    }

    return data;
}