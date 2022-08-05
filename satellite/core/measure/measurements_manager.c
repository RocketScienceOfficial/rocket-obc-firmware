#include "measurements_manager.h"
#include "logger.h"
#include "pinout.h"
#include "config.h"
#include "hardware_utils.h"

static Logger s_MeasureLogger;
static BarometerConfig s_BarometerConfig;
static AccelerometerConfig s_AccelerometerConfig;

#define MY_LOG_MEASURE_NAME "MEASURE_LOG"
#define MY_LOG_MEASURE_PATTERN "%c;"
#define MY_LOG_MEASURE_FILE_NAME "measurements.csv"
#define MY_LOG_MEASURE_INT(value) myLog(&s_MeasureLogger, "", "%d", value)
#define MY_LOG_MEASURE_FLOAT(value) myLog(&s_MeasureLogger, "", "%f", value)
#define MY_LOG_MEASURE_END() myLog(&s_MeasureLogger, "", "\n")

void initializeMeasurements()
{
    myLogCreateLogger(&s_MeasureLogger, MY_LOG_MEASURE_NAME);
    myLogCreateFileSink(&s_MeasureLogger, MY_LOG_MEASURE_PATTERN, MY_LOG_MEASURE_FILE_NAME);

    if (ENABLE_BAROMETER)
    {
        HW_CALL(barometerInit(&s_BarometerConfig, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));
    }

    if (ENABLE_ACCELEROMETER)
    {
        HW_CALL(accelerometerInit(&s_AccelerometerConfig, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL));
    }

    if (ENABLE_GPS)
    {
    }
}

void takeMeasurements(MeasurementData *data_out)
{
    BarometerData barometerData = {0};
    AccelerometerData accelerometerData = {0};
    GPSData gpsData = {0};
    int componentsStatus = 0;

    if (ENABLE_BAROMETER)
    {
        HW_CALL(barometerRead(&s_BarometerConfig, &barometerData));

        componentsStatus |= COMPONENT_BAROMETER;
    }

    if (ENABLE_ACCELEROMETER)
    {
        AccelerometerRawData mpu6050RawData = {0};
        HW_CALL(accelerometerReadRaw(&s_AccelerometerConfig, &mpu6050RawData));
        HW_CALL(accelerometerConvertData(&s_AccelerometerConfig, &mpu6050RawData, &accelerometerData));

        componentsStatus |= COMPONENT_ACCELEROMETER;
    }

    if (ENABLE_GPS)
    {
        componentsStatus |= COMPONENT_GPS;
    }

    MY_LOG_MEASURE_INT(barometerData.pressure);
    MY_LOG_MEASURE_FLOAT(barometerData.temperature);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_x);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_y);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_z);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_x);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_y);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_z);
    MY_LOG_MEASURE_FLOAT(gpsData.latitude);
    MY_LOG_MEASURE_FLOAT(gpsData.longitude);
    MY_LOG_MEASURE_FLOAT(gpsData.altitude);
    MY_LOG_MEASURE_END();

    *data_out = (MeasurementData){
        .barometerData = barometerData,
        .accelerometerData = accelerometerData,
        .gpsData = gpsData,
        .componentsStatus = componentsStatus,
    };
}