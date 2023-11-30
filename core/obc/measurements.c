#include "measurements.h"
#include "pinout.h"

#include "modules/drivers/accelerometer/bmi088_driver.h"
#include "modules/drivers/accelerometer/lsm6dso32_driver.h"
#include "modules/drivers/accelerometer/h3lis331dl_driver.h"
#include "modules/drivers/magnetometer/mmc5983ma_driver.h"
#include "modules/drivers/barometer/bmp581_driver.h"
#include "modules/drivers/gps/ublox_sam_m10q_driver.h"
#include "modules/nmea/nmea_wrapper.h"

static bmi088_accel_config_t s_BMI088AccelConfig;
static bmi088_gyro_config_t s_BMI088GyroConfig;
static lsm6dso32_config_t s_LSM6DSO32Config;
static h3lis331dl_config_t s_H3LIS331DLConfig;
static mmc5983ma_config_t s_MMC5983MAConfig;
static bmp581_config_t s_BMP581Config;
static ublox_sam_m10q_config_t s_UBloxSamM10QConfig;
static ublox_sam_m10q_data_t s_UBloxSamM10QData;
static gnss_data_t s_GNSSData;

void measurements_init(void)
{
    bmi088_accel_init_i2c(&s_BMI088AccelConfig, I2C, SDA, SCL, false);
    bmi088_accel_set_conf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088_accel_set_range(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_6G);

    bmi088_gyro_init_i2c(&s_BMI088GyroConfig, I2C, SDA, SCL, false);
    bmi088_gyro_set_bandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088_gyro_set_range(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS);

    lsm6dso32_init_i2c(&s_LSM6DSO32Config, I2C, SDA, SCL);
    lsm6dso32_set_odr(&s_LSM6DSO32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ);
    lsm6dso32_set_range(&s_LSM6DSO32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS);

    h3lis331dl_init_i2c(&s_H3LIS331DLConfig, I2C, SDA, SCL);
    h3lis331dl_set_power_mode(&s_H3LIS331DLConfig, H3LIS331DL_POWER_NORMAL);
    h3lis331dl_set_range(&s_H3LIS331DLConfig, H3LIS331DL_RANGE_100G);
    h3lis331dl_set_odr(&s_H3LIS331DLConfig, H3LIS331DL_ODR_400HZ);

    mmc5983ma_init_i2c(&s_MMC5983MAConfig, I2C, SDA, SCL);
    mmc5983ma_set_continuous_mode_odr(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ);

    bmp581_init_i2c(&s_BMP581Config, I2C, SDA, SCL);
    bmp581_set_mode(&s_BMP581Config, BMP581_MODE_CONTINUOUS);
    bmp581_set_odr_osr(&s_BMP581Config, BMP581_OSR_1, BMP581_OSR_1, BMP581_ODR_240);

    ublox_sam_m10q_init_i2c(&s_UBloxSamM10QConfig, I2C, SDA, SCL);
}

void measurements_get(measurement_data_t *data)
{
    bmi088_accel_read(&s_BMI088AccelConfig, &data->acc1);
    bmi088_gyro_read(&s_BMI088GyroConfig, &data->gyro1);

    lsm6dso32_read(&s_LSM6DSO32Config, &data->acc2, &data->gyro2, NULL);

    h3lis331dl_read(&s_H3LIS331DLConfig, &data->acc3);

    mmc5983ma_read(&s_MMC5983MAConfig, &data->mag);

    bmp581_data_t bmpData;
    bmp581_read(&s_BMP581Config, &bmpData);

    data->press = bmpData.pressure;
    data->temp = bmpData.temperature;

    ublox_sam_m10q_read_data(&s_UBloxSamM10QConfig, &s_UBloxSamM10QData);
    gnss_update(&s_GNSSData, &s_UBloxSamM10QData.sentence);

    data->lat = s_GNSSData.lat;
    data->lon = s_GNSSData.lon;
    data->alt = s_GNSSData.alt;
}