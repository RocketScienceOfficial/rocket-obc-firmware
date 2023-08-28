#include "drivers/gpio/pwm_driver.h"
#include "hardware/pwm.h"
#include <math.h>

static const UINT32 PWM_FREQ_HZ = 125E6;
static const UINT32 PWM_DEFAULT_WRAP = 65535;

static UINT32 __pwmGetClockDiv(UINT32 frequency)
{
    return ceil((FLOAT)PWM_FREQ_HZ / (FLOAT)(PWM_DEFAULT_WRAP * frequency));
}

static UINT32 __pwmGetWrap(UINT32 frequency, UINT32 clockDiv)
{
    return PWM_FREQ_HZ / (clockDiv * frequency);
}

BOOL pwmCheckPin(PinNumber pin)
{
    return pin >= 0 && pin <= 28;
}

FUNCRESULT pwmInit(PWMConfig *config, PinNumber pin, UINT32 frequency)
{
    if (!config || !pwmCheckPin(pin))
    {
        return ERR_INVALIDARG;
    }

    if (FUNCFAILED(gpioSetPinFunction(pin, GPIO_FUNCTION_PWM)))
    {
        return ERR_FAIL;
    }

    config->pin = pin;

    pwmSetFrequency(config, frequency);

    UINT32 slice_num = pwm_gpio_to_slice_num(pin);
    
    pwm_set_enabled(slice_num, TRUE);

    return SUC_OK;
}

FUNCRESULT pwmSetFrequency(PWMConfig *config, UINT32 frequency)
{
    if (!config)
    {
        return ERR_INVALIDARG;
    }

    config->frequency = frequency;

    UINT32 slice_num = pwm_gpio_to_slice_num(config->pin);
    UINT32 clockDiv = __pwmGetClockDiv(frequency);
    UINT32 wrap = __pwmGetWrap(frequency, clockDiv);

    pwm_set_clkdiv(slice_num, clockDiv);
    pwm_set_wrap(slice_num, wrap);

    return SUC_OK;
}

FUNCRESULT pwmSetDuty(PWMConfig *config, FLOAT dutyCyclePercent)
{
    if (!config)
    {
        return ERR_INVALIDARG;
    }

    UINT32 clockDiv = __pwmGetClockDiv(config->frequency);
    UINT32 wrap = __pwmGetWrap(config->frequency, clockDiv);

    pwm_set_gpio_level(config->pin, dutyCyclePercent * wrap);

    return SUC_OK;
}