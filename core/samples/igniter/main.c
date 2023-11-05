#include "drivers/igniter/igniter_driver.h"
#include "drivers/tools/time_tracker.h"

int main()
{
    sleepMiliseconds(10000);

    IgniterData igniterData = {
        .pins = {7, 21, 22, 23},
        .count = 4,
        .delay = 1000,
    };
    
    ignInit(&igniterData);
    ignFire(&igniterData);

    while (TRUE)
    {
        ignUpdate(&igniterData);
    }

    return 0;
}