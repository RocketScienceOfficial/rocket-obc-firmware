#include "hal/board_control.h"
#include "hal/time_tracker.h"
#include "hal/serial_driver.h"
#include "hal/spi_driver.h"
#include "hal/i2c_driver.h"
#include "hal/adc_driver.h"
#include "lib/drivers/accelerometer/bmi088_driver.h"
#include "lib/drivers/accelerometer/lsm6dso32_driver.h"
#include "lib/drivers/accelerometer/h3lis331dl_driver.h"
#include "lib/drivers/magnetometer/mmc5983ma_driver.h"
#include "lib/drivers/barometer/ms56xx_02ba03.h"
#include "lib/drivers/gps/ublox_neo_m9n_driver.h"
#include "lib/drivers/adc/ads7038_q1_driver.h"
#include "lib/drivers/led/w2812_driver.h"
#include "lib/drivers/battery/battery_driver.h"
#include "lib/nmea/nmea_wrapper.h"
#include <string.h>
#include "board_config.h"

static bmi088_accel_config_t s_BMI088AccelConfig;
static bmi088_gyro_config_t s_BMI088GyroConfig;
static lsm6dso32_config_t s_LSM6DSO32Config;
static h3lis331dl_config_t s_H3LIS331DLConfig;
static mmc5983ma_config_t s_MMC5983MAConfig;
static ms56xx_02ba03_config_t s_MS56XX02BA03Config;
static ublox_neo_m9n_config_t s_UBloxNeoM9NConfig;
static gps_nmea_sentence_t s_GPSSentence;
static geo_position_wgs84_t s_GNSSData;
static ads7038_q1_config_t s_ADS7038Q1Config;
static battery_config_t s_BatteryConfig;

int main()
{
    hal_board_init(10000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Hardware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(OBC_SPI, OBC_SPI_FREQ);
    hal_spi_init_pins(OBC_SPI, OBC_SPI_MISO_PIN, OBC_SPI_MOSI_PIN, OBC_SPI_SCK_PIN);
    hal_adc_init_all();

    hal_serial_printf("Enter command: \n");

    char cmd[128];
    size_t currentSize = 0;
    while (true)
    {
        int c = 0;
        if (hal_serial_read_char(&c))
        {
            if (c == (int)'\r')
            {
                cmd[currentSize] = '\0';
                hal_serial_printf("\n");

                hal_serial_printf("Submitting command: %s", cmd);

                break;
            }
            else
            {
                if ((c >= (int)'0' && c <= (int)'9') || (c >= (int)'A' && c <= (int)'Z') || (c >= (int)'a' && c <= (int)'z'))
                {
                    if (currentSize >= sizeof(cmd) - 1)
                    {
                        hal_serial_printf("Buffer overflow\n");

                        break;
                    }

                    cmd[currentSize++] = (char)c;
                    hal_serial_printf("%c", (char)c);
                }
            }
        }
    }

    if (strcmp(cmd, "bmi") == 0)
    {
        bmi088_accel_init_spi(&s_BMI088AccelConfig, OBC_SPI, PIN_CS_BMI_ACC);
        bmi088_accel_set_conf(&s_BMI088AccelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
        bmi088_accel_set_range(&s_BMI088AccelConfig, BMI088_ACCEL_RANGE_6G);

        bmi088_gyro_init_spi(&s_BMI088GyroConfig, OBC_SPI, PIN_CS_BMI_GYRO);
        bmi088_gyro_set_bandwidth(&s_BMI088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
        bmi088_gyro_set_range(&s_BMI088GyroConfig, BMI088_GYRO_RANGE_500DPS);

        while (true)
        {
            vec3_t acc, gyro;
            bmi088_accel_read(&s_BMI088AccelConfig, &acc);
            bmi088_gyro_read(&s_BMI088GyroConfig, &gyro);

            hal_serial_printf("%f %f %f   %f %f %f\n", acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "lsm") == 0)
    {
        lsm6dso32_init_spi(&s_LSM6DSO32Config, OBC_SPI, PIN_CS_LSM);
        lsm6dso32_set_odr(&s_LSM6DSO32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ);
        lsm6dso32_set_range(&s_LSM6DSO32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS);

        while (true)
        {
            vec3_t acc, gyro;
            lsm6dso32_read(&s_LSM6DSO32Config, &acc, &gyro, NULL);

            hal_serial_printf("%f %f %f   %f %f %f\n", acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "h3lis") == 0)
    {
        h3lis331dl_init_spi(&s_H3LIS331DLConfig, OBC_SPI, PIN_CS_H3LIS);
        h3lis331dl_set_power_mode(&s_H3LIS331DLConfig, H3LIS331DL_POWER_NORMAL);
        h3lis331dl_set_range(&s_H3LIS331DLConfig, H3LIS331DL_RANGE_100G);
        h3lis331dl_set_odr(&s_H3LIS331DLConfig, H3LIS331DL_ODR_400HZ);

        while (true)
        {
            vec3_t acc;
            h3lis331dl_read(&s_H3LIS331DLConfig, &acc);

            hal_serial_printf("%f %f %f\n", acc.x, acc.y, acc.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "mmc") == 0)
    {
        mmc5983ma_init_spi(&s_MMC5983MAConfig, OBC_SPI, PIN_CS_MMC);
        mmc5983ma_set_continuous_mode_odr(&s_MMC5983MAConfig, MMC5983MA_ODR_1000HZ);

        bool valid = false;
        mmc5983_validate_id(&s_MMC5983MAConfig, &valid);
        hal_serial_printf("%d\n\n\n\n", (int)valid);

        while (true)
        {
            vec3_t mag;
            mmc5983ma_read(&s_MMC5983MAConfig, &mag);

            hal_serial_printf("%f %f %f\n", mag.x, mag.y, mag.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "ms56") == 0)
    {
        ms56xx_02ba03_init_spi(&s_MS56XX02BA03Config, OBC_SPI, PIN_CS_MS56);
        ms56xx_02ba03_set_osr(&s_MS56XX02BA03Config, MS56XX_02BA03_OSR_256, MS56XX_02BA03_OSR_256);

        while (true)
        {
            int press;
            float temp;
            ms56xx_02ba03_read(&s_MS56XX02BA03Config, &press, &temp);

            hal_serial_printf("%d Pa  %f C\n", press, temp);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "neo") == 0)
    {
        // ublox_neo_m9n_init_spi(&s_UBloxNeoM9NConfig, OBC_SPI, PIN_CS_NEO);
        // msec_t offset = hal_time_get_ms_since_boot();

        // while (true)
        // {
        //     ublox_neo_m9n_read_data(&s_UBloxNeoM9NConfig, &s_GPSSentence);
        //     gnss_update(&s_GNSSData, &s_GPSSentence);

        //     if (hal_time_run_every_ms(1000, &offset))
        //     {
        //         hal_serial_printf("%f %f %f\n", s_GNSSData.lat, s_GNSSData.lon, s_GNSSData.alt);
        //     }
        // }

        // hal_spi_init_PIN_CS(OBC_SPI, PIN_CS_NEO);

        // while (true)
        // {
        //     uint8_t data;
        //     hal_spi_PIN_CS_select(PIN_CS_NEO);
        //     hal_spi_read(OBC_SPI, 0, &data, 1);
        //     hal_spi_PIN_CS_deselect(PIN_CS_NEO);
        //     printf("%d\n", data);
        //     hal_time_sleep_ms(10);
        // }
    }
    else if (strcmp(cmd, "ads") == 0)
    {
        ads7038_q1_init(&s_ADS7038Q1Config, OBC_SPI, PIN_CS_ADC, 1 << ADC_IGN_1_DET_CH | 1 << ADC_IGN_2_DET_CH | 1 << ADC_IGN_3_DET_CH | 1 << ADC_IGN_4_DET_CH, ADC_VREF);

        while (true)
        {
            hal_voltage_level_t channels[4];
            ads7038_q1_read_channels(&s_ADS7038Q1Config, channels, sizeof(channels) / sizeof(hal_voltage_level_t));

            hal_serial_printf("%f %f %f %f\n", channels[0], channels[1], channels[2], channels[3]);
            hal_time_sleep_ms(1000);
        }
    }
    else if (strcmp(cmd, "bat") == 0)
    {
        battery_interval_t intervals[] = {
            {0.5454f, 0.7636f, 0, 100},
        };
        battery_init(&s_BatteryConfig, hal_adc_convert_pin_to_input(PIN_BATTERY), intervals, sizeof(intervals) / sizeof(battery_interval_t));

        while (true)
        {
            hal_voltage_level_t batVolts;
            uint8_t batPercent;
            battery_read(&s_BatteryConfig, &batPercent, &batVolts);
            batVolts *= 6.0f / 0.5454f;

            hal_serial_printf("%d%%   %fV\n", batPercent, batVolts);
        }
    }
    else if (strcmp(cmd, "ws") == 0)
    {
        ws2812_init(PIN_LED, false);

        WS2812COLOR colors[] = {
            ws2812_get_color(0, 255, 0),
            ws2812_get_color(255, 255, 0),
            ws2812_get_color(0, 255, 255),
            ws2812_get_color(255, 255, 255),
            ws2812_get_color(255, 128, 0),
            ws2812_get_color(128, 255, 0),
            ws2812_get_color(128, 255, 128),
        };

        ws2812_set_colors(colors, sizeof(colors) / sizeof(WS2812COLOR));
    }

    while (true)
    {
        hal_tight_loop();
    }
}