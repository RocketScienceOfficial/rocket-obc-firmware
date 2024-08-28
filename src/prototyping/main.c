#include "hal/board_control.h"
#include "hal/time_tracker.h"
#include "hal/serial_driver.h"
#include "hal/spi_driver.h"
#include "hal/uart_driver.h"
#include "hal/i2c_driver.h"
#include "hal/adc_driver.h"
#include "lib/drivers/imu/bmi088_driver.h"
#include "lib/drivers/imu/lsm6dso32_driver.h"
#include "lib/drivers/accelerometer/h3lis331dl_driver.h"
#include "lib/drivers/magnetometer/mmc5983ma_driver.h"
#include "lib/drivers/barometer/ms5611_driver.h"
#include "lib/drivers/gps/gps_driver.h"
#include "lib/drivers/adc/ads7038_driver.h"
#include "lib/drivers/adc/ads786x_driver.h"
#include "lib/drivers/led/w2812_driver.h"
#include "lib/drivers/buzzer/passive_buzzer_driver.h"
#include "lib/battery/battery_utils.h"
#include "lib/geo/geo.h"
#include "lib/geo/geo_utils.h"
#include <string.h>
#include <stdlib.h>
#include "board_config.h"

static char *_get_cmd(char *cmd, size_t len, const char *prompt)
{
    hal_serial_printf("%s\n", prompt);

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

                hal_serial_printf("Submitting command: %s\n", cmd);

                break;
            }
            else
            {
                if ((c >= (int)'0' && c <= (int)'9') || (c >= (int)'A' && c <= (int)'Z') || (c >= (int)'a' && c <= (int)'z'))
                {
                    if (currentSize >= len)
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

    return cmd;
}

int main()
{
    hal_board_init(5000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Hardware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(OBC_SPI, OBC_SPI_MISO_PIN, OBC_SPI_MOSI_PIN, OBC_SPI_SCK_PIN, OBC_SPI_FREQ);
    hal_uart_init_all(OBC_UART, OBC_UART_RX, OBC_UART_TX, OBC_UART_FREQ);
    hal_adc_init_all();

    char cmd[128];
    _get_cmd(cmd, sizeof(cmd), "Enter command: ");

    if (strcmp(cmd, "bmi") == 0)
    {
        bmi088_accel_config_t bmi088AccelConfig = {0};
        bmi088_gyro_config_t bmi088GyroConfig = {0};

        bmi088_accel_init_spi(&bmi088AccelConfig, OBC_SPI, PIN_CS_BMI_ACC);
        bmi088_accel_set_conf(&bmi088AccelConfig, BMI088_ACCEL_ODR_800HZ, BMI088_ACCEL_OSR_NORMAL);
        bmi088_accel_set_range(&bmi088AccelConfig, BMI088_ACCEL_RANGE_6G);

        bmi088_gyro_init_spi(&bmi088GyroConfig, OBC_SPI, PIN_CS_BMI_GYRO);
        bmi088_gyro_set_bandwidth(&bmi088GyroConfig, BMI088_GYRO_ODR_2000_BW_523HZ);
        bmi088_gyro_set_range(&bmi088GyroConfig, BMI088_GYRO_RANGE_500DPS);

        while (true)
        {
            vec3_t acc, gyro;
            bmi088_accel_read(&bmi088AccelConfig, &acc);
            bmi088_gyro_read(&bmi088GyroConfig, &gyro);

            hal_serial_printf("%f %f %f   %f %f %f\n", acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "lsm") == 0)
    {
        lsm6dso32_config_t lsm6dso32Config = {0};

        lsm6dso32_init_spi(&lsm6dso32Config, OBC_SPI, PIN_CS_LSM);
        lsm6dso32_set_odr(&lsm6dso32Config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ);
        lsm6dso32_set_range(&lsm6dso32Config, LSM6DSO32_RANGE_32G, LSM6DSO32_RANGE_2000DPS);

        while (true)
        {
            vec3_t acc, gyro;
            lsm6dso32_read(&lsm6dso32Config, &acc, &gyro, NULL);

            hal_serial_printf("%f %f %f   %f %f %f\n", acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "h3lis") == 0)
    {
        h3lis331dl_config_t h3lisConfig = {0};

        h3lis331dl_init_spi(&h3lisConfig, OBC_SPI, PIN_CS_H3LIS);
        h3lis331dl_set_power_mode(&h3lisConfig, H3LIS331DL_POWER_NORMAL);
        h3lis331dl_set_range(&h3lisConfig, H3LIS331DL_RANGE_100G);
        h3lis331dl_set_odr(&h3lisConfig, H3LIS331DL_ODR_400HZ);

        while (true)
        {
            vec3_t acc;
            h3lis331dl_read(&h3lisConfig, &acc);

            hal_serial_printf("%f %f %f\n", acc.x, acc.y, acc.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "mmc") == 0)
    {
        mmc5983ma_config_t mmc5983maConfig = {0};

        mmc5983ma_init_spi(&mmc5983maConfig, OBC_SPI, PIN_CS_MMC);
        mmc5983ma_set_continuous_mode_odr(&mmc5983maConfig, MMC5983MA_ODR_1000HZ, MMC5983MA_PRD_SET_250);

        bool valid = mmc5983ma_validate_id(&mmc5983maConfig);
        hal_serial_printf("%d\n\n\n\n", (int)valid);

        while (true)
        {
            vec3_t mag;
            mmc5983ma_read(&mmc5983maConfig, &mag);

            hal_serial_printf("%f %f %f\n", mag.x, mag.y, mag.z);
            hal_time_sleep_ms(100);
        }
    }
    else if (strcmp(cmd, "ms56") == 0)
    {
        ms5611_config_t ms5611Config = {0};

        ms5611_init_spi(&ms5611Config, OBC_SPI, PIN_CS_MS56);
        ms5611_set_osr(&ms5611Config, MS5611_OSR_256, MS5611_OSR_256);

        while (true)
        {
            int press;
            float temp;

            if (ms5611_read_non_blocking(&ms5611Config, &press, &temp))
            {
                hal_serial_printf("%d Pa  %f C  %f m\n", press, temp, height_from_baro_formula(press));
            }
        }
    }
    else if (strcmp(cmd, "neo") == 0)
    {
        gps_config_t gpsConfig = {0};
        geo_position_wgs84_t gpsPosition = {0};

        gps_init_spi(&gpsConfig, OBC_SPI, PIN_CS_NEO);

        msec_t s = 0;

        while (true)
        {
            if (!gps_read(&gpsConfig))
            {
                hal_time_sleep_ms(10);
            }

            gps_get_pos(&gpsConfig, &gpsPosition);

            if (hal_time_run_every_ms(1000, &s))
            {
                hal_serial_printf("Lat: %f\nLon: %f\nAlt: %f\n", gpsPosition.lat, gpsPosition.lon, gpsPosition.alt);
            }
        }
    }
    else if (strcmp(cmd, "ads") == 0)
    {
        // ads7038_config_t ads7038Config = {0};
        // ads7038_init(&ads7038Config, OBC_SPI, PIN_CS_ADS, 1 << ADC_IGN_1_DET_CH | 1 << ADC_IGN_2_DET_CH | 1 << ADC_IGN_3_DET_CH | 1 << ADC_IGN_4_DET_CH, ADC_VREF);

        // while (true)
        // {
        //     float channels[4];
        //     ads7038_read_channels(&ads7038Config, channels, sizeof(channels) / sizeof(float));

        //     channels[0] -= 0.015f;
        //     channels[1] -= 0.023f;
        //     channels[2] -= 0.048f;
        //     channels[3] -= 0.010f;

        //     hal_serial_printf("%f %f %f %f\n", channels[0], channels[1], channels[2], channels[3]);
        //     hal_time_sleep_ms(100);
        // }

        ads786x_config_t ads786xConfig = {0};
        ads786x_init(&ads786xConfig, OBC_SPI, PIN_CS_ADS, ADS786X_TYPE_6, ADC_VREF);

        while (true)
        {
            float total = 0;
            for (int i = 0; i < 100; i++)
            {
                total += 1.138f * (ads786x_read(&ads786xConfig) * 11) - 1.7f;
            }
            total /= 100;
            hal_serial_printf("%f\n", total);
            hal_time_sleep_ms(10);
        }
    }
    else if (strcmp(cmd, "ws") == 0)
    {
        ws2812_init(PIN_LED, false);

        ws2812_color_t colors[] = {
            ws2812_get_color(0, 255, 0),
            ws2812_get_color(255, 255, 0),
            ws2812_get_color(0, 255, 255),
            ws2812_get_color(255, 255, 255),
            ws2812_get_color(255, 128, 0),
            ws2812_get_color(128, 255, 0),
            ws2812_get_color(128, 255, 128),
        };

        ws2812_set_colors(colors, sizeof(colors) / sizeof(ws2812_color_t));
    }
    else if (strcmp(cmd, "ign") == 0)
    {
        hal_gpio_init_pin(PIN_IGN_EN_1, GPIO_OUTPUT);
        hal_gpio_init_pin(PIN_IGN_EN_2, GPIO_OUTPUT);
        hal_gpio_init_pin(PIN_IGN_EN_3, GPIO_OUTPUT);
        hal_gpio_init_pin(PIN_IGN_EN_4, GPIO_OUTPUT);
        hal_gpio_set_pin_state(PIN_IGN_EN_1, GPIO_HIGH);
        hal_gpio_set_pin_state(PIN_IGN_EN_2, GPIO_HIGH);
        hal_gpio_set_pin_state(PIN_IGN_EN_3, GPIO_HIGH);
        hal_gpio_set_pin_state(PIN_IGN_EN_4, GPIO_HIGH);
    }
    else if (strcmp(cmd, "pin") == 0)
    {
        char pin_str[128];
        _get_cmd(pin_str, sizeof(pin_str), "GPIO: ");

        char delay_str[128];
        _get_cmd(delay_str, sizeof(delay_str), "Delay [ms]: ");

        int pin = atoi(pin_str);
        int delay = atoi(delay_str);

        hal_gpio_init_pin(pin, GPIO_OUTPUT);
        hal_gpio_set_pin_state(pin, GPIO_LOW);

        while (true)
        {
            hal_time_sleep_ms(delay);
            hal_serial_printf("High!\n");
            hal_gpio_set_pin_state(pin, GPIO_HIGH);

            hal_time_sleep_ms(delay);
            hal_serial_printf("Low!\n");
            hal_gpio_set_pin_state(pin, GPIO_LOW);
        }
    }
    else if (strcmp(cmd, "buzzer") == 0)
    {
        hal_pwm_config_t config;
        passive_buzzer_init(&config, PIN_BUZZER, 2730);
        passive_buzzer_set_active(&config, true);
    }
    else if (strcmp(cmd, "adc") == 0)
    {
        hal_adc_init_pin(PIN_IGN_DET_1);
        hal_adc_init_pin(PIN_IGN_DET_2);
        hal_adc_init_pin(PIN_IGN_DET_3);
        hal_adc_init_pin(PIN_IGN_DET_4);

        while (true)
        {
            hal_serial_printf("%f  %f  %f  %f\n", 1.035f * (hal_adc_read_voltage(PIN_IGN_DET_1) - 0.036f), 1.035f * (hal_adc_read_voltage(PIN_IGN_DET_2) - 0.036f), 1.035f * (hal_adc_read_voltage(PIN_IGN_DET_3) - 0.036f), 1.035f * (hal_adc_read_voltage(PIN_IGN_DET_4) - 0.036f));

            hal_time_sleep_ms(100);
        }
    }

    while (true)
    {
        hal_tight_loop();
    }
}