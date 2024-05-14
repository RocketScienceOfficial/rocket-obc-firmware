#include "lib/drivers/led/simple_led_driver.h"

void led_init(hal_pin_number_t pin)
{
    hal_gpio_init_pin(pin, GPIO_OUTPUT);
}

void led_set_state(hal_pin_number_t pin, bool state)
{
    hal_gpio_set_pin_state(pin, state ? GPIO_HIGH : GPIO_LOW);
}