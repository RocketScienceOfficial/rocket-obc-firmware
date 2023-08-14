#include "drivers/buzzer/simple_buzzer_driver.h"
#include "drivers/tools/time_tracker.h"
#include <stdio.h>

#define PIN 24

int main()
{
    simpleBuzzerInit(PIN);

    while (TRUE)
    {
        printf("Buzzer ON\n");
        simpleBuzzerSetActive(PIN, TRUE);
        sleepMiliseconds(1000);
        
        printf("Buzzer OFF\n");
        simpleBuzzerSetActive(PIN, FALSE);
        sleepMiliseconds(1000);
    }

    return 0;
}