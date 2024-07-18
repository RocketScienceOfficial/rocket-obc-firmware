#include "hal/spi_driver.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

static spi_inst_t *_get_spi(hal_spi_instance_t spi);
static bool _spi_check_instance(hal_spi_instance_t spi);
static bool _spi_check_miso(hal_spi_instance_t spi, hal_pin_number_t miso);
static bool _spi_check_mosi(hal_spi_instance_t spi, hal_pin_number_t mosi);
static bool _spi_check_sck(hal_spi_instance_t spi, hal_pin_number_t sck);

bool hal_spi_init_all(hal_spi_instance_t spi, hal_pin_number_t miso, hal_pin_number_t mosi, hal_pin_number_t sck, hal_baud_rate_t baudrate)
{
    if (!_spi_check_instance(spi) || !_spi_check_miso(spi, miso) || !_spi_check_mosi(spi, mosi) || !_spi_check_sck(spi, sck))
    {
        return false;
    }

    spi_init(_get_spi(spi), baudrate);

    bool v1 = hal_gpio_set_pin_function(miso, GPIO_FUNCTION_SPI);
    bool v2 = hal_gpio_set_pin_function(mosi, GPIO_FUNCTION_SPI);
    bool v3 = hal_gpio_set_pin_function(sck, GPIO_FUNCTION_SPI);

    return v1 && v2 && v3;
}

bool hal_spi_init_cs(hal_pin_number_t cs)
{
    if (hal_gpio_init_pin(cs, GPIO_OUTPUT))
    {
        return hal_gpio_set_pin_state(cs, GPIO_HIGH);
    }

    return false;
}

bool hal_spi_cs_select(hal_pin_number_t cs)
{
    return hal_gpio_set_pin_state(cs, GPIO_LOW);
}

bool hal_spi_cs_deselect(hal_pin_number_t cs)
{
    return hal_gpio_set_pin_state(cs, GPIO_HIGH);
}

bool hal_spi_write(hal_spi_instance_t spi, const uint8_t *data, size_t size)
{
    if (!_spi_check_instance(spi) || !data || size == 0)
    {
        return false;
    }

    if (spi_write_blocking(_get_spi(spi), data, size) < 0)
    {
        return false;
    }

    return true;
}

bool hal_spi_read(hal_spi_instance_t spi, uint8_t repeatedTXData, uint8_t *destination, size_t size)
{
    if (!_spi_check_instance(spi) || !destination || size == 0)
    {
        return false;
    }

    if (spi_read_blocking(_get_spi(spi), repeatedTXData, destination, size) < 0)
    {
        return false;
    }

    return true;
}

bool hal_spi_write_read(hal_spi_instance_t spi, const uint8_t *data, uint8_t *destination, size_t size)
{
    if (!_spi_check_instance(spi) || !destination || size == 0)
    {
        return false;
    }

    if (spi_write_read_blocking(_get_spi(spi), data, destination, size) < 0)
    {
        return false;
    }

    return true;
}

static spi_inst_t *_get_spi(hal_spi_instance_t spi)
{
    return (spi == 0 ? spi0 : spi1);
}

static bool _spi_check_instance(hal_spi_instance_t spi)
{
    return spi >= 0 && spi <= 1;
}

static bool _spi_check_miso(hal_spi_instance_t spi, hal_pin_number_t miso)
{
    if (!_spi_check_instance(spi))
    {
        return false;
    }

    if (spi == 0)
    {
        return miso == 0 || miso == 4 || miso == 16;
    }
    else if (spi == 1)
    {
        return miso == 8 || miso == 12;
    }
    else
    {
        return false;
    }
}

static bool _spi_check_mosi(hal_spi_instance_t spi, hal_pin_number_t mosi)
{
    if (!_spi_check_instance(spi))
    {
        return false;
    }

    if (spi == 0)
    {
        return mosi == 3 || mosi == 7 || mosi == 19;
    }
    else if (spi == 1)
    {
        return mosi == 11 || mosi == 15;
    }
    else
    {
        return false;
    }
}

static bool _spi_check_sck(hal_spi_instance_t spi, hal_pin_number_t sck)
{
    if (!_spi_check_instance(spi))
    {
        return false;
    }

    if (spi == 0)
    {
        return sck == 2 || sck == 6 || sck == 18;
    }
    else if (spi == 1)
    {
        return sck == 10 || sck == 14;
    }
    else
    {
        return false;
    }
}