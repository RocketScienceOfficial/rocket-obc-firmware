#include "modules/drivers/led/simple_led_driver.h"

void led_init(pin_number_t pin)
{
    return gpio_init_pin(pin, GPIO_OUTPUT);
}

void led_set_state(pin_number_t pin, bool state)
{
    return gpio_set_pin_state(pin, state ? GPIO_HIGH : GPIO_LOW);
}