#include <stdio.h>

#include "logger.h"
#include "bmp280_controller.h"
#include "mpu6050_controller.h"
#include "sd_card_controller.h"
#include "kalman.h"

#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

/*
* video: https://www.youtube.com/watch?v=Au-oc4hxj-c
* f = 1 / T = 50Hz
* T = 20ms = 20000ns
* clkT = 8ns
* clkDiv = 1
* wrap = T / (clkT * clkDiv)
* duty = t / T
* set = wrap * duty = (T / (clkT * clkDiv)) * (t / T) = t / (clkT * clkDiv)
*/

void setMillis(int servoPin, float millis)
{
    pwm_set_gpio_level(servoPin, (millis / 20000.f) * 39062.0f);
}

void setServo(int servoPin, float startMillis)
{
    gpio_set_function(servoPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(servoPin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.0f);
    pwm_config_set_wrap(&config, 39062.0f);

    pwm_init(slice_num, &config, true);

    setMillis(servoPin, startMillis);
}

const uint PIN = 14;

int main()
{
    stdio_init_all();

    bool direction = true;
    int currentMillis = 500;

    setServo(PIN, currentMillis);

    while (true)
    {
        //currentMillis += (direction) ? 5 : -5;

        if (currentMillis >= 2400)
            direction = false;
        if (currentMillis <= 400)
            direction = true;

        setMillis(PIN, currentMillis);
        sleep_ms(10);
    }

    // mpu6050_init();

    // sleep_ms(500);

    // struct KalmanFilter filter;

    // while (1)
    // {
    //     double accel_x, accel_y, accel_z, rot_x, rot_y, rot_z, temperature_mpu;

    //     mpu6050_get_values(&accel_x, &accel_y, &accel_z, &rot_x, &rot_y, &rot_z, &temperature_mpu);

    //     printf("Accel: x = %f, y = %f, z = %f\n", accel_x, accel_y, accel_z);
    //     printf("Gyro: x = %f, y = %f, z = %f\n", rot_x, rot_y, rot_z);

    //     sleep_ms(1000);
    // }
}

/*
int main()
{
    stdio_init_all();

    sd_init();

    bmp280_init();
    mpu6050_init();

    sleep_ms(500);

    my_log_measure_init();

    while (1)
    {
        my_log("Measuring...");

        int32_t temperature, pressure;
        bmp280_read_values(&temperature, &pressure);

        int16_t accel_x, accel_y, accel_z, rot_x, rot_y, rot_z, temperature_mpu;

        mpu6050_get_values(&accel_x, &accel_y, &accel_z, &rot_x, &rot_y, &rot_z, &temperature_mpu);

        my_log("%d", accel_x);
        my_log("%d", accel_y);
        my_log("%d", accel_z);

        my_log_measure_start();
        my_log_measure("%.3f", pressure / 100.f);
        my_log_measure("%.2f", temperature / 100.f);
        my_log_measure_end();

        sleep_ms(1000);
    }

    sd_terminate();

    return 0;
}
*/