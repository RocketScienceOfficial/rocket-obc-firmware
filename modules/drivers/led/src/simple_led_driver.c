#include "drivers/led/simple_led_driver.h"

FUNCRESULT ledInit(PinNumber pin)
{
    return gpioInitPin(pin, GPIO_OUTPUT);
}

FUNCRESULT ledSetState(PinNumber pin, BOOL state)
{
    return gpioSetPinState(pin, state ? GPIO_HIGH : GPIO_LOW);
}