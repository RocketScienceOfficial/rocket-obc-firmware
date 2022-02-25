#include <stdio.h>

#include "logger.h"
#include "bmp280_controller.h"
#include "sd_card_controller.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    sd_init();
    bmp_init();

    sleep_ms(500);

    my_log_measure_init();

    while (1)
    {
        int32_t temperature, pressure;
        bmp_getParams(&temperature, &pressure);

        my_log("Measuring...");

        my_log_measure_start();
        my_log_measure("%.3f", pressure / 100.f);
        my_log_measure("%.2f", temperature / 100.f);
        my_log_measure_end();

        sleep_ms(500);
    }

    sd_terminate();

    return 0;
}