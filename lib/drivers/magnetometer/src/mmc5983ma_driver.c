#include "lib/drivers/magnetometer/mmc5983ma_driver.h"
#include "hal/time_tracker.h"
#include <stdint.h>

#define X_OUT_0 0x00
#define X_OUT_1 0x01
#define Y_OUT_0 0x02
#define Y_OUT_1 0x03
#define Z_OUT_0 0x04
#define Z_OUT_1 0x05
#define XYZ_OUT_2 0x06
#define T_OUT 0x07
#define STATUS 0x08
#define INTERNAL_CONTROL_0 0x09
#define INTERNAL_CONTROL_1 0x0A
#define INTERNAL_CONTROL_2 0x0B
#define INTERNAL_CONTROL_3 0x0C
#define PRODUCT_ID 0x2F

#define I2C_ADDRESS 0x30

#define PRODUCT_ID_VALID 0x30

static void _mmc5983ma_soft_reset(const mmc5983ma_config_t *config);
static void _mmc5983ma_auto_set_reset(mmc5983ma_config_t *config);
static void _mmc5983ma_set(const mmc5983ma_config_t *config);
static void _mmc5983ma_reset(const mmc5983ma_config_t *config);

void mmc5983ma_init_spi(mmc5983ma_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = true,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    hal_spi_init_cs(cs);

    _mmc5983ma_auto_set_reset(config);
    _mmc5983ma_set(config);
    _mmc5983ma_reset(config);
}

void mmc5983ma_init_i2c(mmc5983ma_config_t *config, hal_i2c_instance_t i2c)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = false,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    _mmc5983ma_auto_set_reset(config);
    _mmc5983ma_set(config);
    _mmc5983ma_reset(config);
}

bool mmc5983ma_validate_id(const mmc5983ma_config_t *config)
{
    return gpio_utils_read_reg(&config->gpioConfig, PRODUCT_ID) == PRODUCT_ID_VALID;
}

void mmc5983ma_set_continuous_mode_odr(const mmc5983ma_config_t *config, mmc5983_odr_t odr)
{
    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_1, 0x00);

    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_2, 0x00);
    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_2, odr | (1 << 3));
}

void mmc5983ma_read(const mmc5983ma_config_t *config, vec3_t *mag)
{
    uint8_t buffer[7];
    gpio_utils_read_regs(&config->gpioConfig, X_OUT_0, buffer, 7);

    uint32_t rawX = (uint32_t)((buffer[0] << 10) | (buffer[1] << 2) | ((buffer[7] & 0xC0) >> 6));
    uint32_t rawY = (uint32_t)((buffer[2] << 10) | (buffer[3] << 2) | ((buffer[7] & 0x30) >> 4));
    uint32_t rawZ = (uint32_t)((buffer[4] << 10) | (buffer[5] << 2) | ((buffer[7] & 0x0C) >> 2));

    mag->x = ((float)rawX / (1 << 14) - 8) * 1000;
    mag->y = ((float)rawY / (1 << 14) - 8) * 1000;
    mag->z = ((float)rawZ / (1 << 14) - 8) * 1000;
}

void mmc5983ma_read_temp(const mmc5983ma_config_t *config, float *temp)
{
    uint8_t rawTemp = gpio_utils_read_reg(&config->gpioConfig, T_OUT);

    *temp = 0.8f * (float)rawTemp - 75.0f;
}

static void _mmc5983ma_soft_reset(const mmc5983ma_config_t *config)
{
    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_1, 1 << 7);

    hal_time_sleep_ms(15);
}

static void _mmc5983ma_auto_set_reset(mmc5983ma_config_t *config)
{
    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 1 << 5);
}

static void _mmc5983ma_set(const mmc5983ma_config_t *config)
{
    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 1 << 3);

    hal_time_sleep_ms(1);
}

static void _mmc5983ma_reset(const mmc5983ma_config_t *config)
{
    gpio_utils_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 1 << 4);

    hal_time_sleep_ms(1);
}