// #include "measurements.h"
// #include "config.h"
// #include "driver_calling.h"
// #include "services/logging/logger.h"
// #include "drivers/accelerometer/bmi088_driver.h"
// #include "drivers/accelerometer/h3lis331dl_driver.h"
// #include "drivers/accelerometer/lsm6dso32_driver.h"
// #include "drivers/barometer/bmp581_driver.h"
// #include "drivers/magnetometer/mmc5983ma_driver.h"
// #include "drivers/gps/u_blox_sam_m10q_driver.h"
// #include "drivers/gps/nmea_parser.h"
// #include "maths/vector.h"

// static Logger s_Logger;

// #define MY_LOG_RECEIVER_DATA_PATTERN "%c"
// #define MY_LOG_RECEIVER_DATA_NAME "RECEIVER_DATA_LOG"
// #define MY_LOG_RECEIVER_DATA_BEGIN() myLog(&s_Logger, "", "/*")
// #define MY_LOG_RECEIVER_DATA_END() myLog(&s_Logger, "", "*/\n")
// #define MY_LOG_RECEIVER_DATA_INTERNAL(val, format) myLog(&s_Logger, "", format, val)
// #define MY_LOG_RECEIVER_DATA_FLOAT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%f,")
// #define MY_LOG_RECEIVER_DATA_INT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%d,")
// #define MY_LOG_RECEIVER_DATA_EMPTY() myLog(&s_Logger, "", ",")

// static BMI088AccelConfig s_BMI088AccelConfig;
// static BMI088GyroConfig s_BMI088GyroConfig;
// static BMP581Config s_BMP581Config;
// static H3lis331dlConfig s_H3lis331dlConfig;
// static LSM6DSO32Config s_LSM6DSO32Config;
// static MMC5983MAConfig s_MMC5983MAConfig;
// static UBloxSamM10QConfig s_UBloxNeoM9NConfig;
// static UBloxSamM10QData s_UBloxNeoM9NData;

// static vec3 s_LastGPSPos;

// VOID initializeMeasurementLogger()
// {
//     myLogCreateLogger(&s_Logger, MY_LOG_RECEIVER_DATA_NAME);
//     myLogCreateConsoleSink(&s_Logger, MY_LOG_RECEIVER_DATA_PATTERN);
// }

// VOID logMeasurementData(MeasurementData *data)
// {
//     MY_LOG_RECEIVER_DATA_BEGIN();

//     MY_LOG_RECEIVER_DATA_FLOAT(data->pos_x);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->pos_y);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->pos_z);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->roll);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->pitch);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->yaw);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->latitude);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->longitude);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->altitude);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->velocity);

//     MY_LOG_RECEIVER_DATA_END();
// }

// VOID logRawMeasurementData(RawMeasurementData *data)
// {
//     MY_LOG_RECEIVER_DATA_BEGIN();

//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelX_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelX_2);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelX_3);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelY_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelY_2);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelY_3);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelZ_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelZ_2);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->accelZ_3);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolX_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolX_2);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolY_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolY_2);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolZ_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolZ_2);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->magX_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->magY_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->magZ_1);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->lat);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->lon);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->alt);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->pressure);
//     MY_LOG_RECEIVER_DATA_FLOAT(data->temperature);

//     MY_LOG_RECEIVER_DATA_END();
// }

// VOID initSensors(VOID)
// {
//     DRIVER_CALL(bmi088AccelInitI2C(&s_BMI088AccelConfig, I2C, SDA, SCL, TRUE));
//     DRIVER_CALL(bmi088AccelSetConf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL));
//     DRIVER_CALL(bmi088AccelSetRange(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_6G));

//     DRIVER_CALL(bmi088GyroInitI2C(&s_BMI088GyroConfig, I2C, SDA, SCL, TRUE));
//     DRIVER_CALL(bmi088GyroSetBandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ));
//     DRIVER_CALL(bmi088GyroSetRange(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS));

//     DRIVER_CALL(lsm6dso32InitI2C(&s_LSM6DSO32Config, I2C, SDA, SCL));
//     DRIVER_CALL(lsm6dso32SetODR(&s_LSM6DSO32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ));
//     DRIVER_CALL(lsm6dso32SetRange(&s_LSM6DSO32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS));

//     DRIVER_CALL(bmp581InitI2C(&s_BMP581Config, I2C, SDA, SCL));
//     DRIVER_CALL(bmp581SetMode(&s_BMP581Config, BMP581_MODE_CONTINUOUS));
//     DRIVER_CALL(bmp581SetODR_OSR(&s_BMP581Config, BMP581_OSR_1, BMP581_OSR_1, BMP581_ODR_240));

//     DRIVER_CALL(h3lis331dlInitI2C(&s_H3lis331dlConfig, I2C, SDA, SCL));
//     DRIVER_CALL(h3lis331dlSetPowerMode(&s_H3lis331dlConfig, H3LIS331DL_POWER_NORMAL));
//     DRIVER_CALL(h3lis331dlSetRange(&s_H3lis331dlConfig, H3LIS331DL_RANGE_200G));
//     DRIVER_CALL(h3lis331dlSetODR(&s_H3lis331dlConfig, H3LIS331DL_ODR_400HZ));

//     DRIVER_CALL(mmc5983maInitI2C(&s_MMC5983MAConfig, I2C, SDA, SCL));
//     DRIVER_CALL(mmc5983maSetContinuousModeODR(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ));

//     DRIVER_CALL(uBloxSamM10QInitI2C(&s_UBloxNeoM9NConfig, I2C, SDA, SCL));
// }

// VOID takeMeasurements(RawMeasurementData *rawMeasurements)
// {
//     vec3 accel_1 = {0};
//     vec3 accel_2 = {0};
//     vec3 accel_3 = {0};
//     vec3 gyro_1 = {0};
//     vec3 gyro_2 = {0};
//     vec3 mag = {0};
//     BMP581Data baroData = {0};

//     DRIVER_CALL(bmi088AccelRead(&s_BMI088AccelConfig, &accel_1));
//     DRIVER_CALL(bmi088GyroRead(&s_BMI088GyroConfig, &gyro_1));
//     DRIVER_CALL(lsm6dso32Read(&s_LSM6DSO32Config, &accel_2, &gyro_2, NULL));
//     DRIVER_CALL(h3lis331dlRead(&s_H3lis331dlConfig, &accel_3));
//     DRIVER_CALL(mmc5983maRead(&s_MMC5983MAConfig, &mag));
//     DRIVER_CALL(bmp581Read(&s_BMP581Config, &baroData));

//     *rawMeasurements = (RawMeasurementData){
//         .accelX_1 = accel_1.x,
//         .accelX_2 = accel_2.x,
//         .accelX_3 = accel_3.x,
//         .accelY_1 = accel_1.y,
//         .accelY_2 = accel_2.y,
//         .accelY_3 = accel_3.y,
//         .accelZ_1 = accel_1.z,
//         .accelZ_2 = accel_2.z,
//         .accelZ_3 = accel_3.z,
//         .gyrolX_1 = gyro_1.x,
//         .gyrolX_2 = gyro_2.x,
//         .gyrolY_1 = gyro_1.y,
//         .gyrolY_2 = gyro_2.y,
//         .gyrolZ_1 = gyro_1.z,
//         .gyrolZ_2 = gyro_2.z,
//         .magX_1 = mag.x,
//         .magY_1 = mag.y,
//         .magZ_1 = mag.z,
//         .lat = s_LastGPSPos.x,
//         .lon = s_LastGPSPos.y,
//         .alt = s_LastGPSPos.z,
//         .pressure = baroData.pressure,
//         .temperature = baroData.temperature,
//     };
// }

// BOOL checkGPS(VOID)
// {
//     DRIVER_CALL(uBloxSamM10QReadData(&s_UBloxNeoM9NConfig, &s_UBloxNeoM9NData));

//     if (s_UBloxNeoM9NData.isFinishedSentence)
//     {
//         if (nmeaCheckSentence(s_UBloxNeoM9NData.sentence))
//         {
//             NMEASentence s = nmeaGetSentenceId(s_UBloxNeoM9NData.sentence);

//             if (s == NMEA_SENTENCE_GGA)
//             {
//                 NMEASentence_GGA frame = {0};
//                 nmeaParse_GGA(s_UBloxNeoM9NData.sentence, &frame);

//                 s_LastGPSPos.x = frame.lat;
//                 s_LastGPSPos.y = frame.lon;
//                 s_LastGPSPos.z = frame.alt;

//                 return TRUE;
//             }
//             else if (s == NMEA_SENTENCE_GLL)
//             {
//                 NMEASentence_GLL frame = {0};
//                 nmeaParse_GLL(s_UBloxNeoM9NData.sentence, &frame);

//                 s_LastGPSPos.x = frame.lat;
//                 s_LastGPSPos.y = frame.lon;

//                 return TRUE;
//             }
//             else if (s == NMEA_SENTENCE_GNS)
//             {
//                 NMEASentence_GNS frame = {0};
//                 nmeaParse_GNS(s_UBloxNeoM9NData.sentence, &frame);

//                 s_LastGPSPos.x = frame.lat;
//                 s_LastGPSPos.y = frame.lon;
//                 s_LastGPSPos.z = frame.alt;

//                 return TRUE;
//             }
//         }
//     }

//     return FALSE;
// }

#include "measurements.h"
#include "config.h"
#include "driver_calling.h"
#include "services/logging/logger.h"
#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/accelerometer/lsm6dso32_driver.h"
#include "drivers/barometer/bme688_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include "drivers/gps/nmea_parser.h"
#include "maths/vector.h"

static Logger s_Logger;

#define MY_LOG_RECEIVER_DATA_PATTERN "%c"
#define MY_LOG_RECEIVER_DATA_NAME "RECEIVER_DATA_LOG"
#define MY_LOG_RECEIVER_DATA_BEGIN() myLog(&s_Logger, "", "/*")
#define MY_LOG_RECEIVER_DATA_END() myLog(&s_Logger, "", "*/\n")
#define MY_LOG_RECEIVER_DATA_INTERNAL(val, format) myLog(&s_Logger, "", format, val)
#define MY_LOG_RECEIVER_DATA_FLOAT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%f,")
#define MY_LOG_RECEIVER_DATA_INT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%d,")
#define MY_LOG_RECEIVER_DATA_EMPTY() myLog(&s_Logger, "", ",")

static BMI088AccelConfig s_BMI088AccelConfig;
static BMI088GyroConfig s_BMI088GyroConfig;
static BME688Config s_BME688Config;
static H3lis331dlConfig s_H3lis331dlConfig;
static LSM6DSO32Config s_LSM6DSO32Config;
static MMC5983MAConfig s_MMC5983MAConfig;
static UBloxNeoM9NConfig s_UBloxNeoM9NConfig;
static UBloxNeoM9NData s_UBloxNeoM9NData;

static vec3 s_LastGPSPos;

VOID initMeasurementLogger()
{
    myLogCreateLogger(&s_Logger, MY_LOG_RECEIVER_DATA_NAME);
    myLogCreateConsoleSink(&s_Logger, MY_LOG_RECEIVER_DATA_PATTERN);
}

VOID logMeasurementData(MeasurementData *data)
{
    MY_LOG_RECEIVER_DATA_BEGIN();

    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->roll);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pitch);
    MY_LOG_RECEIVER_DATA_FLOAT(data->yaw);
    MY_LOG_RECEIVER_DATA_FLOAT(data->latitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->longitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->altitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->velocity);

    MY_LOG_RECEIVER_DATA_END();
}

VOID logRawMeasurementData(RawMeasurementData *data)
{
    MY_LOG_RECEIVER_DATA_BEGIN();

    MY_LOG_RECEIVER_DATA_FLOAT(data->accelX_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelX_2);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelX_3);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelY_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelY_2);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelY_3);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelZ_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelZ_2);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accelZ_3);
    MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolX_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolX_2);
    MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolY_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolY_2);
    MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolZ_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->gyrolZ_2);
    MY_LOG_RECEIVER_DATA_FLOAT(data->magX_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->magY_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->magZ_1);
    MY_LOG_RECEIVER_DATA_FLOAT(data->lat);
    MY_LOG_RECEIVER_DATA_FLOAT(data->lon);
    MY_LOG_RECEIVER_DATA_FLOAT(data->alt);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pressure);
    MY_LOG_RECEIVER_DATA_FLOAT(data->temperature);

    MY_LOG_RECEIVER_DATA_END();
}

VOID initSensors(VOID)
{
    DRIVER_CALL(bmi088AccelInitSPI(&s_BMI088AccelConfig, SPI, MISO, MOSI, BMI088_ACCEL_CS_PIN, SCK));
    DRIVER_CALL(bmi088AccelSetConf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_1600HZ, BMI088_ACCEL_OSR_NORMAL));
    DRIVER_CALL(bmi088AccelSetRange(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_6G));

    DRIVER_CALL(bmi088GyroInitSPI(&s_BMI088GyroConfig, SPI, MISO, MOSI, BMI088_GYRO_CS_PIN, SCK));
    DRIVER_CALL(bmi088GyroSetBandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ));
    DRIVER_CALL(bmi088GyroSetRange(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS));

    DRIVER_CALL(lsm6dso32InitSPI(&s_LSM6DSO32Config, SPI, MISO, MOSI, LSM6DSO32_CS_PIN, SCK));
    DRIVER_CALL(lsm6dso32SetODR(&s_LSM6DSO32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ));
    DRIVER_CALL(lsm6dso32SetRange(&s_LSM6DSO32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS));

    DRIVER_CALL(bme688Init(&s_BME688Config, SPI, MISO, MOSI, SCK, BME688_CS_PIN));
    DRIVER_CALL(bme688SetConfig(&s_BME688Config, BME688_SENSOR_OSR_8X, BME688_SENSOR_OSR_8X, BME688_SENSOR_OSR_16X, BME688_IIR_FILTER_COEFF_OFF));
    DRIVER_CALL(bme688SetHeaterConfig(&s_BME688Config, 0, 0, 300, 0, 100));

    DRIVER_CALL(h3lis331dlInitSPI(&s_H3lis331dlConfig, SPI, MISO, MOSI, H3LIS331DL_CS_PIN, SCK));
    DRIVER_CALL(h3lis331dlSetPowerMode(&s_H3lis331dlConfig, H3LIS331DL_POWER_NORMAL));
    DRIVER_CALL(h3lis331dlSetRange(&s_H3lis331dlConfig, H3LIS331DL_RANGE_200G));
    DRIVER_CALL(h3lis331dlSetODR(&s_H3lis331dlConfig, H3LIS331DL_ODR_400HZ));

    DRIVER_CALL(mmc5983maInitSPI(&s_MMC5983MAConfig, SPI, MISO, MOSI, MMC5983MA_CS_PIN, SCK));
    DRIVER_CALL(mmc5983maSetContinuousModeODR(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ));

    DRIVER_CALL(uBloxNeoM9NInitSPI(&s_UBloxNeoM9NConfig, SPI, MISO, MOSI, UBLOX_NEO_M9N_CS_PIN, SCK));
}

VOID takeMeasurements(RawMeasurementData *rawMeasurements)
{
    vec3 accel_1 = {0};
    vec3 accel_2 = {0};
    vec3 accel_3 = {0};
    vec3 gyro_1 = {0};
    vec3 gyro_2 = {0};
    vec3 mag = {0};
    BME688Data baroData = {0};

    DRIVER_CALL(bme688SetMode(&s_BME688Config, BME688_MODE_FORCED));

    DRIVER_CALL(bmi088AccelRead(&s_BMI088AccelConfig, &accel_1));
    DRIVER_CALL(bmi088GyroRead(&s_BMI088GyroConfig, &gyro_1));
    DRIVER_CALL(lsm6dso32Read(&s_LSM6DSO32Config, &accel_2, &gyro_2, NULL));
    DRIVER_CALL(h3lis331dlRead(&s_H3lis331dlConfig, &accel_3));
    DRIVER_CALL(mmc5983maRead(&s_MMC5983MAConfig, &mag));
    DRIVER_CALL(bme688Read(&s_BME688Config, &baroData));

    *rawMeasurements = (RawMeasurementData){
        .accelX_1 = accel_1.x,
        .accelX_2 = accel_2.x,
        .accelX_3 = accel_3.x,
        .accelY_1 = accel_1.y,
        .accelY_2 = accel_2.y,
        .accelY_3 = accel_3.y,
        .accelZ_1 = accel_1.z,
        .accelZ_2 = accel_2.z,
        .accelZ_3 = accel_3.z,
        .gyrolX_1 = gyro_1.x,
        .gyrolX_2 = gyro_2.x,
        .gyrolY_1 = gyro_1.y,
        .gyrolY_2 = gyro_2.y,
        .gyrolZ_1 = gyro_1.z,
        .gyrolZ_2 = gyro_2.z,
        .magX_1 = mag.x,
        .magY_1 = mag.y,
        .magZ_1 = mag.z,
        .lat = s_LastGPSPos.x,
        .lon = s_LastGPSPos.y,
        .alt = s_LastGPSPos.z,
        .pressure = baroData.pressure,
        .temperature = baroData.temperature,
    };
}

BOOL checkGPS(VOID)
{
    DRIVER_CALL(uBloxNeoM9NReadData(&s_UBloxNeoM9NConfig, &s_UBloxNeoM9NData));

    if (s_UBloxNeoM9NData.isFinishedSentence)
    {
        if (nmeaCheckSentence(s_UBloxNeoM9NData.sentence))
        {
            NMEASentence s = nmeaGetSentenceId(s_UBloxNeoM9NData.sentence);

            if (s == NMEA_SENTENCE_GGA)
            {
                NMEASentence_GGA frame = {0};
                nmeaParse_GGA(s_UBloxNeoM9NData.sentence, &frame);

                s_LastGPSPos.x = frame.lat;
                s_LastGPSPos.y = frame.lon;
                s_LastGPSPos.z = frame.alt;

                return TRUE;
            }
            else if (s == NMEA_SENTENCE_GLL)
            {
                NMEASentence_GLL frame = {0};
                nmeaParse_GLL(s_UBloxNeoM9NData.sentence, &frame);

                s_LastGPSPos.x = frame.lat;
                s_LastGPSPos.y = frame.lon;

                return TRUE;
            }
            else if (s == NMEA_SENTENCE_GNS)
            {
                NMEASentence_GNS frame = {0};
                nmeaParse_GNS(s_UBloxNeoM9NData.sentence, &frame);

                s_LastGPSPos.x = frame.lat;
                s_LastGPSPos.y = frame.lon;
                s_LastGPSPos.z = frame.alt;

                return TRUE;
            }
        }
    }

    return FALSE;
}