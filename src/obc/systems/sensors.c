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
#include "lib/drivers/barometer/ms5611_driver.h"
#include "lib/drivers/gps/gps_driver.h"
#include "lib/drivers/adc/ads7038_driver.h"
#include "lib/drivers/led/w2812_driver.h"
#include "lib/battery/battery_utils.h"

#define SYSTEM_NAME "sensors"
#define BATTERY_VOLTAGE_DIVIDER 11.0f
#define EXP_FILTER_IGN_COEFF 0.2f
#define EXP_FILTER_BAT_COEFF 0.3f

// REF: https://blog.ampow.com/lipo-voltage-chart/
static battery_table_entry_t s_BatteryTable[] = {
    {4.20f, 100},
    {4.15f, 95},
    {4.11f, 90},
    {4.08f, 85},
    {4.02f, 80},
    {3.98f, 75},
    {3.95f, 70},
    {3.91f, 65},
    {3.87f, 60},
    {3.85f, 55},
    {3.84f, 50},
    {3.82f, 45},
    {3.80f, 40},
    {3.79f, 35},
    {3.77f, 30},
    {3.75f, 25},
    {3.73f, 20},
    {3.71f, 15},
    {3.69f, 10},
    {3.61f, 5},
    {3.27f, 0},
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
static ms5611_config_t s_MS5611Config;
static gps_config_t s_GPSConfig;
static ads7038_config_t s_ADS7038Config;
static battery_config_t s_BatteryConfig;

static float _adc_exp_filter(float x1, float x0, float a);
static float _adc_corrected(float x);

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
    h3lis331dl_set_range(&s_H3LIS331DLConfig, H3LIS331DL_RANGE_200G);
    h3lis331dl_set_odr(&s_H3LIS331DLConfig, H3LIS331DL_ODR_400HZ);

    SYS_LOG("H3LIS: READY");

    mmc5983ma_init_spi(&s_MMC5983MAConfig, OBC_SPI, PIN_CS_MMC);
    mmc5983ma_set_continuous_mode_odr(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ, MMC5983MA_PRD_SET_250);

    SYS_LOG("MMC5983MA: READY");

    ms5611_init_spi(&s_MS5611Config, OBC_SPI, PIN_CS_MS56);
    ms5611_set_osr(&s_MS5611Config, MS5611_OSR_256, MS5611_OSR_256);

    SYS_LOG("MS5611: READY");

    gps_init_spi(&s_GPSConfig, OBC_SPI, PIN_CS_NEO);

    SYS_LOG("GPS: READY");

    ads7038_init(&s_ADS7038Config, OBC_SPI, PIN_CS_ADC, 1 << ADC_IGN_1_DET_CH | 1 << ADC_IGN_2_DET_CH | 1 << ADC_IGN_3_DET_CH | 1 << ADC_IGN_4_DET_CH, ADC_VREF);

    SYS_LOG("ADS7038: READY");

    hal_adc_init_pin(PIN_BATTERY);
    battery_init(&s_BatteryConfig, s_BatteryTable, sizeof(s_BatteryTable) / sizeof(battery_table_entry_t));

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

    if (ms5611_read_non_blocking(&s_MS5611Config, &s_Frame.press, &s_Frame.temp))
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

    if (hal_time_run_every_ms(100, &s_ADCMeasurementTimeOffset))
    {
        float channels[4];
        ads7038_read_channels(&s_ADS7038Config, channels, sizeof(channels) / sizeof(float));

        s_Frame.ignDet1Volts = _adc_exp_filter(channels[0] - 0.015f, s_Frame.ignDet1Volts, EXP_FILTER_IGN_COEFF);
        s_Frame.ignDet2Volts = _adc_exp_filter(channels[1] - 0.023f, s_Frame.ignDet2Volts, EXP_FILTER_IGN_COEFF);
        s_Frame.ignDet3Volts = _adc_exp_filter(channels[2] - 0.048f, s_Frame.ignDet3Volts, EXP_FILTER_IGN_COEFF);
        s_Frame.ignDet4Volts = _adc_exp_filter(channels[3] - 0.010f, s_Frame.ignDet4Volts, EXP_FILTER_IGN_COEFF);

        float rawBatVolts = _adc_corrected(hal_adc_read_voltage(PIN_BATTERY) * BATTERY_VOLTAGE_DIVIDER);
        s_Frame.batVolts = _adc_exp_filter(rawBatVolts, s_Frame.batVolts, EXP_FILTER_BAT_COEFF);

        battery_data_t data = {};
        battery_convert(&s_BatteryConfig, s_Frame.batVolts, &data);

        s_Frame.batPercent = data.percentage;
        s_Frame.batNCells = data.nCells;

        events_publish(MSG_SENSORS_ADC_READ);
    }
}

const sensors_frame_t *sensors_get_frame(void)
{
    return &s_Frame;
}

static float _adc_exp_filter(float x1, float x0, float a)
{
    return x0 == 0 ? x1 : a * x1 + (1 - a) * x0;
}

static float _adc_corrected(float x)
{
    float offset = -0.08f * x + 0.74f;

    return x - offset;
}