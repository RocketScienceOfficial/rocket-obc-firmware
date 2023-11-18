#include <stdio.h>
#include "pico/stdlib.h"
#include "modules/drivers/accelerometer/bmi088_driver.h"

int main()
{
    stdio_init_all();
    _bmi088_accel_soft_reset(NULL);

    while (1)
    {
        printf("Hello, world!\n");

        sleep_ms(1000);
    }
}