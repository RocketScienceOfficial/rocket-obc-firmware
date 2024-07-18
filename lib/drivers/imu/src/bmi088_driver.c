#include "lib/drivers/imu/bmi088_driver.h"
#include "lib/geo/physical_constants.h"
#include "lib/maths/math_constants.h"
#include "hal/time_tracker.h"
#include <math.h>
#include <string.h>

#define ACC_CHIP_ID 0x00
#define ACC_ERR_REG 0x02
#define ACC_STATUS 0x03
#define ACC_DATA 0x12
#define ACC_SENSORTIME 0x18
#define ACC_INT_STAT_1 0x1D
#define ACC_TEMP 0x22
#define ACC_FIFO_LENGTH 0x24
#define ACC_FIFO_DATA 0x26
#define ACC_CONF 0x40
#define ACC_OSR_NORMAL 0x0A
#define ACC_OSR_2 0x09
#define ACC_OSR_4 0x08
#define ACC_RANGE 0x41
#define ACC_FIFO_DOWNS 0x45
#define ACC_FIFO_WTM 0x46
#define ACC_FIFO_CONFIG 0x48
#define ACC_INT1_IO_CTRL 0x53
#define ACC_INT2_IO_CTRL 0x54
#define ACC_INT_MAP 0x58
#define ACC_SELF_TEST 0x6D
#define ACC_SELF_TEST_OFF_CMD 0x00
#define ACC_SELF_TEST_POS_SIG_CMD 0x0D
#define ACC_SELF_TEST_NEG_SIG_CMD 0x09
#define ACC_PWR_CONF 0x7C
#define ACC_PWR_CONF_SUSPEND_CMD 0x03
#define ACC_PWR_CONF_ACTIVE_CMD 0x00
#define ACC_PWR_CTRL 0x7D
#define ACC_PWR_CTRL_OFF_CMD 0x00
#define ACC_PWR_CTRL_ON_CMD 0x04
#define ACC_SOFTRESET 0x7F
#define ACC_SOFTRESET_CMD 0xB6

#define GYRO_CHIP_ID 0x00
#define GYRO_RATE 0x02
#define GYRO_INT_STAT_1 0x0A
#define GYRO_FIFO_STATUS 0x0E
#define GYRO_RANGE 0x0F
#define GYRO_BANDWIDTH 0x10
#define GYRO_LPM1 0x11
#define GYRO_LPM1_NORMAL_CMD 0x00
#define GYRO_LPM1_SUSPEND_CMD 0x80
#define GYRO_LPM1_DEEP_SUSPEND_CMD 0x20
#define GYRO_SOFTRESET 0x14
#define GYRO_SOFTRESET_CMD 0xB6
#define GYRO_INT_CTRL 0x15
#define GYRO_INT3_INT4_IO_CONF 0x16
#define GYRO_INT3_INT4_IO_MAP 0x18
#define GYRO_WM_EN 0x1E
#define FIFO_EXT_INT_S 0x34
#define GYRO_SELF_TEST 0x3C
#define GYRO_FIFO_CONFIG 0x3D
#define GYRO_FIFO_DATA 0x3E

#define ACC_GND_I2C_ADDRESS 0x18
#define ACC_VDD_I2C_ADDRESS 0x19
#define GYRO_GND_I2C_ADDRESS 0x68
#define GYRO_VDD_I2C_ADDRESS 0x69

#define BMI_RESOLUTION 32768.0f

static void _bmi088_accel_soft_reset(const bmi088_accel_config_t *config);
static void _bmi088_accel_set_mode(const bmi088_accel_config_t *config, bool active);
static void _bmi088_accel_set_power(const bmi088_accel_config_t *config, bool on);
static void _bmi088_accel_init_base(bmi088_accel_config_t *config);
static uint8_t _bmi088_accel_read_reg(const bmi088_accel_config_t *config, uint8_t address);
static void _bmi088_accel_read_regs(const bmi088_accel_config_t *config, uint8_t address, uint8_t *buffer, size_t count);
static void _bmi088_accel_write_reg(const bmi088_accel_config_t *config, uint8_t address, uint8_t data);
static void _bmi088_gyro_soft_reset(const bmi088_gyro_config_t *config);

void bmi088_accel_init_spi(bmi088_accel_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = true,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0.0f;

    hal_spi_init_cs(cs);

    _bmi088_accel_init_base(config);
}

void bmi088_accel_init_i2c(bmi088_accel_config_t *config, hal_i2c_instance_t i2c, bool sdo1Grounded)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = false,
        .i2c = i2c,
        .i2cAddress = sdo1Grounded ? ACC_GND_I2C_ADDRESS : ACC_VDD_I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0.0f;

    _bmi088_accel_init_base(config);
}

void bmi088_accel_set_conf(const bmi088_accel_config_t *config, bmi088_accel_odr_t odr, bmi088_accel_osr_t osr)
{
    _bmi088_accel_write_reg(config, ACC_CONF, odr | (osr << 4));

    hal_time_sleep_ms(5);
}

void bmi088_accel_set_range(bmi088_accel_config_t *config, bmi088_accel_range_t range)
{
    config->rangeFactor = powf(2, range + 1) * 1.5f * EARTH_GRAVITY / BMI_RESOLUTION;

    _bmi088_accel_write_reg(config, ACC_RANGE, range);

    hal_time_sleep_ms(5);
}

void bmi088_accel_read(const bmi088_accel_config_t *config, vec3_t *accel)
{
    uint8_t buff[6];

    _bmi088_accel_read_regs(config, ACC_DATA, buff, 6);

    int16_t accelX = (int16_t)((buff[1] << 8) | buff[0]);
    int16_t accelY = (int16_t)((buff[3] << 8) | buff[2]);
    int16_t accelZ = (int16_t)((buff[5] << 8) | buff[4]);

    accel->x = accelX * config->rangeFactor;
    accel->y = accelY * config->rangeFactor;
    accel->z = accelZ * config->rangeFactor;
}

void bmi088_gyro_init_spi(bmi088_gyro_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = true,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0.0f;

    hal_spi_init_cs(cs);
}

void bmi088_gyro_init_i2c(bmi088_gyro_config_t *config, hal_i2c_instance_t i2c, bool sdo1Grounded)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = false,
        .i2c = i2c,
        .i2cAddress = sdo1Grounded ? GYRO_GND_I2C_ADDRESS : GYRO_VDD_I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0.0f;
}

void bmi088_gyro_set_bandwidth(const bmi088_gyro_config_t *config, bmi088_gyro_bandwidth_t bw)
{
    gpio_utils_write_reg(&config->gpioConfig, GYRO_BANDWIDTH, (uint8_t)bw | 0x80);

    hal_time_sleep_ms(5);
}

void bmi088_gyro_set_range(bmi088_gyro_config_t *config, bmi088_gyro_range_t range)
{
    float val = 2000.0f / powf(2, range);

    config->rangeFactor = DEG_2_RAD(val) / BMI_RESOLUTION;

    gpio_utils_write_reg(&config->gpioConfig, GYRO_RANGE, (uint8_t)range);

    hal_time_sleep_ms(5);
}

void bmi088_gyro_read(const bmi088_gyro_config_t *config, vec3_t *gyro)
{
    uint8_t buff[6];

    gpio_utils_read_regs(&config->gpioConfig, GYRO_RATE, buff, 6);

    int16_t gyroX = (int16_t)((buff[1] << 8) | buff[0]);
    int16_t gyroY = (int16_t)((buff[3] << 8) | buff[2]);
    int16_t gyroZ = (int16_t)((buff[5] << 8) | buff[4]);

    gyro->x = gyroX * config->rangeFactor;
    gyro->y = gyroY * config->rangeFactor;
    gyro->z = gyroZ * config->rangeFactor;
}

static void _bmi088_accel_soft_reset(const bmi088_accel_config_t *config)
{
    _bmi088_accel_write_reg(config, ACC_SOFTRESET, ACC_SOFTRESET_CMD);

    hal_time_sleep_ms(50);
}

static void _bmi088_accel_set_mode(const bmi088_accel_config_t *config, bool active)
{
    uint8_t data = active ? ACC_PWR_CONF_ACTIVE_CMD : ACC_PWR_CONF_SUSPEND_CMD;

    _bmi088_accel_write_reg(config, ACC_PWR_CONF, data);

    hal_time_sleep_ms(5);
}

static void _bmi088_accel_set_power(const bmi088_accel_config_t *config, bool on)
{
    uint8_t data = on ? ACC_PWR_CTRL_ON_CMD : ACC_PWR_CTRL_OFF_CMD;

    _bmi088_accel_write_reg(config, ACC_PWR_CTRL, data);

    hal_time_sleep_ms(5);
}

static void _bmi088_accel_init_base(bmi088_accel_config_t *config)
{
    _bmi088_accel_set_power(config, true);
    _bmi088_accel_set_mode(config, true);
}

static uint8_t _bmi088_accel_read_reg(const bmi088_accel_config_t *config, uint8_t address)
{
    uint8_t data[2];

    gpio_utils_read_regs(&config->gpioConfig, address, data, 2);

    return data[1];
}

static void _bmi088_accel_read_regs(const bmi088_accel_config_t *config, uint8_t address, uint8_t *buffer, size_t count)
{
    uint8_t tmp_buffer[16];

    gpio_utils_read_regs(&config->gpioConfig, address, tmp_buffer, count + 1);

    memcpy(buffer, tmp_buffer + 1, count);
}

static void _bmi088_accel_write_reg(const bmi088_accel_config_t *config, uint8_t address, uint8_t data)
{
    gpio_utils_write_reg(&config->gpioConfig, address, data);
}

static void _bmi088_gyro_soft_reset(const bmi088_gyro_config_t *config)
{
    gpio_utils_write_reg(&config->gpioConfig, GYRO_SOFTRESET, GYRO_SOFTRESET_CMD);

    hal_time_sleep_ms(50);
}