#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>
#include <string.h>
#include "maths/constants.h"
#include "maths/kalman_filter.h"
#include "ahrs/madgwick_filter.h"
#include "common/radio_utils.h"
#include "kernel/logging/logger.h"
#include "drivers/cpu/cpu_temp_driver.h"
#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/console/console_output.h"
#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include "drivers/led/w2812_driver.h"
#include "drivers/storage/sd_driver.h"
#include "drivers/battery/battery_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/barometer/bme688_driver.h"
#include "drivers/buzzer/simple_buzzer_driver.h"

#define SCK 18
#define MOSI 19
#define MISO 16

static RadioUtilPacketData s_Packet;

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    // ---------------------------------
    // spiInitAll(0, 1 * 1000 * 1000);
    // BME688Config bme688Config = {0};
    // bme688Init(&bme688Config, 0, MISO, MOSI, SCK, 4);
    // while (TRUE)
    // {
    //     BYTE chipId = 0;
    //     bme688GetChipId(&bme688Config, &chipId);
    //     printf("Chip ID: %d\n", chipId);
    //     sleep_ms(500);
    // }
    // ---------------------------------

    // ---------------------------------
    // spiInitAll(0, 1 * 1000 * 1000);
    // H3lis331dlConfig h3lis331dlConfig = {0};
    // h3lis331dlInit(&h3lis331dlConfig, 0, MISO, MOSI, 1, SCK);
    // h3lis331dlSetPower(&h3lis331dlConfig, TRUE);
    // h3lis331dlSetRange(&h3lis331dlConfig, H3LIS331DL_RANGE_100G);
    // h3lis331dlSetODR(&h3lis331dlConfig, H3LIS331DL_ODR_50HZ);

    // while (TRUE)
    // {
    //     H3lis331dlData accelData = {0};
    //     h3lis331dlRead(&h3lis331dlConfig, &accelData);
    //     sleep_ms(50);
    // }
    // ---------------------------------

    // ---------------------------------
    spiInitAll(0, 1 * 1000 * 1000);
    MMC5983MAConfig mmc5983maConfig = {0};
    mmc5983maInit(&mmc5983maConfig, 0, MISO, MOSI, 12, SCK);
    mmc5983maSetBandwidth(&mmc5983maConfig, MMC5983MA_BW_100HZ);
    while (TRUE)
    {
        MMC5983MAData magData = {0};
        mmc5983maRead(&mmc5983maConfig, &magData);
        sleep_ms(50);
    }
    // ---------------------------------

    // ---------------------------------
    // SDCard sdCard = {0};

    // sdInit(&sdCard, 0);
    // sdInitFile(&sdCard, "TESTXDD.txt");
    // sdClearFile(&sdCard, "TESTXDD.txt");
    // sdBegin(&sdCard, "TESTXDD.txt");
    // sdWrite(&sdCard, "Kolejny test", "TESTXDD.txt");
    // sdEnd(&sdCard, "TESTXDD.txt");
    // sdTerminate(&sdCard);

    // while (TRUE)
    // {
    //     sleep_ms(1000);
    // }
    // ---------------------------------

    // ---------------------------------
    // adcInitAll();
    // BatteryConfig batteryConfig = {0};
    // batteryInit(&batteryConfig, 3, 6.0f, 8.4f);

    // while (TRUE)
    // {
    //     FLOAT percent = -1;
    //     batteryReadPercent(&batteryConfig, &percent);
    //     printf("Battery: %f\n", percent);
    //     sleep_ms(500);
    // }
    // ---------------------------------

    // ---------------------------------
    // // MadgiwckFilterDataIMU madgwickFilterData;
    // BMI088AccelConfig accelConfig;
    // BMI088GyroConfig gyroConfig;

    // // madgwickInitIMU(&madgwickFilterData, 5, 0.1);

    // spiInitAll(0, 10 * 1000 * 1000);

    // bmi088AccelInit(&accelConfig, 0, MISO, MOSI, ACCEL_CS, SCK);
    // bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL);
    // bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_24G);

    // bmi088GyroInit(&gyroConfig, 0, MISO, MOSI, GYRO_CS, SCK);
    // bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    // bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_500DPS);

    // while (TRUE)
    // {
    //     BMI088AccelData accelData;
    //     BMI088GyroData gyroData;
    //     vec3 eulerData;

    //     bmi088AccelRead(&accelConfig, &accelData);
    //     bmi088GyroRead(&gyroConfig, &gyroData);

    //     // madgwickUpdateIMU(&madgwickFilterData, gyroData.gyro, accelData.accel);
    //     // quatToEuler(&eulerData, &madgwickFilterData.q);

    //     printf("Accel X: %f\n", accelData.accel.x);
    //     printf("Accel Y: %f\n", accelData.accel.y);
    //     printf("Accel Z: %f\n", accelData.accel.z);
    //     printf("Gyro X: %f\n", RAD_2_DEG(gyroData.gyro.x));
    //     printf("Gyro Y: %f\n", RAD_2_DEG(gyroData.gyro.y));
    //     printf("Gyro Z: %f\n", RAD_2_DEG(gyroData.gyro.z));
    //     // printf("MAD Gyro X: %f\n", eulerData.x);
    //     // printf("MAD Gyro Y: %f\n", eulerData.y);
    //     // printf("MAD Gyro Z: %f\n", eulerData.z);

    //     sleep_ms(100);
    // }
    // ---------------------------------

    // ---------------------------------
    // gpioInitPin(29, GPIO_OUTPUT);

    // while (TRUE)
    // {
    //     gpioSetPinState(29, GPIO_HIGH);
    //     sleep_ms(1000);
    //     gpioSetPinState(29, GPIO_LOW);
    //     sleep_ms(1000);
    // }
    // ---------------------------------

    return 0;
}