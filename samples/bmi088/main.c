#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"
#include <stdio.h>

#define I2C 1
#define SDA 14
#define SCL 15

int main()
{
    boardInit(5000);

    i2cInitAll(I2C, 400000);

    MadgiwckFilterData madgwickFilterData;
    BMI088AccelConfig accelConfig;
    BMI088GyroConfig gyroConfig;

    madgwickInit(&madgwickFilterData, 0.2f, 0.01f);

    bmi088AccelInitI2C(&accelConfig, I2C, SDA, SCL, TRUE);
    bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_6G);

    bmi088GyroInitI2C(&gyroConfig, I2C, SDA, SCL, TRUE);
    bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_250DPS);

    while (TRUE)
    {
        vec3 accel;
        vec3 gyro;
        vec3 eulerData;

        bmi088AccelRead(&accelConfig, &accel);
        bmi088GyroRead(&gyroConfig, &gyro);

        madgwickUpdateIMU(&madgwickFilterData, gyro, accel);
        quatToEuler(&eulerData, &madgwickFilterData.q);

        printf("Accel X: %f\n", accel.x);
        printf("Accel Y: %f\n", accel.y);
        printf("Accel Z: %f\n", accel.z);
        printf("Gyro X: %f\n", RAD_2_DEG(gyro.x));
        printf("Gyro Y: %f\n", RAD_2_DEG(gyro.y));
        printf("Gyro Z: %f\n", RAD_2_DEG(gyro.z));
        printf("MAD Gyro X: %f\n", eulerData.x);
        printf("MAD Gyro Y: %f\n", eulerData.y);
        printf("MAD Gyro Z: %f\n", eulerData.z);

        sleepMiliseconds(10);
    }

    return 0;
}