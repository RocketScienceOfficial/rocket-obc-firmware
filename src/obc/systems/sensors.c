#include "sensors.h"
#include "board_config.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/adc_driver.h"
#include "hal/time_tracker.h"
#include "hal/serial_driver.h"
#include "lib/geo/geo_utils.h"
#include "lib/drivers/imu/bmi088_driver.h"
#include "lib/drivers/imu/lsm6dso32_driver.h"
#include "lib/drivers/accelerometer/h3lis331dl_driver.h"
#include "lib/drivers/magnetometer/mmc5983ma_driver.h"
#include "lib/drivers/barometer/ms5607_driver.h"
#include "lib/drivers/gps/gps_driver.h"
#include "lib/drivers/adc/ads7038_driver.h"
#include "lib/drivers/led/w2812_driver.h"
#include "lib/battery/battery_utils.h"

#define SYSTEM_NAME "sensors"

#define BATTERY_VOLTAGE_DIVIDER 11.001f

static battery_table_entry_t s_BatteryTable[] = {
    {4.2f, 100},
    {3.0f, 0},
};

static usec_t s_MeasurementTimeOffset;
static msec_t s_GPSTimeOffset;
static msec_t s_ADCMeasurementTimeOffset;

static sensors_frame_t s_Frame;

static bmi088_accel_config_t s_BMI088AccelConfig;
static bmi088_gyro_config_t s_BMI088GyroConfig;
static lsm6dso32_config_t s_LSM6DSO32Config;
static h3lis331dl_config_t s_H3LIS331DLConfig;
static mmc5983ma_config_t s_MMC5983MAConfig;
static ms5607_config_t s_MS5607Config;
static gps_config_t s_GPSConfig;
static ads7038_config_t s_ADS7038Config;
static battery_config_t s_BatteryConfig;

void sensors_init(void)
{
    hal_spi_init_all(OBC_SPI, OBC_SPI_MISO_PIN, OBC_SPI_MOSI_PIN, OBC_SPI_SCK_PIN, OBC_SPI_FREQ);
    hal_adc_init_all();

    SYS_LOG("Protocols: READY");

    bmi088_accel_init_spi(&s_BMI088AccelConfig, OBC_SPI, PIN_CS_BMI_ACC);
    bmi088_accel_set_conf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088_accel_set_range(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_6G);

    SYS_LOG("BMI088 Accel: READY");

    bmi088_gyro_init_spi(&s_BMI088GyroConfig, OBC_SPI, PIN_CS_BMI_GYRO);
    bmi088_gyro_set_bandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088_gyro_set_range(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS);

    SYS_LOG("BMI088 Gyro: READY");

    lsm6dso32_init_spi(&s_LSM6DSO32Config, OBC_SPI, PIN_CS_LSM);
    lsm6dso32_set_odr(&s_LSM6DSO32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ);
    lsm6dso32_set_range(&s_LSM6DSO32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS);

    SYS_LOG("LSM: READY");

    h3lis331dl_init_spi(&s_H3LIS331DLConfig, OBC_SPI, PIN_CS_H3LIS);
    h3lis331dl_set_power_mode(&s_H3LIS331DLConfig, H3LIS331DL_POWER_NORMAL);
    h3lis331dl_set_range(&s_H3LIS331DLConfig, H3LIS331DL_RANGE_100G);
    h3lis331dl_set_odr(&s_H3LIS331DLConfig, H3LIS331DL_ODR_400HZ);

    SYS_LOG("H3LIS: READY");

    mmc5983ma_init_spi(&s_MMC5983MAConfig, OBC_SPI, PIN_CS_MMC);
    mmc5983ma_set_continuous_mode_odr(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ);

    SYS_LOG("MMC5983MA: READY");

    ms5607_init_spi(&s_MS5607Config, OBC_SPI, PIN_CS_MS56);
    ms5607_set_osr(&s_MS5607Config, MS5607_OSR_256, MS5607_OSR_256);

    SYS_LOG("MS5607: READY");

    gps_init_spi(&s_GPSConfig, OBC_SPI, PIN_CS_NEO);

    SYS_LOG("GPS: READY");

    ads7038_init(&s_ADS7038Config, OBC_SPI, PIN_CS_ADC, 1 << ADC_IGN_1_DET_CH | 1 << ADC_IGN_2_DET_CH | 1 << ADC_IGN_3_DET_CH | 1 << ADC_IGN_4_DET_CH, ADC_VREF);

    SYS_LOG("ADS7038: READY");

    hal_adc_init_pin(PIN_BATTERY);
    battery_init(&s_BatteryConfig, s_BatteryTable, sizeof(s_BatteryTable) / sizeof(battery_table_entry_t), BATTERY_VOLTAGE_DIVIDER);

    SYS_LOG("Battery: READY");

    SYS_LOG("READY");
}

void sensors_update(void)
{
    if (hal_time_run_every_us(2500, &s_MeasurementTimeOffset))
    {
        bmi088_accel_read(&s_BMI088AccelConfig, &s_Frame.acc1);
        bmi088_gyro_read(&s_BMI088GyroConfig, &s_Frame.gyro1);
        lsm6dso32_read(&s_LSM6DSO32Config, &s_Frame.acc2, &s_Frame.gyro2, NULL);
        h3lis331dl_read(&s_H3LIS331DLConfig, &s_Frame.acc3);
        mmc5983ma_read(&s_MMC5983MAConfig, &s_Frame.mag1);

        events_publish(MSG_SENSORS_NORMAL_READ);
    }

    if (ms5607_read_non_blocking(&s_MS5607Config, &s_Frame.press, &s_Frame.temp))
    {
        s_Frame.baroHeight = height_from_baro_formula(s_Frame.press);

        events_publish(MSG_SENSORS_BARO_READ);
    }

    if (hal_time_get_ms_since_boot() - s_GPSTimeOffset >= 10)
    {
        bool avail = gps_read(&s_GPSConfig);

        if (!avail)
        {
            s_GPSTimeOffset = hal_time_get_ms_since_boot();
        }

        if (gps_get_pos(&s_GPSConfig, &s_Frame.pos) && s_Frame.pos.lat != 0)
        {
            if (!s_Frame.gpsFix)
            {
                SYS_LOG("GPS has fix!");

                s_Frame.gpsFix = true;
            }

            events_publish(MSG_SENSORS_GPS_READ);
        }
    }

    if (hal_time_run_every_ms(5000, &s_ADCMeasurementTimeOffset))
    {
        float channels[4];
        ads7038_read_channels(&s_ADS7038Config, channels, sizeof(channels) / sizeof(float));

        s_Frame.ignDet1Volts = channels[0];
        s_Frame.ignDet2Volts = channels[1];
        s_Frame.ignDet3Volts = channels[2];
        s_Frame.ignDet4Volts = channels[3];

        battery_data_t data = {};
        battery_convert(&s_BatteryConfig, hal_adc_read_voltage(PIN_BATTERY), &data);

        s_Frame.batVolts = data.voltage;
        s_Frame.batPercent = data.percentage;
        s_Frame.batNCells = data.nCells;

        events_publish(MSG_SENSORS_ADC_READ);
    }
}

const sensors_frame_t *sensors_get_frame(void)
{
    return &s_Frame;
}