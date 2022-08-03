#include "accelerometer.h"
#include "constants.h"
#include "logger.h"
#include "recorder.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

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

static int s_i2c;
static float s_AccelLBS;
static float s_GryoLBS;

static i2c_inst_t *getI2C()
{
    return (s_i2c == 0 ? i2c0 : i2c1);
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

static void mpu6050Reset(int lpFilter, int accelSensLevel, int gyroSensLevel)
{
    FUNCTION_PROFILE_BEGIN();

    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(getI2C(), MPU6050_ADDR, buf, 2, false);

    uint8_t buf2[] = {0x1A, lpFilter <= 6 ? lpFilter : 0x00};
    i2c_write_blocking(getI2C(), MPU6050_ADDR, buf2, 2, false);

    uint8_t buf3[] = {0x1B, gyroSensLevel == 1 ? 0x08 : gyroSensLevel == 2 ? 0x10
                                                    : gyroSensLevel == 3   ? 0x18
                                                                           : 0x00};
    i2c_write_blocking(getI2C(), MPU6050_ADDR, buf3, 2, false);

    uint8_t buf4[] = {0x1C, accelSensLevel == 1 ? 0x08 : accelSensLevel == 2 ? 0x10
                                                     : accelSensLevel == 3   ? 0x18
                                                                             : 0x00};
    i2c_write_blocking(getI2C(), MPU6050_ADDR, buf4, 2, false);

    FUNCTION_PROFILE_END();
}

static void mpu6050ReadRawValues(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    FUNCTION_PROFILE_BEGIN();

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

    FUNCTION_PROFILE_END();
}

int accelerometerInit(int i2c, int sda, int scl, int lpFilter, int accelSensLevel, int gyroSensLevel)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Initializing MPU6050...");

    s_i2c = i2c;

    i2c_init(getI2C(), 400 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    if (!accelerometerCheck())
    {
        MY_LOG_CORE_ERROR("MPU6050 not found!");

        FUNCTION_PROFILE_END();

        return 0;
    }

    s_AccelLBS = getAccelLBS(accelSensLevel);
    s_GryoLBS = getGyroLBS(gyroSensLevel);

    MY_LOG_CORE_INFO("AccelLBS: %f, GyroLBS: %f", s_AccelLBS, s_GryoLBS);

    mpu6050Reset(lpFilter, accelSensLevel, gyroSensLevel);

    MY_LOG_CORE_INFO("Successfully initialized MPU6050!");

    FUNCTION_PROFILE_END();

    return 1;
}

int accelerometerCheck()
{
    FUNCTION_PROFILE_BEGIN();

    uint8_t data;
    int ret = i2c_read_blocking(getI2C(), MPU6050_ADDR, &data, 1, false);

    FUNCTION_PROFILE_END();

    return ret < 0 ? 0 : 1;
}

void accelerometerReadRaw(accelerometer_raw_data_t *data)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Reading MPU6050...");

    int16_t acceleration[3], gyro[3], temp;

    mpu6050ReadRawValues(acceleration, gyro, &temp);

    data->accel_x = acceleration[0];
    data->accel_y = acceleration[1];
    data->accel_z = acceleration[2];
    data->rot_x = gyro[0];
    data->rot_y = gyro[1];
    data->rot_z = gyro[2];
    data->temperature = temp;

    MY_LOG_CORE_INFO("Successfully read MPU6050!");

    FUNCTION_PROFILE_END();
}

static float convertAcceleration(int16_t raw, int16_t offset, float scale)
{
    float calcScale = scale * s_AccelLBS;
    float calcOffset = scale * ((float)offset) / ACCEL_LBS_0;

    return ((float)raw) * calcScale - calcOffset;
}

static float convertGyro(int16_t raw, float scale)
{
    float calcScale = scale * s_GryoLBS;

    return ((float)raw) * calcScale;
}

static float convertTemperature(int16_t raw)
{
    return (float)raw / 340.0 + 36.53;
}

void accelerometerConvertData(accelerometer_raw_data_t *inData, accelerometer_data_t *outData)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Converting data from MPU6050...");

    outData->accel_x = convertAcceleration(inData->accel_x, AX_OFFSET, AX_SCALE);
    outData->accel_y = convertAcceleration(inData->accel_y, AY_OFFSET, AY_SCALE);
    outData->accel_z = convertAcceleration(inData->accel_z, AZ_OFFSET, AZ_SCALE);
    outData->rot_x = convertGyro(inData->rot_x, GX_SCALE);
    outData->rot_y = convertGyro(inData->rot_y, GY_SCALE);
    outData->rot_z = convertGyro(inData->rot_z, GZ_SCALE);
    outData->temperature = convertTemperature(inData->temperature);

    MY_LOG_CORE_INFO("Successfully converted raw data from MPU6050!");

    FUNCTION_PROFILE_END();
}