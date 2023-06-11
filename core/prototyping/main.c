#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "filters/exp_filter.h"
#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"
#include "tools/board_control.h"
#include "tools/time_tracker.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define ACCEL_CS 11
#define GYRO_CS 10
#define MAG_CS 12
#define ACCEL2_CS 1

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    MadgiwckFilterData madgwickFilterData;
    BMI088AccelConfig accelConfig;
    BMI088GyroConfig gyroConfig;

    madgwickInit(&madgwickFilterData, 45.0f, 0.002f);

    bmi088AccelInit(&accelConfig, SPI, MISO, MOSI, ACCEL_CS, SCK);
    bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_6G);

    bmi088GyroInit(&gyroConfig, SPI, MISO, MOSI, GYRO_CS, SCK);
    bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_250DPS);

    MMC5983MAConfig mmc5983maConfig = {0};
    mmc5983maInit(&mmc5983maConfig, SPI, MISO, MOSI, MAG_CS, SCK);
    mmc5983maSetContinuousModeODR(&mmc5983maConfig, MMC5983MA_ODR_200HZ);

    H3lis331dlConfig h3lis331dlConfig = {0};
    h3lis331dlInit(&h3lis331dlConfig, SPI, MISO, MOSI, ACCEL2_CS, SCK);
    h3lis331dlSetPowerMode(&h3lis331dlConfig, H3LIS331DL_POWER_NORMAL);
    h3lis331dlSetRange(&h3lis331dlConfig, H3LIS331DL_RANGE_100G);
    h3lis331dlSetODR(&h3lis331dlConfig, H3LIS331DL_ODR_400HZ);

    while (TRUE)
    {
        vec3 accel;
        vec3 accel2;
        vec3 gyro;
        vec3 mag;
        vec3 eulerData;

        bmi088AccelRead(&accelConfig, &accel);
        bmi088GyroRead(&gyroConfig, &gyro);
        mmc5983maRead(&mmc5983maConfig, &mag);
        h3lis331dlRead(&h3lis331dlConfig, &accel2);

        madgwickUpdateMARG(&madgwickFilterData, gyro, accel, mag);
        quatToEuler(&eulerData, &madgwickFilterData.q);

        printf("/*0,0,0,%f,%f,%f,0,0,0,0*/\n", eulerData.z, eulerData.x, eulerData.y);

        sleepMiliseconds(2);
    }

    return 0;
}