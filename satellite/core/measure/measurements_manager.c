#include "measurements_manager.h"
#include "measurement_logger.h"
#include "pinout.h"
#include "config.h"

void initializeMeasurements()
{
    if (ENABLE_BAROMETER)
    {
        barometerInit(BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN);
    }

    if (ENABLE_ACCELEROMETER)
    {
        accelerometerInit(MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL);
    }

    if (ENABLE_GPS)
    {
    }
}

void takeMeasurements(measurement_data_t *data_out)
{
    barometer_data_t barometerData = {0};
    accelerometer_data_t accelerometerData = {0};
    gps_data_t gpsData = {0};

    if (ENABLE_BAROMETER)
    {
        barometerRead(&barometerData);
    }

    if (ENABLE_ACCELEROMETER)
    {
        accelerometer_raw_data_t mpu6050RawData = {0};
        accelerometerReadRaw(&mpu6050RawData);
        accelerometerConvertData(&mpu6050RawData, &accelerometerData);
    }

    if (ENABLE_GPS)
    {
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

    *data_out = (measurement_data_t){
        .barometerData = barometerData,
        .accelerometerData = accelerometerData,
        .gpsData = gpsData,
        .componentsStatus = (ENABLE_BAROMETER ? COMPONENT_BAROMETER : 0) | (ENABLE_ACCELEROMETER ? COMPONENT_ACCELEROMETER : 0) | (ENABLE_GPS ? COMPONENT_GPS : 0),
    };
}