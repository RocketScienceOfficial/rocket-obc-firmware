#include "modules/drivers/hal/pwm_driver.h"
#include "hardware/pwm.h"
#include <math.h>

static const unsigned long PWM_FREQ_HZ = 125E6;
static const unsigned long PWM_DEFAULT_WRAP = 65535;

static unsigned long __pwm_get_clk_div(unsigned long frequency)
{
    return ceilf((float)PWM_FREQ_HZ / (float)(PWM_DEFAULT_WRAP * frequency));
}

static unsigned long __pwm_get_wrap(unsigned long frequency, unsigned long clockDiv)
{
    return PWM_FREQ_HZ / (clockDiv * frequency);
}

bool pwm_check_pin(pin_number_t pin)
{
    return pin >= 0 && pin <= 28;
}

void pwm_init_pin(pwm_config_t *config, pin_number_t pin, unsigned long frequency)
{
    if (!config || !pwm_check_pin(pin))
    {
        return;
    }

    gpio_set_pin_function(pin, GPIO_FUNCTION_PWM);

    config->pin = pin;

    pwm_set_frequency(config, frequency);

    unsigned long slice_num = pwm_gpio_to_slice_num(pin);

    pwm_set_enabled(slice_num, true);
}

void pwm_set_frequency(pwm_config_t *config, unsigned long frequency)
{
    if (!config)
    {
        return;
    }

    config->frequency = frequency;

    unsigned long slice_num = pwm_gpio_to_slice_num(config->pin);
    unsigned long clockDiv = __pwm_get_clk_div(frequency);
    unsigned long wrap = __pwm_get_wrap(frequency, clockDiv);

    pwm_set_clkdiv(slice_num, clockDiv);
    pwm_set_wrap(slice_num, wrap);
}

void pwm_set_duty(pwm_config_t *config, float dutyCyclePercent)
{
    if (!config)
    {
        return;
    }

    unsigned long clockDiv = __pwm_get_clk_div(config->frequency);
    unsigned long wrap = __pwm_get_wrap(config->frequency, clockDiv);

    pwm_set_gpio_level(config->pin, dutyCyclePercent * wrap);
}