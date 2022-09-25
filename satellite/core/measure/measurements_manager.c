#include "measurements_manager.h"
#include "pinout.h"
#include "config.h"
#include "kernel/logging/logger.h"
#include "shared/functions_utils.h"

static Logger s_MeasureLogger;
static BMP280Config s_BarometerConfig;
static MPU6050Config s_AccelerometerConfig;

#define MY_LOG_MEASURE_NAME "MEASURE_LOG"
#define MY_LOG_MEASURE_PATTERN "%c;"
#define MY_LOG_MEASURE_INT(value) myLog(&s_MeasureLogger, "", "%d", value)
#define MY_LOG_MEASURE_FLOAT(value) myLog(&s_MeasureLogger, "", "%f", value)
#define MY_LOG_MEASURE_END() myLog(&s_MeasureLogger, "", "\n")

VOID initializeMeasurements()
{
    myLogCreateLogger(&s_MeasureLogger, MY_LOG_MEASURE_NAME);
    myLogCreateFileSink(&s_MeasureLogger, MY_LOG_MEASURE_PATTERN, MEASUREMENTS_FILE_INDEX);

    if (ENABLE_BAROMETER)
    {
        FUNC_CALL(bmp280Init(&s_BarometerConfig, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));
    }

    if (ENABLE_ACCELEROMETER)
    {
        FUNC_CALL(mpu6050Init(&s_AccelerometerConfig, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL));
    }

    if (ENABLE_GPS)
    {
    }
}

VOID takeMeasurements(MeasurementData *data_out)
{
    BMP280Data barometerData = {0};
    MPU6050Data accelerometerData = {0};
    INT32 componentsStatus = 0;

    if (ENABLE_BAROMETER)
    {
        FUNC_CALL(bmp280Read(&s_BarometerConfig, &barometerData));

        componentsStatus |= COMPONENT_BAROMETER;
    }

    if (ENABLE_ACCELEROMETER)
    {
        MPU6050RawData mpu6050RawData = {0};
        FUNC_CALL(mpu6050ReadRaw(&s_AccelerometerConfig, &mpu6050RawData));
        FUNC_CALL(mpu6050ConvertData(&s_AccelerometerConfig, &mpu6050RawData, &accelerometerData));

        componentsStatus |= COMPONENT_ACCELEROMETER;
    }

    MY_LOG_MEASURE_INT(barometerData.pressure);
    MY_LOG_MEASURE_FLOAT(barometerData.temperature);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_x);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_y);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_z);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_x);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_y);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_z);
    MY_LOG_MEASURE_END();

    *data_out = (MeasurementData){
        .barometerData = barometerData,
        .accelerometerData = accelerometerData,
        .componentsStatus = componentsStatus,
    };
}