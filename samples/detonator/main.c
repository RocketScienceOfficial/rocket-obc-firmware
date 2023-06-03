#include "drivers/detonator/detonator_driver.h"
#include "tools/time_tracker.h"

#define PIN 23

int main()
{
    sleepMiliseconds(10000);

    detonatorActivate(PIN, 1000);

    while (TRUE)
    {
        sleepMiliseconds(1000);
    }

    return 0;
}