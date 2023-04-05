#include "drivers/detonator/detonator_driver.h"
#include "pico/stdlib.h"

FUNCRESULT detonatorActivate(PinNumber pin, UINT32 delay)
{
    gpioInitPin(pin, GPIO_OUTPUT);

    gpioSetPinState(pin, GPIO_HIGH);
    sleep_ms(delay);
    gpioSetPinState(pin, GPIO_LOW);

    return SUC_OK;
}