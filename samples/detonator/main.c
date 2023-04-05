#include "drivers/detonator/detonator_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define PIN 23

int main()
{
    sleep_ms(10000);

    detonatorActivate(PIN);

    sleep_ms(1000);

    detonatorActivate(PIN + 1);

    sleep_ms(1000);

    detonatorActivate(PIN + 2);

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}