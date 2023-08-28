#include "drivers/buzzer/active_buzzer_driver.h"

FUNCRESULT activeBuzzerInit(PinNumber pin)
{
    gpioInitPin(pin, GPIO_OUTPUT);
    
    activeBuzzerSetActive(pin, FALSE);

    return SUC_OK;
}

FUNCRESULT activeBuzzerSetActive(PinNumber pin, BOOL active)
{
    gpioSetPinState(pin, active ? GPIO_HIGH : GPIO_LOW);

    return SUC_OK;
}