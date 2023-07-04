#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "drivers/storage/flash_driver.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/console/console_input.h"
#include "core/common/measurements_utils.h"
#include "filters/exp_filter.h"
#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"
#include <stdio.h>
#include <string.h>

typedef struct MeasurementData_FlashSave
{
    INT32 _startSeqeuence;
    MeasurementData data;
} MeasurementData_FlashSave;

#define MEASUREMENTS_UPDATE_RATE_MS 10
#define MEASUREMENTS_BUFFER_SIZE 128
#define FLASH_STORAGE_MAX_SIZE (1 << 19)
#define FLASH_DATA_FRAME_START_SEQUENCE 0xEEEEEEEE

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define ACCEL_CS 11
#define GYRO_CS 10
#define MAG_CS 12
#define ACCEL2_CS 1

static MeasurementData_FlashSave s_Measurements[MEASUREMENTS_BUFFER_SIZE];
static SIZE s_MeasurementIndex;
static SIZE s_PagesOffset;

VOID readFlashData(VOID)
{
    const BYTE *newBuffer;
    flashRead(0, &newBuffer);
    MeasurementData_FlashSave *newData = (MeasurementData_FlashSave *)newBuffer;
    SIZE seq = 0, i = 0;

    while (TRUE)
    {
        memcpy(&seq, &newBuffer[i * sizeof(MeasurementData_FlashSave)], sizeof(SIZE));

        if (seq != 0xEEEEEEEE)
        {
            break;
        }

        printf("/*0,0,0,%f,%f,%f,0,0,0,0*/\n", newData[i].data.yaw, newData[i].data.pitch, newData[i].data.roll);

        i++;
    }

    printf("CMD:read-done\n");
}

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    flashEraseSectors(0, (SIZE)FLASH_STORAGE_MAX_SIZE / flashSectorSize());

    MadgiwckFilterData madgwickFilterData;
    BMI088AccelConfig accelConfig;
    BMI088GyroConfig gyroConfig;

    madgwickInit(&madgwickFilterData, 45.0f, 0.01f);

    bmi088AccelInitSPI(&accelConfig, SPI, MISO, MOSI, ACCEL_CS, SCK);
    bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_6G);

    bmi088GyroInitSPI(&gyroConfig, SPI, MISO, MOSI, GYRO_CS, SCK);
    bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_250DPS);

    MMC5983MAConfig mmc5983maConfig = {0};
    mmc5983maInitSPI(&mmc5983maConfig, SPI, MISO, MOSI, MAG_CS, SCK);
    mmc5983maSetContinuousModeODR(&mmc5983maConfig, MMC5983MA_ODR_1000HZ);

    H3lis331dlConfig h3lis331dlConfig = {0};
    h3lis331dlInitSPI(&h3lis331dlConfig, SPI, MISO, MOSI, ACCEL2_CS, SCK);
    h3lis331dlSetPowerMode(&h3lis331dlConfig, H3LIS331DL_POWER_NORMAL);
    h3lis331dlSetRange(&h3lis331dlConfig, H3LIS331DL_RANGE_100G);
    h3lis331dlSetODR(&h3lis331dlConfig, H3LIS331DL_ODR_1000HZ);

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

        MeasurementData measurement = {
            .pos_x = 0,
            .pos_y = 0,
            .pos_z = 0,
            .roll = eulerData.y,
            .pitch = eulerData.x,
            .yaw = eulerData.z,
            .latitude = 0,
            .longtitude = 0,
            .altitude = 0,
            .velocity = 0,
        };

        s_Measurements[s_MeasurementIndex++] = (MeasurementData_FlashSave){
            ._startSeqeuence = FLASH_DATA_FRAME_START_SEQUENCE,
            .data = measurement,
        };

        if (s_MeasurementIndex >= MEASUREMENTS_BUFFER_SIZE)
        {
            flashWritePages(s_PagesOffset, (BYTE *)s_Measurements, sizeof(s_Measurements) / flashWriteBufferSize());

            memset(s_Measurements, 0, sizeof(s_Measurements));
            s_MeasurementIndex = 0;
            s_PagesOffset += sizeof(s_Measurements) / flashWriteBufferSize();
        }

        sleepMiliseconds(10);
    }

    BOOL available = FALSE;
    INT32 chr = 0;
    ConsoleInput input = {0};
    BOOL tokensReady = FALSE;

    while (TRUE)
    {
        consoleCheckInput(&available, &chr);

        if (available)
        {
            consoleInputProcessCharacter(chr, &input, &tokensReady);

            if (tokensReady)
            {
                if (strcmp(input.tokens[0], "read-data") == 0)
                {
                    readFlashData();
                }

                tokensReady = FALSE;
            }

            available = FALSE;
        }
    }

    return 0;
}