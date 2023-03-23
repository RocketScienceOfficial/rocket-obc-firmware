#include "drivers/accelerometer/mpu6050_driver.h"
#include "maths/constants.h"

#define MPU6050_ADDR 0x68
#define AX_OFFSET 552
#define AY_OFFSET -241
#define AZ_OFFSET -3185
#define AX_SCALE 1.00457
#define AY_SCALE 0.99170
#define AZ_SCALE 0.98317
#define GX_SCALE 0.99764
#define GY_SCALE 1.0
#define GZ_SCALE 1.01037

static FLOAT getAccelLBS(INT32 level)
{
    return (FLOAT)(level < 0 || level > 3) ? (1) : (1.0 / (level == 0 ? 16384.0 : level == 1 ? 8192.0
                                                                              : level == 2   ? 4096.0
                                                                                             : 2048.0));
}

static FLOAT getGyroLBS(INT32 level)
{
    return (FLOAT)(level < 0 || level > 3) ? (1) : ((PI / 180.0) / (level == 0 ? 131.0 : level == 1 ? 65.5
                                                                                     : level == 2   ? 32.8
                                                                                                    : 16.4));
}

static VOID mpu6050Reset(MPU6050Config *config, INT32 lpFilter, INT32 accelSensLevel, INT32 gyroSensLevel)
{
    BYTE buf[] = {0x6B, 0x00};
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, buf, 2, FALSE);

    BYTE buf2[] = {0x1A, lpFilter <= 6 ? lpFilter : 0x00};
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, buf2, 2, FALSE);

    BYTE buf3[] = {0x1B, gyroSensLevel == 1 ? 0x08 : gyroSensLevel == 2 ? 0x10
                                                 : gyroSensLevel == 3   ? 0x18
                                                                        : 0x00};
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, buf3, 2, FALSE);

    BYTE buf4[] = {0x1C, accelSensLevel == 1 ? 0x08 : accelSensLevel == 2 ? 0x10
                                                  : accelSensLevel == 3   ? 0x18
                                                                          : 0x00};
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, buf4, 2, FALSE);
}

static VOID mpu6050ReadRawValues(MPU6050Config *config, INT16 accel[3], INT16 gyro[3], INT16 *temp)
{
    BYTE buffer[6];

    BYTE val = 0x3B;
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, &val, 1, TRUE);
    i2cReadBlocking(config->_i2c, MPU6050_ADDR, buffer, 6, FALSE);

    for (INT32 i = 0; i < 3; i++)
    {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x43;
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, &val, 1, TRUE);
    i2cReadBlocking(config->_i2c, MPU6050_ADDR, buffer, 6, FALSE);

    for (INT32 i = 0; i < 3; i++)
    {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    val = 0x41;
    i2cWriteBlocking(config->_i2c, MPU6050_ADDR, &val, 1, TRUE);
    i2cReadBlocking(config->_i2c, MPU6050_ADDR, buffer, 6, FALSE);

    *temp = buffer[0] << 8 | buffer[1];
}

FUNCRESULT mpu6050Init(MPU6050Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl, UINT8 lpFilter, UINT8 accelSensLevel, UINT8 gyroSensLevel)
{
    if (!config || !i2cCheckInstance(i2c) || !i2cCheckSDA(i2c, sda) || !i2cCheckSCL(i2c, scl) || lpFilter < 0 || lpFilter > 6 || accelSensLevel < 0 || accelSensLevel > 3 || gyroSensLevel < 0 || gyroSensLevel > 3)
    {
        return ERR_INVALIDARG;
    }

    config->_i2c = i2c;

    if (FUNCFAILED(i2cInitPins(config->_i2c, sda, scl)))
    {
        return ERR_FAIL;
    }

    BOOL result;

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

FUNCRESULT mpu6050Check(MPU6050Config *config, BOOL *result)
{
    if (!config || !result)
    {
        return ERR_INVALIDARG;
    }

    *result = i2cCheckDevice(config->_i2c, MPU6050_ADDR);

    return SUC_OK;
}

static FLOAT convertAcceleration(MPU6050Config *config, INT16 raw, INT16 offset, FLOAT scale)
{
    FLOAT calcScale = scale * config->_accelLBS;
    FLOAT calcOffset = scale * ((FLOAT)offset) / config->_accelLBS;

    return ((FLOAT)raw) * calcScale - calcOffset;
}

static FLOAT convertGyro(MPU6050Config *config, INT16 raw, FLOAT scale)
{
    FLOAT calcScale = scale * config->_gryoLBS;

    return ((FLOAT)raw) * calcScale;
}

static FLOAT convertTemperature(INT16 raw)
{
    return (FLOAT)raw / 340.0 + 36.53;
}

FUNCRESULT mpu6050Read(MPU6050Config *config, MPU6050Data *data)
{
    if (!config || !data)
    {
        return ERR_INVALIDARG;
    }

    INT16 acceleration[3], gyro[3], temp;

    mpu6050ReadRawValues(config, acceleration, gyro, &temp);

    data->accel_x = convertAcceleration(config, acceleration[0], AX_OFFSET, AX_SCALE);
    data->accel_y = convertAcceleration(config, acceleration[1], AY_OFFSET, AY_SCALE);
    data->accel_z = convertAcceleration(config, acceleration[2], AZ_OFFSET, AZ_SCALE);
    data->rot_x = convertGyro(config, gyro[0], GX_SCALE);
    data->rot_y = convertGyro(config, gyro[1], GY_SCALE);
    data->rot_z = convertGyro(config, gyro[2], GZ_SCALE);
    data->temperature = convertTemperature(temp);

    return SUC_OK;
}