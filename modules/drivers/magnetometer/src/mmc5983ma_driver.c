#include "modules/drivers/magnetometer/mmc5983ma_driver.h"
#include "modules/drivers/hal/time_tracker.h"

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

#define PRODUCT_ID_VALID 0x0C
#define RESOLUTION 0.0625f
#define BASE_OFFSET 131072

static float HARD_IRON_CALIB[3] = {-21.11f, 11.87f, -47.88f};
static float SOFT_IRON_CALIB[3][3] = {
    {1.022f, 0.007f, 0.007f},
    {0.007f, 0.979f, -0.001f},
    {0.007f, -0.001f, 0.999f},
};

static void _mmc5983ma_set(mmc5983ma_config_t *config);
static void _mmc5983ma_reset(mmc5983ma_config_t *config);
static void _mmc5983ma_init_base(mmc5983ma_config_t *config);

void mmc5983ma_init_spi(mmc5983ma_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    hal_spi_init_cs(spi, cs);

    _mmc5983ma_init_base(config);
}

void mmc5983ma_init_i2c(mmc5983ma_config_t *config, hal_i2c_instance_t i2c)
{
    config->gpioConfig = (hal_gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
}

void mmc5983_validate_id(mmc5983ma_config_t *config, bool *valid)
{
    *valid = hal_gpio_read_reg(&config->gpioConfig, PRODUCT_ID) == PRODUCT_ID_VALID;
}

void mmc5983ma_set_bandwidth(mmc5983ma_config_t *config, mmc5983ma_bandwidth_t bandwidth)
{
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_1, (uint8_t)bandwidth);
}

void mmc5983ma_set_continuous_mode_odr(mmc5983ma_config_t *config, mmc5983_odr_t odr)
{
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_2, odr == 0 ? 0 : (0x08 | odr));
}

void mmc5983ma_reset(mmc5983ma_config_t *config)
{
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_1, 0x80);

    hal_time_sleep_ms(20);

    _mmc5983ma_set(config);
    _mmc5983ma_reset(config);

    mmc5983ma_set_continuous_mode_odr(config, 0);
}

void mmc5983ma_calculate_offset(mmc5983ma_config_t *config, vec3_t *offset)
{
    uint8_t buffer[6];
    uint16_t dataSet[3];
    uint16_t dataReset[3];

    mmc5983ma_set_continuous_mode_odr(config, 0);

    _mmc5983ma_set(config);
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 0x01);
    hal_time_sleep_ms(10);
    hal_gpio_read_regs(&config->gpioConfig, X_OUT_0, buffer, 6);
    dataSet[0] = (uint16_t)((buffer[0] << 8) | buffer[1]);
    dataSet[1] = (uint16_t)((buffer[2] << 8) | buffer[3]);
    dataSet[2] = (uint16_t)((buffer[4] << 8) | buffer[5]);

    _mmc5983ma_reset(config);
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 0x01);
    hal_time_sleep_ms(10);
    hal_gpio_read_regs(&config->gpioConfig, X_OUT_0, buffer, 6);
    dataReset[0] = (uint16_t)((buffer[0] << 8) | buffer[1]);
    dataReset[1] = (uint16_t)((buffer[2] << 8) | buffer[3]);
    dataReset[2] = (uint16_t)((buffer[4] << 8) | buffer[5]);

    offset->x = (float)(dataSet[0] + dataReset[0]) / 2.0f;
    offset->y = (float)(dataSet[1] + dataReset[1]) / 2.0f;
    offset->z = (float)(dataSet[2] + dataReset[2]) / 2.0f;
}

void mmc5983ma_read(mmc5983ma_config_t *config, vec3_t *mag)
{
    uint8_t buffer[7];

    hal_gpio_read_regs(&config->gpioConfig, X_OUT_0, buffer, 7);

    int x = (unsigned int)((buffer[0] << 10) | (buffer[1] << 2) | ((buffer[7] & 0xC0) >> 6)) - BASE_OFFSET;
    int y = (unsigned int)((buffer[2] << 10) | (buffer[3] << 2) | ((buffer[7] & 0x30) >> 4)) - BASE_OFFSET;
    int z = (unsigned int)((buffer[4] << 10) | (buffer[5] << 2) | ((buffer[7] & 0x0C) >> 2)) - BASE_OFFSET;

    float rawX = (float)x * RESOLUTION - HARD_IRON_CALIB[0];
    float rawY = (float)y * RESOLUTION - HARD_IRON_CALIB[1];
    float rawZ = (float)z * RESOLUTION - HARD_IRON_CALIB[2];

    mag->x = SOFT_IRON_CALIB[0][0] * rawX + SOFT_IRON_CALIB[0][1] * rawY + SOFT_IRON_CALIB[0][2] * rawZ;
    mag->y = SOFT_IRON_CALIB[1][0] * rawX + SOFT_IRON_CALIB[1][1] * rawY + SOFT_IRON_CALIB[1][2] * rawZ;
    mag->z = SOFT_IRON_CALIB[2][0] * rawX + SOFT_IRON_CALIB[2][1] * rawY + SOFT_IRON_CALIB[2][2] * rawZ;
}

void mmc5983ma_read_temp(mmc5983ma_config_t *config, float *temp)
{
    uint8_t rawTemp = hal_gpio_read_reg(&config->gpioConfig, T_OUT);

    *temp = 0.8f * (float)rawTemp - 75.0f;
}

static void _mmc5983ma_set(mmc5983ma_config_t *config)
{
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 0x08);

    hal_time_sleep_ms(1);
}

static void _mmc5983ma_reset(mmc5983ma_config_t *config)
{
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 0x10);

    hal_time_sleep_ms(1);
}

static void _mmc5983ma_init_base(mmc5983ma_config_t *config)
{
    hal_gpio_write_reg(&config->gpioConfig, INTERNAL_CONTROL_0, 0x20);

    mmc5983ma_reset(config);
}