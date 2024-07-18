#include "hal/i2c_driver.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

static i2c_inst_t *_get_i2c(hal_i2c_instance_t i2c);
static bool _i2c_check_instance(hal_i2c_instance_t i2c);
static bool _i2c_check_sda(hal_i2c_instance_t i2c, hal_pin_number_t sda);
static bool _i2c_check_scl(hal_i2c_instance_t i2c, hal_pin_number_t scl);

bool hal_i2c_init_all(hal_i2c_instance_t i2c, hal_pin_number_t sda, hal_pin_number_t scl, hal_baud_rate_t baudrate)
{
    if (!_i2c_check_instance(i2c) || !_i2c_check_sda(i2c, sda) || !_i2c_check_scl(i2c, scl))
    {
        return false;
    }

    i2c_init(_get_i2c(i2c), baudrate);

    bool v1 = hal_gpio_set_pin_function(sda, GPIO_FUNCTION_I2C);
    bool v2 = hal_gpio_set_pin_function(scl, GPIO_FUNCTION_I2C);
    bool v3 = hal_gpio_pull_up_pin(sda);
    bool v4 = hal_gpio_pull_up_pin(scl);

    return v1 && v2 && v3 && v4;
}

bool hal_i2c_write(hal_i2c_instance_t i2c, uint8_t address, const uint8_t *data, size_t size, bool nostop)
{
    if (!_i2c_check_instance(i2c) || !data || size == 0)
    {
        return false;
    }

    if (i2c_write_blocking(_get_i2c(i2c), address, data, size, nostop) < 0)
    {
        return false;
    }

    return true;
}

bool hal_i2c_read(hal_i2c_instance_t i2c, uint8_t address, uint8_t *destination, size_t size, bool nostop)
{
    if (!_i2c_check_instance(i2c) || !destination || size == 0)
    {
        return false;
    }

    if (i2c_read_blocking(_get_i2c(i2c), address, destination, size, nostop) < 0)
    {
        return false;
    }

    return true;
}

static i2c_inst_t *_get_i2c(hal_i2c_instance_t i2c)
{
    return (i2c == 0 ? i2c0 : i2c1);
}

static bool _i2c_check_instance(hal_i2c_instance_t i2c)
{
    return i2c >= 0 && i2c <= 1;
}

static bool _i2c_check_sda(hal_i2c_instance_t i2c, hal_pin_number_t sda)
{
    if (!_i2c_check_instance(i2c))
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

static bool _i2c_check_scl(hal_i2c_instance_t i2c, hal_pin_number_t scl)
{
    if (!_i2c_check_instance(i2c))
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