#include "drivers/accelerometer/bmi088_driver.h"
#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define ACCEL_CS 11
#define GYRO_CS 10

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    spiInitAll(SPI, 10 * 1000 * 1000);

    MadgiwckFilterDataIMU madgwickFilterData;
    BMI088AccelConfig accelConfig;
    BMI088GyroConfig gyroConfig;

    madgwickInitIMU(&madgwickFilterData, 5, 0.1);

    bmi088AccelInit(&accelConfig, SPI, MISO, MOSI, ACCEL_CS, SCK);
    bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_24G);

    bmi088GyroInit(&gyroConfig, SPI, MISO, MOSI, GYRO_CS, SCK);
    bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_500DPS);

    while (TRUE)
    {
        BMI088AccelData accelData;
        BMI088GyroData gyroData;
        vec3 eulerData;

        bmi088AccelRead(&accelConfig, &accelData);
        bmi088GyroRead(&gyroConfig, &gyroData);

        madgwickUpdateIMU(&madgwickFilterData, gyroData.gyro, accelData.accel);
        quatToEuler(&eulerData, &madgwickFilterData.q);

        printf("Accel X: %f\n", accelData.accel.x);
        printf("Accel Y: %f\n", accelData.accel.y);
        printf("Accel Z: %f\n", accelData.accel.z);
        printf("Gyro X: %f\n", RAD_2_DEG(gyroData.gyro.x));
        printf("Gyro Y: %f\n", RAD_2_DEG(gyroData.gyro.y));
        printf("Gyro Z: %f\n", RAD_2_DEG(gyroData.gyro.z));
        printf("MAD Gyro X: %f\n", eulerData.x);
        printf("MAD Gyro Y: %f\n", eulerData.y);
        printf("MAD Gyro Z: %f\n", eulerData.z);

        sleep_ms(100);
    }

    return 0;
}