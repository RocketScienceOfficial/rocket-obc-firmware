#include "drivers/led/simple_led_driver.h"
#include "pico/stdlib.h"

FUNCRESULT ledInit(PinNumber pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);

    return SUC_OK;
}

FUNCRESULT setLedState(PinNumber pin, BOOL state)
{
    gpio_put(pin, state);
}