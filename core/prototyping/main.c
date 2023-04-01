#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>
#include <stdlib.h>
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
#include "drivers/gps/nmea_parser.h"
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
    // bme688SetHumidityOSR(&bme688Config, BME688_SENSOR_OSR_16X);
    // bme688SetTemperatureOSR(&bme688Config, BME688_SENSOR_OSR_2X);
    // bme688SetPressureOSR(&bme688Config, BME688_SENSOR_OSR_1X);
    // bme688SetIIRFilter(&bme688Config, BME688_IIR_FILTER_COEFF_OFF);
    // __bme688SetGasSensorHeaterOnTime(&bme688Config, 0, 100);
    // __bme688SetTargetHeaterTemp(&bme688Config, 0, 300);
    // __bme688SetHeaterProfile(&bme688Config, 0);
    // __bme688RunGas(&bme688Config);
    // bme688SetMode(&bme688Config, BME688_MODE_FORCED);

    // while (TRUE)
    // {
    //     if (__bme688IsDataReady(&bme688Config, 0))
    //     {
    //         BME688Data data = {0};
    //         bme688Read(&bme688Config, &data);
    //         printf("Temperature: %f\n", data.temperature);
    //         printf("Pressure: %f\n", data.pressure);
    //         printf("Humidity: %f\n", data.humidity);
    //         printf("Gas: %f\n", data.gas);
    //     }

    //     sleep_ms(10);
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
    // spiInitAll(0, 1 * 1000 * 1000);
    // MMC5983MAConfig mmc5983maConfig = {0};
    // mmc5983maInit(&mmc5983maConfig, 0, MISO, MOSI, 12, SCK);
    // mmc5983maSetBandwidth(&mmc5983maConfig, MMC5983MA_BW_100HZ);
    // while (TRUE)
    // {
    //     MMC5983MAData magData = {0};
    //     mmc5983maRead(&mmc5983maConfig, &magData);
    //     sleep_ms(50);
    // }
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
    // BatteryInterval batteryIntervals[10] = {
    //     {0.7273f, 0.7636f, 90.0f, 100.0f},
    //     {0.7182, 0.7273f, 80.0f, 90.0f},
    //     {0.6999f, 0.7182f, 70.0f, 80.0f},
    //     {0.6909f, 0.6999f, 60.0f, 70.0f},
    //     {0.6818f, 0.6909f, 50.0f, 60.0f},
    //     {0.6764, 0.6818f, 40.0f, 50.0f},
    //     {0.6727f, 0.6764f, 30.0f, 40.0f},
    //     {0.6673, 0.6727f, 20.0f, 30.0f},
    //     {0.6636f, 0.6673f, 10.0f, 20.0f},
    //     {0.5455f, 0.6636f, 0.0f, 10.0f},
    // };
    // BatteryConfig batteryConfig = {0};
    // batteryInit(&batteryConfig, 3, batteryIntervals, 10);

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