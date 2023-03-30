#include "drivers/buzzer/simple_buzzer_driver.h"

FUNCRESULT simpleBuzzerInit(PinNumber pin)
{
    gpioInitPin(pin, GPIO_OUTPUT);
    gpioSetPinState(pin, GPIO_LOW);

    return SUC_OK;
}

FUNCRESULT simpleBuzzerSetActive(PinNumber pin, BOOL active)
{
    gpioSetPinState(pin, active ? GPIO_HIGH : GPIO_LOW);

    return SUC_OK;
}