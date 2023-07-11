#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pinout.h"
#include "core/common/radio_utils.h"
#include "core/common/measurements_utils.h"
#include "core/common/driver_calling.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/tools/board_control.h"
#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/barometer/bme688_driver.h"
#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include "drivers/gps/nmea_parser.h"
#include "drivers/storage/flash_driver.h"
#include "drivers/console/console_input.h"
#include "ahrs/madgwick_filter.h"
#include "maths/constants.h"

typedef struct MeasurementData_FlashSave
{
    INT32 _startSeqeuence;
    MeasurementData data;
} MeasurementData_FlashSave;

#define MEASUREMENTS_UPDATE_RATE_MS 10
#define MEASUREMENTS_BUFFER_SIZE 128
#define FLASH_STORAGE_MAX_SIZE (1 << 19)
#define FLASH_DATA_FRAME_START_SEQUENCE 0xEEEEEEEE

static RadioUtilPacketData s_Packet;
static MeasurementData_FlashSave s_Measurements[MEASUREMENTS_BUFFER_SIZE];
static SIZE s_MeasurementIndex;
static SIZE s_PagesOffset;
static MadgiwckFilterData s_MadgwickFilterData;
static BMI088AccelConfig s_BMI088AccelConfig;
static BMI088GyroConfig s_BMI088GyroConfig;
static BME688Config s_BME688Config;
static H3lis331dlConfig s_H3lis331dlConfig;
static MMC5983MAConfig s_MMC5983MAConfig;
static UBloxNeoM9NConfig s_UBloxNeoM9NConfig;
static UBloxNeoM9NData s_UBloxNeoM9NData;
static TIME s_TimerOffset;

VOID initSensors(VOID);
VOID takeMeasurements(MeasurementData *measurements);
VOID readFlashData(VOID);

int main(VOID)
{
    boardInit(0);

    initSensors();

    while (TRUE)
    {
        if (runEvery(MEASUREMENTS_UPDATE_RATE_MS, &s_TimerOffset))
        {
            MeasurementData measurement = {0};

            takeMeasurements(&measurement);

            s_Measurements[s_MeasurementIndex++] = (MeasurementData_FlashSave){
                ._startSeqeuence = FLASH_DATA_FRAME_START_SEQUENCE,
                .data = measurement,
            };

            if (s_MeasurementIndex >= MEASUREMENTS_BUFFER_SIZE)
            {
                DRIVER_CALL(flashWritePages(s_PagesOffset, (BYTE *)s_Measurements, sizeof(s_Measurements) / flashWriteBufferSize()));

                memset(s_Measurements, 0, sizeof(s_Measurements));
                s_MeasurementIndex = 0;
                s_PagesOffset += sizeof(s_Measurements) / flashWriteBufferSize();
            }

            RadioBody body = {0};

            body.command = MEASUREMENTS_RADIO_COMMAND_ID;
            body.payloadSize = sizeof(measurement);

            memcpy(body.payload, &measurement, body.payloadSize);

            sendRadioPacket(&body);
        }
    }

    return 0;
}

VOID initSensors(VOID)
{
    spiInitAll(0, 1 * 1000 * 1000);

    SX127XPinout loraPinout = (SX127XPinout){
        .spi = SX1278_SPI,
        .sck = SX1278_SCK_GPIO,
        .miso = SX1278_MISO_GPIO,
        .mosi = SX1278_MOSI_GPIO,
        .cs = SX1278_CS_GPIO,
        .reset = SX1278_RESET_GPIO,
    };

    initializeRadio(&loraPinout);

    madgwickInit(&s_MadgwickFilterData, 5, 0.1);

    DRIVER_CALL(bmi088AccelInitSPI(&s_BMI088AccelConfig, BMI088_SPI, BMI088_MISO, BMI088_MOSI, BMI088_ACCEL_CS, BMI088_SCK));
    DRIVER_CALL(bmi088AccelSetConf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL));
    DRIVER_CALL(bmi088AccelSetRange(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_24G));

    DRIVER_CALL(bmi088GyroInitSPI(&s_BMI088GyroConfig, BMI088_SPI, BMI088_MISO, BMI088_MOSI, BMI088_GYRO_CS, BMI088_SCK));
    DRIVER_CALL(bmi088GyroSetBandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ));
    DRIVER_CALL(bmi088GyroSetRange(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS));

    DRIVER_CALL(bme688Init(&s_BME688Config, BME688_SPI, BME688_MISO, BME688_MOSI, BME688_SCK, BME688_CS));
    DRIVER_CALL(bme688SetConfig(&s_BME688Config, BME688_SENSOR_OSR_2X, BME688_SENSOR_OSR_1X, BME688_SENSOR_OSR_16X, BME688_IIR_FILTER_COEFF_OFF));

    DRIVER_CALL(h3lis331dlInitSPI(&s_H3lis331dlConfig, H3LIS331DL_SPI, H3LIS331DL_MISO, H3LIS331DL_MOSI, H3LIS331DL_CS, H3LIS331DL_SCK));
    DRIVER_CALL(h3lis331dlSetPowerMode(&s_H3lis331dlConfig, H3LIS331DL_POWER_NORMAL));
    DRIVER_CALL(h3lis331dlSetRange(&s_H3lis331dlConfig, H3LIS331DL_RANGE_100G));
    DRIVER_CALL(h3lis331dlSetODR(&s_H3lis331dlConfig, H3LIS331DL_ODR_50HZ));

    DRIVER_CALL(mmc5983maInitSPI(&s_MMC5983MAConfig, MMC5983MA_SPI, MMC5983MA_MISO, MMC5983MA_MOSI, MMC5983MA_CS, MMC5983MA_SCK));
    DRIVER_CALL(mmc5983maSetContinuousModeODR(&s_MMC5983MAConfig, MMC5983MA_ODR_200HZ));

    DRIVER_CALL(uBloxNeoM9NInitSPI(&s_UBloxNeoM9NConfig, UBX_NEO_M9N_SPI, UBX_NEO_M9N_MISO, UBX_NEO_M9N_MOSI, UBX_NEO_M9N_CS, UBX_NEO_M9N_SCK));

    DRIVER_CALL(flashEraseSectors(0, (SIZE)FLASH_STORAGE_MAX_SIZE / flashSectorSize()));
}

VOID takeMeasurements(MeasurementData *measurements)
{
    vec3 accel_low;
    vec3 accel_high;
    vec3 gyro;
    vec3 mag;
    FLOAT lon = 0, lat = 0;
    vec3 eulerData;

    DRIVER_CALL(bmi088AccelRead(&s_BMI088AccelConfig, &accel_low));
    DRIVER_CALL(bmi088GyroRead(&s_BMI088GyroConfig, &gyro));
    DRIVER_CALL(h3lis331dlRead(&s_H3lis331dlConfig, &accel_high));
    DRIVER_CALL(mmc5983maRead(&s_MMC5983MAConfig, &mag));

    DRIVER_CALL(uBloxNeoM9NReadData(&s_UBloxNeoM9NConfig, &s_UBloxNeoM9NData));

    if (s_UBloxNeoM9NData.isFinishedSentence)
    {
        BOOL valid = nmeaCheckSentence(s_UBloxNeoM9NData.sentence);

        printf("Valid: %d\n", valid);

        if (valid)
        {
            NMEASentence s = nmeaGetSentenceId(s_UBloxNeoM9NData.sentence);
            BOOL isPos = FALSE;

            if (s == NMEA_SENTENCE_GGA)
            {
                NMEASentence_GGA frame = {0};
                nmeaParse_GGA(s_UBloxNeoM9NData.sentence, &frame);

                lat = frame.lat;
                lon = frame.lon;
            }
            else if (s == NMEA_SENTENCE_GLL)
            {
                NMEASentence_GLL frame = {0};
                nmeaParse_GLL(s_UBloxNeoM9NData.sentence, &frame);

                lat = frame.lat;
                lon = frame.lon;
            }
            else if (s == NMEA_SENTENCE_GNS)
            {
                NMEASentence_GNS frame = {0};
                nmeaParse_GNS(s_UBloxNeoM9NData.sentence, &frame);

                lat = frame.lat;
                lon = frame.lon;
            }
        }
    }

    madgwickUpdateMARG(&s_MadgwickFilterData, gyro, accel_low, mag);
    quatToEuler(&eulerData, &s_MadgwickFilterData.q);

    *measurements = (MeasurementData){
        .pos_x = 0,
        .pos_y = 0,
        .pos_z = 0,
        .roll = eulerData.y,
        .pitch = eulerData.x,
        .yaw = eulerData.z,
        .latitude = lat,
        .longtitude = lon,
        .altitude = 0,
        .velocity = 0,
    };
}

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

        logMeasurementData(&newData[i].data);

        i++;
    }

    printf("CMD:read-done\n");
}

VOID checkCMD()
{
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
}