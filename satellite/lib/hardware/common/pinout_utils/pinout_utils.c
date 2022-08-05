#include "pinout_utils.h"

bool pinoutCheckPin(int pin)
{
    return pin >= 0 && pin <= 28;
}

bool pinoutCheckI2C(int i2c)
{
    return i2c == 0 || i2c == 1;
}

bool pinoutCheckI2C_SDA(int i2c, int sda)
{
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

bool pinoutCheckI2C_SCL(int i2c, int scl)
{
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