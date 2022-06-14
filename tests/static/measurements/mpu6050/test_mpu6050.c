#include "test_mpu6050.h"
#include "mpu6050.h"
#include "pinout.h"

MY_TEST_INIT_FUNC(MPU6050_TEST_NAME)
{
    MY_ASSERT(mpu6050Init(MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN));

    MY_TEST_END();
}

MY_TEST_FUNC(MPU6050_TEST_NAME, 1)
{
    MY_ASSERT(mpu6050Check());

    mpu6050_data_t mpu6050Data;
    mpu6050ReadRaw(&mpu6050Data);
    mpu6050ConvertData(&mpu6050Data);

    double accel_x = mpu6050Data.accel_x;
    double accel_y = mpu6050Data.accel_y;
    double accel_z = mpu6050Data.accel_z;
    double rot_x = mpu6050Data.rot_x;
    double rot_y = mpu6050Data.rot_y;
    double rot_z = mpu6050Data.rot_z;

    MY_ASSERT(accel_x != 0);
    MY_ASSERT(accel_y != 0);
    MY_ASSERT(accel_z != 0);
    MY_ASSERT(rot_x != 0);
    MY_ASSERT(rot_y != 0);
    MY_ASSERT(rot_z != 0);

    MY_TEST_END();
}