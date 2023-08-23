#include "drivers/igniter/igniter_driver.h"
#include "drivers/tools/time_tracker.h"

FUNCRESULT igniterActivate(PinNumber pin, UINT32 delay)
{
    gpioInitPin(pin, GPIO_OUTPUT);

    gpioSetPinState(pin, GPIO_HIGH);
    sleepMiliseconds(delay);
    gpioSetPinState(pin, GPIO_LOW);

    return SUC_OK;
}