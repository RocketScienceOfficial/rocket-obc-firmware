#include "mpu6050.h"
#include "constants.h"
#include "logger.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define MPU6050_ADDR 0x68

static int s_i2c;

static i2c_inst_t *getI2C()
{
    return (s_i2c == 0 ? i2c0 : i2c1);
}

static void mpu6050_reset()
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(getI2C(), MPU6050_ADDR, buf, 2, false);

    uint8_t buf2 = {0x1C, 2 << 3};
    i2c_write_blocking(getI2C(), MPU6050_ADDR, buf2, 2, false);
}

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t buffer[6];

    uint8_t val = 0x3B;
    i2c_write_blocking(getI2C(), MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(getI2C(), MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x43;
    i2c_write_blocking(getI2C(), MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(getI2C(), MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x41;
    i2c_write_blocking(getI2C(), MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(getI2C(), MPU6050_ADDR, buffer, 2, false);

    *temp = buffer[0] << 8 | buffer[1];
}

void mpu6050Init(int i2c, int sda, int scl)
{
    MY_LOG_CORE_INFO("Initializing MPU6050...");

    s_i2c = i2c;

    i2c_init(getI2C(), 400 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    mpu6050_reset();

    MY_LOG_CORE_INFO("Successfully initialized MPU6050!");
}

double convert_to_ms2(int16_t raw)
{
    return raw * MPU6050_ACCEL_FACTOR * GRAVITY;
}

double convert_to_dps(int16_t raw)
{
    return raw * MPU6050_GYRO_FACTOR;
}

double convert_temperature(int16_t raw)
{
    return (raw / 340.0) + 36.53;
}

void mpu6050ReadRaw(mpu6050_data_t *data)
{
    MY_LOG_CORE_INFO("Reading MPU6050...");

    int16_t acceleration[3], gyro[3], temp;

    mpu6050_read_raw(acceleration, gyro, &temp);

    data->accel_x = acceleration[0];
    data->accel_y = acceleration[1];
    data->accel_z = acceleration[2];
    data->rot_x = gyro[0];
    data->rot_y = gyro[1];
    data->rot_z = gyro[2];
    data->temperature = temp;

    MY_LOG_CORE_INFO("Successfully read MPU6050!");
}

void mpu6050ConvertData(mpu6050_data_t *data)
{
    MY_LOG_CORE_INFO("Converting data from MPU6050...");

    data->accel_x = convert_to_ms2(data->accel_x);
    data->accel_y = convert_to_ms2(data->accel_y);
    data->accel_z = convert_to_ms2(data->accel_z);
    data->rot_x = convert_to_dps(data->rot_x);
    data->rot_y = convert_to_dps(data->rot_y);
    data->rot_z = convert_to_dps(data->rot_z);
    data->temperature = convert_temperature(data->temperature);

    MY_LOG_CORE_INFO("Successfully converted raw data from MPU6050!");
}

void mpu6050FilterData(kalman_filter_data_t *kalmanData, mpu6050_data_t *data)
{
    MY_LOG_CORE_INFO("Filtering data from MPU6050...");

    data->accel_x = kalman(kalmanData, data->accel_x);
    data->accel_y = kalman(kalmanData, data->accel_y);
    data->accel_z = kalman(kalmanData, data->accel_z);
    data->rot_x = kalman(kalmanData, data->rot_x);
    data->rot_y = kalman(kalmanData, data->rot_y);
    data->rot_z = kalman(kalmanData, data->rot_z);

    MY_LOG_CORE_INFO("Successfully filtered data from MPU6050!");
}