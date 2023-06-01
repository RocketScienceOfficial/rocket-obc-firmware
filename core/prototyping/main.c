#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"

#include "filters/exp_filter.h"
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
#define MAG_CS 12

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    // spiInitAll(SPI, 10 * 1000 * 1000);

    // MadgiwckFilterData madgwickFilterData;
    // BMI088AccelConfig accelConfig;
    // BMI088GyroConfig gyroConfig;

    // madgwickInit(&madgwickFilterData, 5, 0.1);

    // bmi088AccelInit(&accelConfig, SPI, MISO, MOSI, ACCEL_CS, SCK);
    // bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL);
    // bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_24G);

    // bmi088GyroInit(&gyroConfig, SPI, MISO, MOSI, GYRO_CS, SCK);
    // bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    // bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_500DPS);

    // MMC5983MAConfig mmc5983maConfig = {0};
    // mmc5983maInit(&mmc5983maConfig, SPI, MISO, MOSI, MAG_CS, SCK);
    // mmc5983maSetODR(&mmc5983maConfig, MMC5983MA_ODR_200HZ);

    // while (TRUE)
    // {
    //     BMI088AccelData accelData;
    //     BMI088GyroData gyroData;
    //     MMC5983MAData magData;
    //     vec3 eulerData;

    //     bmi088AccelRead(&accelConfig, &accelData);
    //     bmi088GyroRead(&gyroConfig, &gyroData);
    //     mmc5983maRead(&mmc5983maConfig, &magData);

    //     madgwickUpdateMARG(&madgwickFilterData, gyroData.gyro, accelData.accel, magData.mag);
    //     quatToEuler(&eulerData, &madgwickFilterData.q);

    //     filterAvg(&avgFilterData, &eulerData.x);
    //     filterAvg(&avgFilterData, &eulerData.y);
    //     filterAvg(&avgFilterData, &eulerData.z);

    //     printf("X: %f\n", eulerData.x);
    //     printf("Y: %f\n", eulerData.y);
    //     printf("Z: %f\n", eulerData.z);

    //     sleep_ms(100);
    // }

    return 0;
}

// #include "pico/stdlib.h"
// #include "core/common/measurements_utils.h"
// #include "drivers/storage/flash_driver.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>

// typedef struct MeasurementData_FlashSave
// {
//     INT32 _startSeqeuence;
//     MeasurementData data;
// } MeasurementData_FlashSave;

// int main()
// {
//     stdio_init_all();
//     sleep_ms(10000);

//     initializeMeasurementLogger();

//     const SIZE dataSize = 256;
//     const SIZE singleDataSize = sizeof(MeasurementData_FlashSave);
//     const SIZE dataBytesSize = dataSize * singleDataSize;

//     MeasurementData_FlashSave *data = (MeasurementData_FlashSave *)malloc(dataBytesSize);

//     for (SIZE i = 0; i < dataSize; i++)
//     {
//         data[i] = (MeasurementData_FlashSave){0};
//         data[i]._startSeqeuence = 0xEEEEEEEE;
//         data[i].data.accel_x = 100;
//     }

//     BYTE *buffer = (BYTE *)data;

//     flashEraseSectors(0, (SIZE)ceil((FLOAT)dataBytesSize / flashSectorSize()));
//     flashWritePages(0, buffer, dataBytesSize / flashWriteBufferSize());

//     free(data);

//     const BYTE *newBuffer;
//     flashRead(0, &newBuffer);
//     MeasurementData_FlashSave *newData = (MeasurementData_FlashSave *)newBuffer;
//     INT32 seq = 0, i = 0;

//     while (TRUE)
//     {
//         memcpy(&seq, &newBuffer[i * sizeof(MeasurementData_FlashSave)], sizeof(INT32));

//         if (seq != 0xEEEEEEEE)
//         {
//             break;
//         }

//         logMeasurementData(&newData[i].data);

//         i++;
//     }

//     while (TRUE)
//     {
//         sleep_ms(1000);
//     }

//     return 0;
// }