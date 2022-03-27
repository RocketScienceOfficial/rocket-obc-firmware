#include "pinout_config.h"
#include "mpu6050_controller.h"
#include "constants.h"

#define PICO_DEFAULT_I2C MPU6050_I2C
#define PICO_DEFAULT_I2C_SDA_PIN MPU6050_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SCL_PIN MPU6050_I2C_SCL_PIN

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define ADDR 0x68

static void mpu6050_reset()
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    uint8_t buf2 = {0x1C, 2 << 3};
    i2c_write_blocking(i2c_default, ADDR, buf2, 2, false);
}

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t buffer[6];

    uint8_t val = 0x3B;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x43;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x41;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 2, false);

    *temp = buffer[0] << 8 | buffer[1];
}

void mpu6050_init()
{
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    mpu6050_reset();
}

double convert_to_ms2(int16_t raw)
{
    return raw * ACCEL_FACTOR * GRAVITY;
}

double convert_to_dps(int16_t raw)
{
    return raw * GYRO_FACTOR;
}

double convert_temperature(int16_t raw)
{
    return (raw / 340.0) + 36.53;
}

void mpu6050_get_values(double *accel_x, double *accel_y, double *accel_z, double *rot_x, double *rot_y, double *rot_z, double *temperature)
{
    int16_t acceleration[3], gyro[3], temp;

    mpu6050_read_raw(acceleration, gyro, &temp);

    *accel_x = convert_to_ms2(acceleration[0]);
    *accel_y = convert_to_ms2(acceleration[1]);
    *accel_z = convert_to_ms2(acceleration[2]);
    *rot_x = convert_to_dps(gyro[0]);
    *rot_y = convert_to_dps(gyro[1]);
    *rot_z = convert_to_dps(gyro[2]);
    *temperature = convert_temperature(temp);
}