#include "modules/drivers/hal/spi_driver.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

static spi_inst_t *__get_spi(spi_instance_t spi)
{
    return (spi == 0 ? spi0 : spi1);
}

bool spi_check_instance(spi_instance_t spi)
{
    return spi >= 0 && spi <= 1;
}

bool spi_check_miso(spi_instance_t spi, pin_number_t miso)
{
    if (!spi_check_instance(spi))
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

bool spi_check_mosi(spi_instance_t spi, pin_number_t mosi)
{
    if (!spi_check_instance(spi))
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

bool spi_check_sck(spi_instance_t spi, pin_number_t sck)
{
    if (!spi_check_instance(spi))
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

void spi_init_all(spi_instance_t spi, unsigned long baudrate)
{
    if (!spi_check_instance(spi))
    {
        return;
    }

    spi_init(__get_spi(spi), baudrate);
}

void spi_init_pins(spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t sck, pin_number_t cs)
{
    if (!spi_check_instance(spi) || !spi_check_miso(spi, miso) || !spi_check_mosi(spi, mosi) || !spi_check_sck(spi, sck))
    {
        return;
    }

    gpio_init_pin(cs, GPIO_OUTPUT);
    gpio_set_pin_state(cs, GPIO_HIGH);

    gpio_set_pin_function(miso, GPIO_FUNCTION_SPI);
    gpio_set_pin_function(mosi, GPIO_FUNCTION_SPI);
    gpio_set_pin_function(sck, GPIO_FUNCTION_SPI);
}

bool spi_write(spi_instance_t spi, const uint8_t *data, size_t size)
{
    if (!spi_check_instance(spi) || !data)
    {
        return false;
    }

    if (spi_write_blocking(__get_spi(spi), data, size) < 0)
    {
        return false;
    }

    return true;
}

bool spi_read(spi_instance_t spi, uint8_t repeatedTXData, uint8_t *destination, size_t size)
{
    if (!spi_check_instance(spi) || !destination)
    {
        return false;
    }

    if (spi_read_blocking(__get_spi(spi), repeatedTXData, destination, size) < 0)
    {
        return false;
    }

    return true;
}

bool spi_write_read(spi_instance_t spi, const uint8_t *data, uint8_t *destination, size_t size)
{
    if (!spi_check_instance(spi) || !destination)
    {
        return false;
    }

    if (spi_write_read_blocking(__get_spi(spi), data, destination, size) < 0)
    {
        return false;
    }

    return true;
}