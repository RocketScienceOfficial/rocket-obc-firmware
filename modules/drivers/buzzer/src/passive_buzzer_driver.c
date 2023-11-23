#include "modules/drivers/buzzer/passive_buzzer_driver.h"

#define DUTY_CYCLE 0.5f

void passive_buzzer_init(hal_pwm_config_t *config, hal_pin_number_t pin, unsigned int frequency)
{
    hal_pwm_init_pin(config, pin, frequency);
}

void passive_buzzer_set_active(hal_pwm_config_t *config, bool active)
{
    hal_pwm_set_duty(config, active ? DUTY_CYCLE : 0.0f);
}