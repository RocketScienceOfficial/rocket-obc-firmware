#include "drivers/buzzer/passive_buzzer_driver.h"
#include "drivers/tools/time_tracker.h"

#define PIN 24

int main()
{
    PWMConfig config = {0};

    passiveBuzzerInit(&config, PIN, 3000);

    while (TRUE)
    {
        passiveBuzzerSetActive(&config, TRUE);
        sleepMiliseconds(1000);

        passiveBuzzerSetActive(&config, FALSE);
        sleepMiliseconds(1000);
    }

    return 0;
}