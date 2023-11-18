#include "modules/drivers/hal/i2c_driver.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

static i2c_inst_t *__get_i2c(i2c_instance_t i2c)
{
    return (i2c == 0 ? i2c0 : i2c1);
}

bool i2c_check_instance(i2c_instance_t i2c)
{
    return i2c >= 0 && i2c <= 1;
}

bool i2c_check_sda(i2c_instance_t i2c, pin_number_t sda)
{
    if (!i2c_check_instance(i2c))
    {
        return false;
    }

    if (i2c == 0)
    {
        return sda == 0 || sda == 4 || sda == 8 || sda == 12 || sda == 16 || sda == 20;
    }
    else if (i2c == 1)
    {
        return sda == 2 || sda == 6 || sda == 10 || sda == 14 || sda == 18 || sda == 26;
    }
    else
    {
        return false;
    }
}

bool i2c_check_scl(i2c_instance_t i2c, pin_number_t scl)
{
    if (!i2c_check_instance(i2c))
    {
        return false;
    }

    if (i2c == 0)
    {
        return scl == 1 || scl == 5 || scl == 9 || scl == 13 || scl == 17 || scl == 21;
    }
    else if (i2c == 1)
    {
        return scl == 3 || scl == 7 || scl == 11 || scl == 15 || scl == 19 || scl == 27;
    }
    else
    {
        return false;
    }
}

void i2c_init_all(i2c_instance_t i2c, unsigned long baudrate)
{
    if (!i2c_check_instance(i2c))
    {
        return;
    }

    i2c_init(__get_i2c(i2c), baudrate);
}

void i2c_init_pins(i2c_instance_t i2c, pin_number_t sda, pin_number_t scl)
{
    if (!i2c_check_instance(i2c) || !i2c_check_sda(i2c, sda) || !i2c_check_scl(i2c, scl))
    {
        return;
    }

    gpio_set_pin_function(sda, GPIO_FUNCTION_I2C);
    gpio_set_pin_function(scl, GPIO_FUNCTION_I2C);
    gpio_pull_up_pin(sda);
    gpio_pull_up_pin(scl);
}

bool i2c_write(i2c_instance_t i2c, uint8_t address, const uint8_t *data, size_t size, bool nostop)
{
    if (!i2c_check_instance(i2c) || !data)
    {
        return false;
    }

    if (i2c_write_blocking(__get_i2c(i2c), address, data, size, nostop) < 0)
    {
        return false;
    }

    return true;
}

bool i2c_read(i2c_instance_t i2c, uint8_t address, uint8_t *destination, size_t size, bool nostop)
{
    if (!i2c_check_instance(i2c) || !destination)
    {
        return false;
    }

    if (i2c_read_blocking(__get_i2c(i2c), address, destination, size, nostop) < 0)
    {
        return false;
    }

    return true;
}