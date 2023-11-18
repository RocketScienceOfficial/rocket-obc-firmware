#include "modules/drivers/accelerometer/h3lis331dl_driver.h"
#include <stdint.h>

#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define HP_FILTER_RESET 0x25
#define REFERENCE 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define INT_1_CFG 0x30
#define INT_1_SRC 0x31
#define INT_1_THS 0x32
#define INT_1_DURATION 0x33
#define INT_2_CFG 0x34
#define INT_2_SRC 0x35
#define INT_2_THS 0x36
#define INT_2_DURATION 0x37

#define I2C_ADDRESS 0x18
#define WHO_AM_I_VALUE 0x32
#define RESOLUTION_DIVIDER 32768.0f

#define X_OFFSET -95
#define Y_OFFSET -45
#define Z_OFFSET 420

void h3lis331dl_init_spi(h3lis331dl_config_t *config, spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t cs, pin_number_t sck)
{
    config->gpioConfig = (gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0xC0,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0;

    spi_init_pins(spi, miso, mosi, sck, cs);
}

void h3lis331dl_init_i2c(h3lis331dl_config_t *config, i2c_instance_t i2c, pin_number_t sda, pin_number_t scl)
{
    config->gpioConfig = (gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0xC0,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0;

    i2c_init_pins(i2c, sda, scl);
}

void h3lis331dl_validate_id(h3lis331dl_config_t *config, bool *valid)
{
    *valid = gpio_read_reg(&config->gpioConfig, WHO_AM_I) == WHO_AM_I_VALUE;
}

void h3lis331dl_set_power_mode(h3lis331dl_config_t *config, h3lis331dl_power_mode_t power)
{
    gpio_write_reg_field(&config->gpioConfig, CTRL_REG1, 3, 5, (uint8_t)power);
}

void h3lis331dl_set_odr(h3lis331dl_config_t *config, h3lis331dl_odr_t odr)
{
    gpio_write_reg_field(&config->gpioConfig, CTRL_REG1, 2, 3, (uint8_t)odr);
}

void h3lis331dl_set_hpfc(h3lis331dl_config_t *config, h3lis331dl_hpfc_t hpcf)
{
    gpio_write_reg_field(&config->gpioConfig, CTRL_REG2, 2, 0, (uint8_t)hpcf);
}

void h3lis331dl_set_range(h3lis331dl_config_t *config, h3lis331dl_range_t range)
{
    gpio_write_reg_field(&config->gpioConfig, CTRL_REG4, 2, 4, (uint8_t)range);

    switch (range)
    {
    case H3LIS331DL_RANGE_100G:
        config->rangeFactor = 100.0f / RESOLUTION_DIVIDER;
        break;
    case H3LIS331DL_RANGE_200G:
        config->rangeFactor = 200.0f / RESOLUTION_DIVIDER;
        break;
    case H3LIS331DL_RANGE_400G:
        config->rangeFactor = 400.0f / RESOLUTION_DIVIDER;
        break;
    default:
        return;
    }
}

void h3lis331dl_read(h3lis331dl_config_t *config, vec3_t *accel)
{
    uint8_t buffer[6];

    gpio_read_regs(&config->gpioConfig, OUT_X_L, buffer, 6);

    int16_t x = (int16_t)((buffer[1] << 8) | buffer[0]) - X_OFFSET;
    int16_t y = (int16_t)((buffer[3] << 8) | buffer[2]) - Y_OFFSET;
    int16_t z = (int16_t)((buffer[5] << 8) | buffer[4]) - Z_OFFSET;

    accel->x = (float)x * config->rangeFactor;
    accel->y = (float)y * config->rangeFactor;
    accel->z = (float)z * config->rangeFactor;
}