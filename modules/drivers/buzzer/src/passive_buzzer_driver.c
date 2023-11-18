#include "modules/drivers/buzzer/passive_buzzer_driver.h"

#define DUTY_CYCLE 0.5f

void passive_buzzer_init(pwm_config_t *config, pin_number_t pin, unsigned int frequency)
{
    if (!config)
    {
        return;
    }

    pwm_init_pin(config, pin, frequency);
}

void passive_buzzer_set_active(pwm_config_t *config, bool active)
{
    return pwm_set_duty(config, active ? DUTY_CYCLE : 0.0f);
}