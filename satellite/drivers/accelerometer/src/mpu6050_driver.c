#include "drivers/accelerometer/mpu6050_driver.h"
#include "drivers/hwutils/pinout_utils.h"
#include "utils/constants.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>

#define MPU6050_ADDR 0x68
#define ACCEL_LBS_0 16384.0
#define N_AXIS 3
#define DEFAULT_SCALE = 1;
#define AX_OFFSET 552
#define AY_OFFSET -241
#define AZ_OFFSET -3185
#define AX_SCALE 1.00457
#define AY_SCALE 0.99170
#define AZ_SCALE 0.98317
#define GX_SCALE 0.99764
#define GY_SCALE 1.0
#define GZ_SCALE 1.01037
#define GYRO_BAND 35
#define BIAS_COUNT 5000

static i2c_inst_t *getI2C(MPU6050Config *config)
{
    return (config->_i2c == 0 ? i2c0 : i2c1);
}

static float getAccelLBS(int level)
{
    return (float)(level < 0 || level > 3) ? (1) : (1.0 / (level == 0 ? 16384.0 : level == 1 ? 8192.0
                                                                              : level == 2   ? 4096.0
                                                                                             : 2048.0));
}

static float getGyroLBS(int level)
{
    return (float)(level < 0 || level > 3) ? (1) : ((PI / 180.0) / (level == 0 ? 131.0 : level == 1 ? 65.5
                                                                                     : level == 2   ? 32.8
                                                                                                    : 16.4));
}

static void mpu6050Reset(MPU6050Config *config, int lpFilter, int accelSensLevel, int gyroSensLevel)
{
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, buf, 2, false);

    uint8_t buf2[] = {0x1A, lpFilter <= 6 ? lpFilter : 0x00};
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, buf2, 2, false);

    uint8_t buf3[] = {0x1B, gyroSensLevel == 1 ? 0x08 : gyroSensLevel == 2 ? 0x10
                                                    : gyroSensLevel == 3   ? 0x18
                                                                           : 0x00};
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, buf3, 2, false);

    uint8_t buf4[] = {0x1C, accelSensLevel == 1 ? 0x08 : accelSensLevel == 2 ? 0x10
                                                     : accelSensLevel == 3   ? 0x18
                                                                             : 0x00};
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, buf4, 2, false);
}

static void mpu6050ReadRawValues(MPU6050Config *config, int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t buffer[6];

    uint8_t val = 0x3B;
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(getI2C(config), MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x43;
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(getI2C(config), MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x41;
    i2c_write_blocking(getI2C(config), MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(getI2C(config), MPU6050_ADDR, buffer, 2, false);

    *temp = buffer[0] << 8 | buffer[1];
}

FUNCRESULT mpu6050Init(MPU6050Config *config, int i2c, int sda, int scl, int lpFilter, int accelSensLevel, int gyroSensLevel)
{
    if (!config || !pinoutCheckI2C(i2c) || !pinoutCheckI2C_SDA(i2c, sda) || !pinoutCheckI2C_SCL(i2c, scl) || lpFilter < 0 || lpFilter > 6 || accelSensLevel < 0 || accelSensLevel > 3 || gyroSensLevel < 0 || gyroSensLevel > 3)
    {
        return ERR_INVALIDARG;
    }

    memset(config, 0, sizeof(MPU6050Config));

    config->_i2c = i2c;

    i2c_init(getI2C(config), 400 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    bool result;

    if (FUNCFAILED(mpu6050Check(config, &result)))
    {
        return ERR_UNEXPECTED;
    }

    if (!result)
    {
        return ERR_UNINITIALIZED;
    }

    config->_accelLBS = getAccelLBS(accelSensLevel);
    config->_gryoLBS = getGyroLBS(gyroSensLevel);

    mpu6050Reset(config, lpFilter, accelSensLevel, gyroSensLevel);

    return SUC_OK;
}

FUNCRESULT mpu6050Check(MPU6050Config *config, bool *result)
{
    if (!config || !result)
    {
        return ERR_INVALIDARG;
    }

    uint8_t data;
    int ret = i2c_read_timeout_us(getI2C(config), MPU6050_ADDR, &data, 1, false, 1E6);

    *result = ret < 0 ? 0 : 1;

    return SUC_OK;
}

FUNCRESULT mpu6050ReadRaw(MPU6050Config *config, MPU6050RawData *data)
{
    if (!config || !data)
    {
        return ERR_INVALIDARG;
    }

    int16_t acceleration[3], gyro[3], temp;

    mpu6050ReadRawValues(config, acceleration, gyro, &temp);

    data->accel_x = acceleration[0];
    data->accel_y = acceleration[1];
    data->accel_z = acceleration[2];
    data->rot_x = gyro[0];
    data->rot_y = gyro[1];
    data->rot_z = gyro[2];
    data->temperature = temp;

    return SUC_OK;
}

static float convertAcceleration(MPU6050Config *config, int16_t raw, int16_t offset, float scale)
{
    float calcScale = scale * config->_accelLBS;
    float calcOffset = scale * ((float)offset) / ACCEL_LBS_0;

    return ((float)raw) * calcScale - calcOffset;
}

static float convertGyro(MPU6050Config *config, int16_t raw, float scale)
{
    float calcScale = scale * config->_gryoLBS;

    return ((float)raw) * calcScale;
}

static float convertTemperature(int16_t raw)
{
    return (float)raw / 340.0 + 36.53;
}

FUNCRESULT mpu6050ConvertData(MPU6050Config *config, MPU6050RawData *inData, MPU6050Data *outData)
{
    if (!config || !inData || !outData)
    {
        return ERR_INVALIDARG;
    }

    outData->accel_x = convertAcceleration(config, inData->accel_x, AX_OFFSET, AX_SCALE);
    outData->accel_y = convertAcceleration(config, inData->accel_y, AY_OFFSET, AY_SCALE);
    outData->accel_z = convertAcceleration(config, inData->accel_z, AZ_OFFSET, AZ_SCALE);
    outData->rot_x = convertGyro(config, inData->rot_x, GX_SCALE);
    outData->rot_y = convertGyro(config, inData->rot_y, GY_SCALE);
    outData->rot_z = convertGyro(config, inData->rot_z, GZ_SCALE);
    outData->temperature = convertTemperature(inData->temperature);

    return SUC_OK;
}