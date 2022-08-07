#include "test_accelerometer.h"
#include "accelerometer.h"
#include "pinout.h"
#include "time_tracker.h"

static Timer s_TimerOffset = 0;
static AccelerometerConfig s_Config;

MY_TEST_INIT_FUNC(ACCELEROMETER_TEST_NAME)
{
    MY_ASSERT(FUNCSUCCESS(accelerometerInit(&s_Config, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL)));

    MY_TEST_END();
}

MY_TEST_FUNC(ACCELEROMETER_TEST_NAME, 1)
{
    bool checkResult;
    MY_ASSERT(FUNCSUCCESS(accelerometerCheck(&s_Config, &checkResult)));
    MY_ASSERT(checkResult);

    AccelerometerRawData accelerometerRawData;
    AccelerometerData accelerometerData;
    MY_ASSERT(FUNCSUCCESS(accelerometerReadRaw(&s_Config, &accelerometerRawData)));
    MY_ASSERT(FUNCSUCCESS(accelerometerConvertData(&s_Config, &accelerometerRawData, &accelerometerData)));

    float accel_x = accelerometerData.accel_x;
    float accel_y = accelerometerData.accel_y;
    float accel_z = accelerometerData.accel_z;
    float rot_x = accelerometerData.rot_x;
    float rot_y = accelerometerData.rot_y;
    float rot_z = accelerometerData.rot_z;

    MY_ASSERT(accel_x != 0);
    MY_ASSERT(accel_y != 0);
    MY_ASSERT(accel_z != 0);
    MY_ASSERT(rot_x != 0);
    MY_ASSERT(rot_y != 0);
    MY_ASSERT(rot_z != 0);

    MY_TEST_END();
}

MY_TEST_FUNC(ACCELEROMETER_TEST_NAME, 2)
{
    AccelerometerConfig config;

    MY_ASSERT(FUNCFAILED(accelerometerInit(NULL, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL)));
    MY_ASSERT(FUNCFAILED(accelerometerInit(&config, 1, 4, 5, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL)));
    MY_ASSERT(FUNCFAILED(accelerometerInit(&config, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, 7, 4, 2)));

    MY_TEST_END();
}

MY_TEST_FUNC(ACCELEROMETER_TEST_NAME, 3)
{
    MY_ASSERT(FUNCFAILED(accelerometerCheck(NULL, NULL)));

    MY_TEST_END();
}

MY_TEST_FUNC(ACCELEROMETER_TEST_NAME, 4)
{
    MY_ASSERT(FUNCFAILED(accelerometerReadRaw(NULL, NULL)));

    MY_TEST_END();
}

MY_TEST_FUNC(ACCELEROMETER_TEST_NAME, 5)
{
    MY_ASSERT(FUNCFAILED(accelerometerConvertData(NULL, NULL, NULL)));

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(ACCELEROMETER_TEST_NAME, 1)
{
    if (runEvery(1000, &s_TimerOffset))
    {
        bool checkResult;
        MY_ASSERT(FUNCSUCCESS(accelerometerCheck(&s_Config, &checkResult)));
        MY_ASSERT(checkResult);

        AccelerometerRawData accelerometerRawData;
        AccelerometerData accelerometerData;
        MY_ASSERT(FUNCSUCCESS(accelerometerReadRaw(&s_Config, &accelerometerRawData)));
        MY_ASSERT(FUNCSUCCESS(accelerometerConvertData(&s_Config, &accelerometerRawData, &accelerometerData)));

        float accel_x = accelerometerData.accel_x;
        float accel_y = accelerometerData.accel_y;
        float accel_z = accelerometerData.accel_z;
        float rot_x = accelerometerData.rot_x;
        float rot_y = accelerometerData.rot_y;
        float rot_z = accelerometerData.rot_z;

        MY_ASSERT(accel_x != 0);
        MY_ASSERT(accel_y != 0);
        MY_ASSERT(accel_z != 0);
        MY_ASSERT(rot_x != 0);
        MY_ASSERT(rot_y != 0);
        MY_ASSERT(rot_z != 0);
    }

    MY_TEST_END();
}