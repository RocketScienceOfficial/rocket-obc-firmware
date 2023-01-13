#include "measurements_manager.h"
#include "pinout.h"
#include "config.h"
#include "drivers/accelerometer/mpu6050_driver.h"
#include "drivers/barometer/bmp280_driver.h"
#include "kernel/logging/logger.h"
#include "kernel/services/driver_calling.h"

static Logger s_MeasureLogger;
static BMP280Config s_BarometerConfig;
static MPU6050Config s_AccelerometerConfig;

#define MY_LOG_MEASURE_NAME "MEASURE_LOG"
#define MY_LOG_MEASURE_PATTERN "%c;"
#define MY_LOG_MEASURE_INTERNAL(value, format) myLog(&s_MeasureLogger, "", format, value)
#define MY_LOG_MEASURE_INT(value) MY_LOG_MEASURE_INTERNAL(value, "%d")
#define MY_LOG_MEASURE_FLOAT(value) MY_LOG_MEASURE_INTERNAL(value, "%f")
#define MY_LOG_MEASURE_END() myLog(&s_MeasureLogger, "", "\n")

VOID initializeMeasurements()
{
    myLogCreateLogger(&s_MeasureLogger, MY_LOG_MEASURE_NAME);
    myLogCreateFileSink(&s_MeasureLogger, MY_LOG_MEASURE_PATTERN, MEASUREMENTS_FILE_INDEX);

    DRIVER_CALL(bmp280Init(&s_BarometerConfig, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));
    DRIVER_CALL(mpu6050Init(&s_AccelerometerConfig, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL));

    MY_LOG_CORE_INFO("Sensors initialized!");
}

VOID takeMeasurements(MeasurementData *data_out)
{
    BMP280Data barometerData = {0};
    MPU6050Data accelerometerData = {0};

    DRIVER_CALL(bmp280Read(&s_BarometerConfig, &barometerData));

    MPU6050RawData mpu6050RawData = {0};

    DRIVER_CALL(mpu6050ReadRaw(&s_AccelerometerConfig, &mpu6050RawData));
    DRIVER_CALL(mpu6050ConvertData(&s_AccelerometerConfig, &mpu6050RawData, &accelerometerData));

    MY_LOG_MEASURE_INT(barometerData.pressure);
    MY_LOG_MEASURE_FLOAT(barometerData.temperature);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_x);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_y);
    MY_LOG_MEASURE_FLOAT(accelerometerData.accel_z);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_x);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_y);
    MY_LOG_MEASURE_FLOAT(accelerometerData.rot_z);
    MY_LOG_MEASURE_END();

    *data_out = (MeasurementData){0};
}