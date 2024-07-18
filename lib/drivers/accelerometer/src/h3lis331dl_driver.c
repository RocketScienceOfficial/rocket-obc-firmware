#include "lib/drivers/accelerometer/h3lis331dl_driver.h"
#include "lib/geo/physical_constants.h"
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
#define H3LIS_RESOLUTION 32768.0f

void h3lis331dl_init_spi(h3lis331dl_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = true,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0xC0,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0;

    hal_spi_init_cs(cs);
}

void h3lis331dl_init_i2c(h3lis331dl_config_t *config, hal_i2c_instance_t i2c)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = false,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0xC0,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0;
}

bool h3lis331dl_validate(const h3lis331dl_config_t *config)
{
    return gpio_utils_read_reg(&config->gpioConfig, WHO_AM_I) == WHO_AM_I_VALUE;
}

void h3lis331dl_set_power_mode(const h3lis331dl_config_t *config, h3lis331dl_power_mode_t power)
{
    gpio_utils_write_reg_field(&config->gpioConfig, CTRL_REG1, 3, 5, (uint8_t)power);
}

void h3lis331dl_set_odr(const h3lis331dl_config_t *config, h3lis331dl_odr_t odr)
{
    gpio_utils_write_reg_field(&config->gpioConfig, CTRL_REG1, 2, 3, (uint8_t)odr);
}

void h3lis331dl_set_hpfc(const h3lis331dl_config_t *config, h3lis331dl_hpfc_t hpcf)
{
    gpio_utils_write_reg_field(&config->gpioConfig, CTRL_REG2, 2, 0, (uint8_t)hpcf);
}

void h3lis331dl_set_range(h3lis331dl_config_t *config, h3lis331dl_range_t range)
{
    gpio_utils_write_reg_field(&config->gpioConfig, CTRL_REG4, 2, 4, (uint8_t)range);

    switch (range)
    {
    case H3LIS331DL_RANGE_100G:
        config->rangeFactor = 100.0f * EARTH_GRAVITY / H3LIS_RESOLUTION;
        break;
    case H3LIS331DL_RANGE_200G:
        config->rangeFactor = 200.0f * EARTH_GRAVITY / H3LIS_RESOLUTION;
        break;
    case H3LIS331DL_RANGE_400G:
        config->rangeFactor = 400.0f * EARTH_GRAVITY / H3LIS_RESOLUTION;
        break;
    default:
        return;
    }
}

void h3lis331dl_read(const h3lis331dl_config_t *config, vec3_t *accel)
{
    uint8_t buffer[6];

    gpio_utils_read_regs(&config->gpioConfig, OUT_X_L, buffer, 6);

    int16_t x = (int16_t)((buffer[1] << 8) | buffer[0]);
    int16_t y = (int16_t)((buffer[3] << 8) | buffer[2]);
    int16_t z = (int16_t)((buffer[5] << 8) | buffer[4]);

    accel->x = (float)x * config->rangeFactor;
    accel->y = (float)y * config->rangeFactor;
    accel->z = (float)z * config->rangeFactor;
}