#include "hal/board_control.h"
#include "hal/time_tracker.h"
#include "hal/multicore.h"
#include "hal/serial_driver.h"
#include "hal/spi_driver.h"
#include "hal/i2c_driver.h"
#include "hal/adc_driver.h"
#include "lib/drivers/imu/bmi088_driver.h"
#include "lib/drivers/imu/lsm6dso32_driver.h"
#include "lib/drivers/accelerometer/h3lis331dl_driver.h"
#include "lib/drivers/magnetometer/mmc5983ma_driver.h"
#include "lib/drivers/barometer/ms5607_driver.h"
#include "lib/drivers/gps/gps_driver.h"
#include "lib/drivers/adc/ads7038_driver.h"
#include "lib/drivers/led/w2812_driver.h"
#include "lib/battery/battery_utils.h"
#include "modules/sm/sm_control.h"
#include "modules/ign/ign_controller.h"
#include "modules/database/dataman.h"
#include "modules/ekf/ekf.h"
#include "board_config.h"

typedef struct adc_data
{
    hal_voltage_level_t ignDet1;
    hal_voltage_level_t ignDet2;
    hal_voltage_level_t ignDet3;
    hal_voltage_level_t ignDet4;
    hal_voltage_level_t batVolts;
    float batPercent;
} adc_data_t;

#define MAIN_PARACHUTE_HEIGHT 1000
#define IGN_SECOND_DELAY 0

static battery_interval_t s_BatteryIntervals[] = {
    {0.5454f, 0.7636f, 0, 100},
};

static sm_data_t s_SM;
static ign_data_t s_IGN;
static adc_data_t s_CurrentADCData;
static usec_t s_MeasurementTimeOffset;
static usec_t s_ADCMeasurementTimeOffset;
static usec_t s_DiodesTimeOffset;
static dataman_sensor_frame_t s_CurrentFrame;
static bmi088_accel_config_t s_BMI088AccelConfig;
static bmi088_gyro_config_t s_BMI088GyroConfig;
static lsm6dso32_config_t s_LSM6DSO32Config;
static h3lis331dl_config_t s_H3LIS331DLConfig;
static mmc5983ma_config_t s_MMC5983MAConfig;
static ms5607_config_t s_MS5607Config;
static gps_config_t s_GPSConfig;
static geo_position_wgs84_t s_GPSPosition;
static ads7038_config_t s_ADS7038Config;
static battery_config_t s_BatteryConfig;
static ekf_data_t s_EKFData;

static void _core1_entry(void)
{
    ekf_config_t config = {
        .dt = 0.01f,
        .accelVariance = 0.5f,
        .gyroVariance = 0.5f,
        .gpsVariance = 0.5f,
        .baroVariance = 0.5f,
        .magVariance = 0.5f,
    };
    ekf_state_t startingState = {
        .orientation = {1, 0, 0, 0},
        .velocity_ned = {0},
        .position_ned = {0},
        .gyroBias = {0},
        .accelBias = {0},
        .mag_ned = {0},
        .magBias = {0},
    };

    ekf_init(&s_EKFData, config, startingState);

    while (true)
    {
        ekf_predict_state(&s_EKFData, NULL);
        ekf_predict_covariance(&s_EKFData, NULL);
        ekf_fuse(&s_EKFData, NULL);
    }
}

static void _init_board(void)
{
    hal_board_init(5000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Hardware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(OBC_SPI, OBC_SPI_MISO_PIN, OBC_SPI_MOSI_PIN, OBC_SPI_SCK_PIN, OBC_SPI_FREQ);
    hal_adc_init_all();

    hal_core_start_next(&_core1_entry);
}

static void _init_drivers(void)
{
    bmi088_accel_init_spi(&s_BMI088AccelConfig, OBC_SPI, PIN_CS_BMI_ACC);
    bmi088_accel_set_conf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
    bmi088_accel_set_range(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_6G);

    bmi088_gyro_init_spi(&s_BMI088GyroConfig, OBC_SPI, PIN_CS_BMI_GYRO);
    bmi088_gyro_set_bandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
    bmi088_gyro_set_range(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS);

    lsm6dso32_init_spi(&s_LSM6DSO32Config, OBC_SPI, PIN_CS_LSM);
    lsm6dso32_set_odr(&s_LSM6DSO32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ);
    lsm6dso32_set_range(&s_LSM6DSO32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS);

    h3lis331dl_init_spi(&s_H3LIS331DLConfig, OBC_SPI, PIN_CS_H3LIS);
    h3lis331dl_set_power_mode(&s_H3LIS331DLConfig, H3LIS331DL_POWER_NORMAL);
    h3lis331dl_set_range(&s_H3LIS331DLConfig, H3LIS331DL_RANGE_100G);
    h3lis331dl_set_odr(&s_H3LIS331DLConfig, H3LIS331DL_ODR_400HZ);

    mmc5983ma_init_spi(&s_MMC5983MAConfig, OBC_SPI, PIN_CS_MMC);
    mmc5983ma_set_continuous_mode_odr(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ);

    ms5607_init_spi(&s_MS5607Config, OBC_SPI, PIN_CS_MS56);
    ms5607_set_osr(&s_MS5607Config, MS5607_OSR_256, MS5607_OSR_256);

    gps_init_spi(&s_GPSConfig, OBC_SPI, PIN_CS_NEO);

    ads7038_init(&s_ADS7038Config, OBC_SPI, PIN_CS_ADC, 1 << ADC_IGN_1_DET_CH | 1 << ADC_IGN_2_DET_CH | 1 << ADC_IGN_3_DET_CH | 1 << ADC_IGN_4_DET_CH, ADC_VREF);

    hal_adc_init_pin(hal_adc_convert_pin_to_input(PIN_BATTERY));

    battery_init(&s_BatteryConfig, s_BatteryIntervals, sizeof(s_BatteryIntervals) / sizeof(battery_interval_t));

    ws2812_init(PIN_LED, false);
}

static void _init_sytems(void)
{
    sm_init(&s_SM);

    ign_pins_t ignPins = {
        .main = PIN_IGN_1,
        .drouge = PIN_IGN_2,
        .sep = PIN_IGN_3,
        .second = PIN_IGN_4,
    };
    ign_settings_t ignSettings = {
        .vref = ADC_VREF,
        .mainAlt = MAIN_PARACHUTE_HEIGHT,
        .secondIgnDelay = IGN_SECOND_DELAY,
    };

    ign_init(&s_IGN, &ignPins, &ignSettings);
    ign_arm(&s_IGN);

    dataman_clear();

    hal_serial_printf("Initialized systems!\n");
}

static bool _get_measurements(void)
{
    usec_t rate = s_SM.state == FLIGHT_STATE_STANDING || s_SM.state == FLIGHT_STATE_LANDED         ? 20000
                  : s_SM.state == FLIGHT_STATE_FREE_FLIGHT || s_SM.state == FLIGHT_STATE_FREE_FALL ? 10000
                                                                                                   : 2500;
    if (hal_time_run_every_us(rate, &s_MeasurementTimeOffset))
    {
        bmi088_accel_read(&s_BMI088AccelConfig, &s_CurrentFrame.acc1);
        bmi088_gyro_read(&s_BMI088GyroConfig, &s_CurrentFrame.gyro1);

        lsm6dso32_read(&s_LSM6DSO32Config, &s_CurrentFrame.acc2, &s_CurrentFrame.gyro2, NULL);

        h3lis331dl_read(&s_H3LIS331DLConfig, &s_CurrentFrame.acc3);

        mmc5983ma_read(&s_MMC5983MAConfig, &s_CurrentFrame.mag1);

        ms5607_read(&s_MS5607Config, &s_CurrentFrame.press, &s_CurrentFrame.temp);

        gps_read(&s_GPSConfig);
        gps_get_pos(&s_GPSConfig, &s_GPSPosition);

        s_CurrentFrame.lat = s_GPSPosition.lat;
        s_CurrentFrame.lon = s_GPSPosition.lon;
        s_CurrentFrame.alt = s_GPSPosition.alt;

        return true;
    }

    return false;
}

static void _get_adc_data(void)
{
    if (hal_time_run_every_ms(5000, &s_ADCMeasurementTimeOffset))
    {
        hal_voltage_level_t channels[4];
        ads7038_read_channels(&s_ADS7038Config, channels, sizeof(channels) / sizeof(hal_voltage_level_t));

        hal_voltage_level_t batVolts = hal_adc_read_voltage(hal_adc_convert_pin_to_input(PIN_BATTERY));
        uint8_t batPercent = battery_convert(&s_BatteryConfig, batVolts);

        batVolts *= 6.0f / 0.5454f;

        s_CurrentADCData.ignDet1 = channels[0];
        s_CurrentADCData.ignDet2 = channels[1];
        s_CurrentADCData.ignDet3 = channels[2];
        s_CurrentADCData.ignDet4 = channels[3];
        s_CurrentADCData.batVolts = batVolts;
        s_CurrentADCData.batPercent = batPercent;
    }
}

static WS2812COLOR _get_ign_diode_color(hal_voltage_level_t v)
{
    ign_pin_state_t state;
    ign_check_pin(&s_IGN, v, &state);

    return state.fuseWorking && state.ignPresent ? ws2812_get_color(0, 255, 0) : !state.fuseWorking || !state.ignPresent ? ws2812_get_color(255, 255, 0)
                                                                             : !state.fuseWorking && !state.ignPresent   ? ws2812_get_color(255, 0, 0)
                                                                                                                         : ws2812_get_color(0, 0, 0);
}

static WS2812COLOR _get_bat_diode_color(uint8_t percent)
{
    if (percent < 50)
    {
        return ws2812_get_color(255, 255 * percent / 50, 0);
    }
    else
    {
        return ws2812_get_color(255 - 255 * (percent - 50) / 50, 255, 0);
    }
}

static void _update_diodes()
{
    if (hal_time_run_every_ms(250, &s_DiodesTimeOffset))
    {
        WS2812COLOR colors[] = {
            _get_ign_diode_color(s_CurrentADCData.ignDet1),
            _get_ign_diode_color(s_CurrentADCData.ignDet2),
            _get_ign_diode_color(s_CurrentADCData.ignDet3),
            _get_ign_diode_color(s_CurrentADCData.ignDet4),
            ws2812_get_color(0, 255, 0),
            _get_bat_diode_color(s_CurrentADCData.batPercent),
            ws2812_get_color(0, 255, 0),
        };

        ws2812_set_colors(colors, sizeof(colors) / sizeof(WS2812COLOR));
    }
}

static bool _update(void)
{
    if (_get_measurements())
    {
        sm_input_t smInput = {
            .accel = s_CurrentFrame.acc1,
            .alt = s_CurrentFrame.alt,
        };

        sm_update(&s_SM, &smInput);

        if (s_SM.state == FLIGHT_STATE_ACCELERATING || s_SM.state == FLIGHT_STATE_FREE_FLIGHT || s_SM.state == FLIGHT_STATE_FREE_FALL)
        {
            dataman_save_sensors_frame(&s_CurrentFrame);
        }
    }

    ign_update(&s_IGN, &s_SM);

    _get_adc_data();
    _update_diodes();

    return s_SM.state != FLIGHT_STATE_LANDED;
}

static void _terminate(void)
{
    hal_serial_printf("Terminating...\n");

    dataman_flush();

    while (true)
    {
        hal_tight_loop();
    }
}

int main()
{
    _init_board();
    _init_drivers();
    _init_sytems();

    while (true)
    {
        if (!_update())
        {
            break;
        }
    }

    _terminate();
}