#include "drivers/igniter/igniter_driver.h"
#include "drivers/tools/time_tracker.h"

#define PIN 23

int main()
{
    sleepMiliseconds(10000);

    igniterActivate(PIN, 1000);

    while (TRUE)
    {
        sleepMiliseconds(1000);
    }

    return 0;
}