#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/barometer/bme688_driver.h"
#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include "ahrs/madgwick_filter.h"
#include "ins/ins_kf.h"
#include "maths/constants.h"
#include "maths/math_utils.h"
#include <stdio.h>

#define DT 0.01f
#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define ACCEL_CS 11
#define GYRO_CS 10
#define BME_CS 4
#define MMC_CS 12

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 10 * 1000 * 1000);

    MadgiwckFilterData madgwickFilterData;
    INSKalmanFilterState kalmanFilterState = {0};
    INSKalmanFilterConfig kalmanFilterConfig = {
        .dt = DT,
        .accelVariance = 0.2f,
        .gpsPosVariance = 10000.0f,
        .pressureVariance = 0.2f,
        .temperatureVariance = 0.2f,
    };
    INSKalmanFilterInputData kalmanFilterInputData = {0};
    INSKalmanFilterOutputData kalmanFilterOutputData = {0};
    BMI088AccelConfig accelConfig;
    BMI088GyroConfig gyroConfig;
    BME688Config bme688Config = {0};
    MMC5983MAConfig mmc5983maConfig = {0};

    madgwickInit(&madgwickFilterData, 5.0f, DT);
    insKalmanFilterInit(&kalmanFilterState, &kalmanFilterConfig);

    bmi088AccelInitSPI(&accelConfig, SPI, MISO, MOSI, ACCEL_CS, SCK);
    bmi088AccelSetConf(&accelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088AccelSetRange(&accelConfig, BMI088_ACCEL_RANGE_6G);

    bmi088GyroInitSPI(&gyroConfig, SPI, MISO, MOSI, GYRO_CS, SCK);
    bmi088GyroSetBandwidth(&gyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088GyroSetRange(&gyroConfig, BMI088_GYRO_RANGE_250DPS);

    mmc5983maInitSPI(&mmc5983maConfig, SPI, MISO, MOSI, MMC_CS, SCK);
    mmc5983maSetContinuousModeODR(&mmc5983maConfig, MMC5983MA_ODR_200HZ);

    bme688Init(&bme688Config, SPI, MISO, MOSI, SCK, BME_CS);
    bme688SetConfig(&bme688Config, BME688_SENSOR_OSR_8X, BME688_SENSOR_OSR_8X, BME688_SENSOR_OSR_16X, BME688_IIR_FILTER_COEFF_OFF);
    bme688SetHeaterConfig(&bme688Config, 0, 0, 300, 0, 100);
    bme688SetMode(&bme688Config, BME688_MODE_FORCED);

    sleepMiliseconds(100);

    while (TRUE)
    {
        vec3 accel;
        vec3 gyro;
        vec3 mag = {0};
        vec3 eulerData;
        BME688Data data = {0};

        bmi088AccelRead(&accelConfig, &accel);
        bmi088GyroRead(&gyroConfig, &gyro);
        mmc5983maRead(&mmc5983maConfig, &mag);
        bme688Read(&bme688Config, &data);

        madgwickUpdateMARG(&madgwickFilterData, gyro, accel, mag);
        quatToEuler(&eulerData, &madgwickFilterData.q);

        kalmanFilterInputData.acc = accel;
        kalmanFilterInputData.gpsPos = (vec3){0};
        kalmanFilterInputData.orientation = madgwickFilterData.q;
        kalmanFilterInputData.pressure = data.pressure;
        kalmanFilterInputData.temperature = data.temperature;

        insKalmanFilterUpdate(&kalmanFilterState, &kalmanFilterInputData, &kalmanFilterOutputData);

        printf("X: %f, Y: %f, Z: %f\n", eulerData.x, eulerData.y, eulerData.z);
        // printf("X: %f, Y: %f, Z: %f\n", kalmanFilterOutputData.pos.x, kalmanFilterOutputData.pos.y, kalmanFilterOutputData.pos.z);

        bme688SetMode(&bme688Config, BME688_MODE_FORCED);

        sleepMiliseconds(DT * 1000.0f);
    }

    return 0;
}