#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

#include "drivers/accelerometer/bmi088_driver.h"
#include "maths/kalman_filter.h"

#define SPI spi0
#define ACCEL_CS 1
#define GYRO_CS 5
#define SCK 2
#define MOSI 3
#define MISO 4

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    BMI088AccelConfig accelConfig;

    bmi088AccelInit(&accelConfig, 0, MISO, MOSI, ACCEL_CS, SCK);
    bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_4);
    bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_4G);

    BMI088GyroConfig gyroConfig;

    bmi088GyroInit(&gyroConfig, 0, MISO, MOSI, GYRO_CS, SCK);
    bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_250DPS);

    while (TRUE)
    {
        BMI088AccelData accelData;
        BMI088GyroData gyroData;

        bmi088AccelRead(&accelConfig, &accelData);
        bmi088GyroRead(&gyroConfig, &gyroData);

        KalmanOutputData kalmanData;

        kalmanUpdate(&kalmanData, accelData.accel);

        printf("Accel X: %f\n", accelData.accel.x);
        printf("Accel Y: %f\n", accelData.accel.y);
        printf("Accel Z: %f\n", accelData.accel.z);
        printf("Kalman Accel X: %f\n", kalmanData.acc.x);
        printf("Kalman Accel Y: %f\n", kalmanData.acc.y);
        printf("Kalman Accel Z: %f\n", kalmanData.acc.z);
        printf("Gyro X: %f\n", gyroData.gyro.x);
        printf("Gyro Y: %f\n", gyroData.gyro.y);
        printf("Gyro Z: %f\n", gyroData.gyro.z);

        sleep_ms(100);
    }

    return 0;
}