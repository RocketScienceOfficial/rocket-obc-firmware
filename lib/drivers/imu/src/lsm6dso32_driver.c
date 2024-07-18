#include "lib/drivers/imu/lsm6dso32_driver.h"
#include "lib/geo/physical_constants.h"
#include "lib/maths/math_constants.h"
#include <stdint.h>

#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define CTRL4_C 0x13
#define CTRL5_C 0x14
#define CTRL6_C 0x15
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18
#define CTRL10_C 0x19
#define STATUS_REG 0x1E
#define OUT_TEMP_L 0x20

#define WHO_AM_I_VALUE 0x6C
#define I2C_ADDRESS 0x6A
#define LSM_RESOLUTION 32768.0f

void lsm6dso32_init_spi(lsm6dso32_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs)
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
}

void lsm6dso32_init_i2c(lsm6dso32_config_t *config, hal_i2c_instance_t i2c)
{
    config->gpioConfig = (gpio_utils_communication_config_t){
        .useSPI = false,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
}

bool lsm6dso32_validate_id(const lsm6dso32_config_t *config)
{
    return gpio_utils_read_reg(&config->gpioConfig, WHO_AM_I) == WHO_AM_I_VALUE;
}

void lsm6dso32_set_odr(const lsm6dso32_config_t *config, lsm6dso32_odr_t accelODR, lsm6dso32_odr_t gyroODR)
{
    gpio_utils_write_reg_field(&config->gpioConfig, CTRL1_XL, 4, 4, (uint8_t)accelODR);
    gpio_utils_write_reg_field(&config->gpioConfig, CTRL2_G, 4, 4, (uint8_t)gyroODR);
}

void lsm6dso32_set_range(lsm6dso32_config_t *config, lsm6dso32_accel_range_t accelRange, lsm6dso32_gyro_range_t gyroRange)
{
    float acc = 0.0f;
    float gyr = 0.0f;

    if (accelRange == LSM6DSO32_RANGE_4G)
        acc = 4.0f;
    else if (accelRange == LSM6DSO32_RANGE_8G)
        acc = 8.0f;
    else if (accelRange == LSM6DSO32_RANGE_16G)
        acc = 16.0f;
    else if (accelRange == LSM6DSO32_RANGE_32G)
        acc = 32.0f;

    if (gyroRange == LSM6DSO32_RANGE_125DPS)
        gyr = 125.0f;
    else if (gyroRange == LSM6DSO32_RANGE_250DPS)
        gyr = 250.0f;
    else if (gyroRange == LSM6DSO32_RANGE_500DPS)
        gyr = 500.0f;
    else if (gyroRange == LSM6DSO32_RANGE_1000DPS)
        gyr = 1000.0f;
    else if (gyroRange == LSM6DSO32_RANGE_2000DPS)
        gyr = 2000.0f;

    config->accelRangeFactor = acc * EARTH_GRAVITY / LSM_RESOLUTION;
    config->gyroRangeFactor = DEG_2_RAD(gyr) / LSM_RESOLUTION;

    gpio_utils_write_reg_field(&config->gpioConfig, CTRL1_XL, 2, 2, (uint8_t)accelRange);

    if (gyroRange != LSM6DSO32_RANGE_125DPS)
    {
        gpio_utils_write_reg_field(&config->gpioConfig, CTRL2_G, 1, 1, 0x00);
        gpio_utils_write_reg_field(&config->gpioConfig, CTRL2_G, 2, 2, (uint8_t)gyroRange);
    }
    else
    {
        gpio_utils_write_reg_field(&config->gpioConfig, CTRL2_G, 1, 1, 0x01);
    }
}

void lsm6dso32_read(const lsm6dso32_config_t *config, vec3_t *pAcceleration, vec3_t *pGyro, float *pTemperature)
{
    uint8_t buffer[14];

    gpio_utils_read_regs(&config->gpioConfig, OUT_TEMP_L, buffer, 14);

    int16_t rawTemperature = (int16_t)(buffer[1] << 8) | buffer[0];
    int16_t rawAngularRateX = (int16_t)(buffer[3] << 8) | buffer[2];
    int16_t rawAngularRateY = (int16_t)(buffer[5] << 8) | buffer[4];
    int16_t rawAngularRateZ = (int16_t)(buffer[7] << 8) | buffer[6];
    int16_t rawAccelerationX = (int16_t)(buffer[9] << 8) | buffer[8];
    int16_t rawAccelerationY = (int16_t)(buffer[11] << 8) | buffer[10];
    int16_t rawAccelerationZ = (int16_t)(buffer[13] << 8) | buffer[12];

    if (pTemperature != NULL)
    {
        *pTemperature = rawTemperature / LSM_RESOLUTION;
    }

    pGyro->x = rawAngularRateX * config->gyroRangeFactor;
    pGyro->y = rawAngularRateY * config->gyroRangeFactor;
    pGyro->z = rawAngularRateZ * config->gyroRangeFactor;
    pAcceleration->x = rawAccelerationX * config->accelRangeFactor;
    pAcceleration->y = rawAccelerationY * config->accelRangeFactor;
    pAcceleration->z = rawAccelerationZ * config->accelRangeFactor;
}