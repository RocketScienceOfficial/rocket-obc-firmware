#include "drivers/detonator/detonator_driver.h"

FUNCRESULT detonatorActivate(PinNumber pin)
{
    gpioInitPin(pin, GPIO_OUTPUT);
    gpioSetPinState(pin, GPIO_HIGH);
    
    return SUC_OK;
}