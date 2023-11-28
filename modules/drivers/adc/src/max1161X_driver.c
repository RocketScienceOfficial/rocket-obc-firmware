#include "modules/drivers/adc/max1161X_driver.h"
#include "modules/logger/logger.h"

static void _max1161x_setup(max1161x_config_t *config, uint8_t data);
static void _max1161x_config(max1161x_config_t *config, uint8_t data);

void max1161x_init(max1161x_config_t *config, max1161x_type_t type, hal_i2c_instance_t i2c, hal_pin_number_t sda, hal_pin_number_t scl)
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
        OBC_ERR("Unsupported type of MAX1161X: %d", type);

        return;
    }

    config->i2c = i2c;
    config->sda = sda;
    config->scl = scl;

    hal_i2c_init_pins(i2c, sda, scl);

    _max1161x_setup(config, (0x05 << 4) | (0x01 << 1));
    _max1161x_config(config, (0x03 << 5) | 0x01);

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
        OBC_ERR("Unsupported type of MAX1161X: %d", type);

        return;
    }
}

void max1161x_read(max1161x_config_t *config, max1161x_channel_t channel, hal_voltage_level_t *voltage)
{
    _max1161x_config(config, (0x03 << 5) | ((uint8_t)channel << 1) | 0x01);

    uint8_t data[2];
    hal_i2c_read(config->i2c, config->address, data, 2, false);

    uint16_t value = ((data[0] & 0x0F) << 8) | data[1];

    *voltage = (float)value / (float)(1 << 12) * config->vRef;
}

static void _max1161x_setup(max1161x_config_t *config, uint8_t data)
{
    data |= 0x80;

    hal_i2c_write(config->i2c, config->address, &data, 1, false);
}

static void _max1161x_config(max1161x_config_t *config, uint8_t data)
{
    data &= 0x7F;

    hal_i2c_write(config->i2c, config->address, &data, 1, false);
}