#include "drivers/adc/max1161X_driver.h"

FUNCRESULT max1161XInit(MAX1161XConfig *config, MAX1161XType type, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    if (type == MAX1161X_TYPE_2 || type == MAX1161X_TYPE_3)
    {
        config->address = 0x34;
    }
    else if (type == MAX1161X_TYPE_4 || type == MAX1161X_TYPE_5)
    {
        config->address = 0x33;
    }
    else if (type == MAX1161X_TYPE_6 || type == MAX1161X_TYPE_7)
    {
        config->address = 0x35;
    }
    else
    {
        return ERR_INVALIDARG;
    }

    config->i2c = i2c;
    config->sda = sda;
    config->scl = scl;

    i2cInitPins(i2c, sda, scl);

    __max1161XSetup(config, (0x05 << 4) | (0x01 << 1));
    __max1161XConfig(config, (0x03 << 5) | 0x01);

    if (type == MAX1161X_TYPE_2 || type == MAX1161X_TYPE_4 || type == MAX1161X_TYPE_6)
    {
        config->vRef = 4.096f;
    }
    else if (type == MAX1161X_TYPE_3 || type == MAX1161X_TYPE_5 || type == MAX1161X_TYPE_7)
    {
        config->vRef = 2.048f;
    }
    else
    {
        return ERR_INVALIDARG;
    }

    return SUC_OK;
}

FUNCRESULT max1161XRead(MAX1161XConfig *config, MAX1161XChannel channel, VoltageLevel *voltage)
{
    __max1161XConfig(config, (0x03 << 5) | ((BYTE)channel << 1) | 0x01);

    BYTE data[2];
    i2cReadBlocking(config->i2c, config->address, data, 2, FALSE);

    UINT16 value = ((data[0] & 0x0F) << 8) | data[1];

    *voltage = (FLOAT)value / (FLOAT)(1 << 12) * config->vRef;

    return SUC_OK;
}

VOID __max1161XSetup(MAX1161XConfig *config, BYTE data)
{
    data |= 0x80;

    i2cWriteBlocking(config->i2c, config->address, &data, 1, FALSE);
}

VOID __max1161XConfig(MAX1161XConfig *config, BYTE data)
{
    data &= 0x7F;

    i2cWriteBlocking(config->i2c, config->address, &data, 1, FALSE);
}