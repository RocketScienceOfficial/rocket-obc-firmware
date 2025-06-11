#include "hal/pwm_driver.h"
#include "hardware/pwm.h"
#include <math.h>

static const unsigned long PWM_FREQ_HZ = 125E6;
static const unsigned long PWM_DEFAULT_WRAP = 65535;

bool hal_pwm_check_pin(hal_pin_number_t pin)
{
    return pin >= 0 && pin <= 28;
}

bool hal_pwm_init_pin(hal_pwm_config_t *config, hal_pin_number_t pin, unsigned long frequency)
{
    if (!config || !hal_pwm_check_pin(pin))
    {
        return false;
    }

    config->pin = pin;

    hal_gpio_set_pin_function(pin, GPIO_FUNCTION_PWM);

    hal_pwm_set_frequency(config, frequency);
    hal_pwm_set_duty(config, 0);

    unsigned long slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_enabled(slice_num, true);

    return true;
}

bool hal_pwm_set_frequency(hal_pwm_config_t *config, unsigned long frequency)
{
    if (!config)
    {
        return false;
    }

    unsigned long clockDiv = ceilf((float)PWM_FREQ_HZ / (float)(PWM_DEFAULT_WRAP * frequency));
    unsigned long wrap = PWM_FREQ_HZ / (clockDiv * frequency);
    unsigned long slice_num = pwm_gpio_to_slice_num(config->pin);

    pwm_set_clkdiv(slice_num, clockDiv);
    pwm_set_wrap(slice_num, wrap);

    config->clockDiv = clockDiv;

    return true;
}

bool hal_pwm_set_duty(const hal_pwm_config_t *config, unsigned int dutyCycleUs)
{
    if (!config)
    {
        return false;
    }

    unsigned long wrap = dutyCycleUs * PWM_FREQ_HZ / config->clockDiv;

    pwm_set_gpio_level(config->pin, wrap);

    return true;
}