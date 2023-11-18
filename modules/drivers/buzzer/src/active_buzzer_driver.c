#include "modules/drivers/buzzer/active_buzzer_driver.h"

void active_buzzer_init(pin_number_t pin)
{
    gpio_init_pin(pin, GPIO_OUTPUT);
    
    active_buzzer_set_active(pin, false);
}

void active_buzzer_set_active(pin_number_t pin, bool active)
{
    gpio_set_pin_state(pin, active ? GPIO_HIGH : GPIO_LOW);
}