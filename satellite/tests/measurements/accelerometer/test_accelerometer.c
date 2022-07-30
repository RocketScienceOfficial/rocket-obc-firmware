#include "test_accelerometer.h"
#include "accelerometer.h"
#include "pinout.h"
#include "time_tracker.h"

static unsigned int s_TimerOffset = 0;

MY_TEST_INIT_FUNC(ACCELEROMETER_TEST_NAME)
{
    MY_ASSERT(accelerometerInit(MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL));

    MY_TEST_END();
}

MY_TEST_FUNC(ACCELEROMETER_TEST_NAME, 1)
{
    MY_ASSERT(accelerometerCheck());

    accelerometer_raw_data_t accelerometerRawData;
    accelerometer_data_t accelerometerData;
    accelerometerReadRaw(&accelerometerRawData);
    accelerometerConvertData(&accelerometerRawData, &accelerometerData);

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

MY_TEST_FUNC_DYNAMIC(ACCELEROMETER_TEST_NAME, 1)
{
    if (runEvery(1000, &s_TimerOffset))
    {
        MY_ASSERT(accelerometerCheck());

        accelerometer_raw_data_t accelerometerRawData;
        accelerometer_data_t accelerometerData;
        accelerometerReadRaw(&accelerometerRawData);
        accelerometerConvertData(&accelerometerRawData, &accelerometerData);

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