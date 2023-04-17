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

    DRIVER_CALL(bmp280Init(&s_BarometerConfig, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));
    DRIVER_CALL(mpu6050Init(&s_AccelerometerConfig, MPU6050_I2C, MPU6050_I2C_SDA_PIN, MPU6050_I2C_SCL_PIN, MPU6050_LP_FILTER_LEVEL, MPU6050_ACCEL_SENS_LEVEL, MPU6050_GYRO_SENS_LEVEL));

    MY_LOG_CORE_INFO("Sensors initialized!");
}

VOID takeMeasurements(MeasurementData *data_out)
{
    *data_out = (MeasurementData){0};
}