#include "drivers/buzzer/simple_buzzer_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define PIN 14

int main()
{
    simpleBuzzerInit(PIN);

    while (TRUE)
    {
        printf("Buzzer ON\n");
        simpleBuzzerSetActive(PIN, TRUE);
        sleep_ms(1000);
        
        printf("Buzzer OFF\n");
        simpleBuzzerSetActive(PIN, FALSE);
        sleep_ms(1000);
    }

    return 0;
}