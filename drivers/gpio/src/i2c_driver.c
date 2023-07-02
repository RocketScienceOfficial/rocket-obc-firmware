#include "drivers/gpio/i2c_driver.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

static i2c_inst_t *getI2C(I2CInstance i2c)
{
    return (i2c == 0 ? i2c0 : i2c1);
}

BOOL i2cCheckInstance(I2CInstance i2c)
{
    return i2c >= 0 && i2c <= 1;
}

BOOL i2cCheckSDA(I2CInstance i2c, PinNumber sda)
{
    if (!i2cCheckInstance(i2c))
    {
        return FALSE;
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
        return FALSE;
    }
}

BOOL i2cCheckSCL(I2CInstance i2c, PinNumber scl)
{
    if (!i2cCheckInstance(i2c))
    {
        return FALSE;
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
        return FALSE;
    }
}

FUNCRESULT i2cInitAll(I2CInstance i2c, BaudRate baudrate)
{
    if (!i2cCheckInstance(i2c))
    {
        return ERR_INVALIDARG;
    }

    i2c_init(getI2C(i2c), baudrate);

    return SUC_OK;
}

FUNCRESULT i2cInitPins(I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    if (!i2cCheckInstance(i2c) || !i2cCheckSDA(i2c, sda) || !i2cCheckSCL(i2c, scl))
    {
        return ERR_INVALIDARG;
    }

    gpioSetPinFunction(sda, GPIO_FUNCTION_I2C);
    gpioSetPinFunction(scl, GPIO_FUNCTION_I2C);
    gpioPullUpPin(sda);
    gpioPullUpPin(scl);

    return SUC_OK;
}

FUNCRESULT i2cWriteTimeout(I2CInstance i2c, BYTE address, BYTE *data, SIZE size, TIME timeout, BOOL nostop)
{
    if (!i2cCheckInstance(i2c) || !data)
    {
        return ERR_INVALIDARG;
    }

    if (i2c_write_timeout_us(getI2C(i2c), address, data, size, nostop, timeout) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT i2cReadTimeout(I2CInstance i2c, BYTE address, BYTE *destination, SIZE size, TIME timeout, BOOL nostop)
{
    if (!i2cCheckInstance(i2c) || !destination)
    {
        return ERR_INVALIDARG;
    }

    if (i2c_read_timeout_us(getI2C(i2c), address, destination, size, nostop, timeout) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT i2cWriteBlocking(I2CInstance i2c, BYTE address, const BYTE *data, SIZE size, BOOL nostop)
{
    if (!i2cCheckInstance(i2c) || !data)
    {
        return ERR_INVALIDARG;
    }

    if (i2c_write_blocking(getI2C(i2c), address, data, size, nostop) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT i2cReadBlocking(I2CInstance i2c, BYTE address, BYTE *destination, SIZE size, BOOL nostop)
{
    if (!i2cCheckInstance(i2c) || !destination)
    {
        return ERR_INVALIDARG;
    }

    if (i2c_read_blocking(getI2C(i2c), address, destination, size, nostop) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

BOOL i2cCheckDevice(I2CInstance i2c, BYTE address)
{
    BYTE data = 0;

    return FUNCSUCCESS(i2cReadTimeout(i2c, address, &data, 1, 1E6, FALSE));
}