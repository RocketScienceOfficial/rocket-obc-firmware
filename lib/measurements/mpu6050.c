#include "mpu6050.h"
#include "constants.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

static void mpu6050_reset()
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf, 2, false);

    uint8_t buf2 = {0x1C, 2 << 3};
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf2, 2, false);
}

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t buffer[6];

    uint8_t val = 0x3B;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x43;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x41;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 2, false);

    *temp = buffer[0] << 8 | buffer[1];
}

void mpu6050Init()
{
    myLogInfo("Initializing MPU6050...");

    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    mpu6050_reset();

    myLogInfo("Successfully initialized MPU6050!");
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

void mpu6050ReadRaw(mpu6050_data_t *data)
{
    myLogInfo("Reading MPU6050...");

    int16_t acceleration[3], gyro[3], temp;

    mpu6050_read_raw(acceleration, gyro, &temp);

    data->accel_x = convert_to_ms2(acceleration[0]);
    data->accel_y = convert_to_ms2(acceleration[1]);
    data->accel_z = convert_to_ms2(acceleration[2]);
    data->rot_x = convert_to_dps(gyro[0]);
    data->rot_y = convert_to_dps(gyro[1]);
    data->rot_z = convert_to_dps(gyro[2]);
    data->temperature = convert_temperature(temp);

    myLogInfo("Successfully read MPU6050!");
}

void mpu6050ConvertData(mpu6050_data_t *data)
{
    myLogInfo("Converting data from MPU6050...");

    myLogInfo("Successfully converted raw data from MPU6050!");
}