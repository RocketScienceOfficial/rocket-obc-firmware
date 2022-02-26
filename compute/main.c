#include <stdio.h>

#include "logger.h"
#include "bmp280_controller.h"
#include "mpu6050_controller.h"
#include "sd_card_controller.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    //sd_init();

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

        my_log_measure_start();
        my_log_measure("%.3f", pressure / 100.f);
        my_log_measure("%.2f", temperature / 100.f);
        my_log_measure_end();

        sleep_ms(1000);
    }

    sd_terminate();

    return 0;
}