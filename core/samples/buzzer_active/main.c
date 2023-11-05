#include "drivers/buzzer/active_buzzer_driver.h"
#include "drivers/tools/time_tracker.h"
#include <stdio.h>

#define PIN 24

int main()
{
    activeBuzzerInit(PIN);

    while (TRUE)
    {
        printf("Buzzer ON\n");
        activeBuzzerSetActive(PIN, TRUE);
        sleepMiliseconds(1000);
        
        printf("Buzzer OFF\n");
        activeBuzzerSetActive(PIN, FALSE);
        sleepMiliseconds(1000);
    }

    return 0;
}