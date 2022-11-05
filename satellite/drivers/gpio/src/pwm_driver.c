#include "drivers/gpio/pwm_driver.h"
#include "hardware/pwm.h"
#include <math.h>

static const UINT32 PWM_FREQ_HZ = 125E6;
static const UINT32 PWM_DEFAULT_WRAP = 65535;

static UINT32 pwmGetClockDiv(UINT32 frequency)
{
    return ceil((float)PWM_FREQ_HZ / (float)(PWM_DEFAULT_WRAP * frequency));
}

BOOL pwmCheckPin(PinNumber pin)
{
    return pin >= 0 && pin <= 28;
}

FUNCRESULT pwmInit(PinNumber pin, UINT32 frequency)
{
    if (!pwmCheckPin(pin))
    {
        return ERR_INVALIDARG;
    }

    if (FUNCFAILED(gpioSetPinFunction(pin, GPIO_FUNCTION_PWM)))
    {
        return ERR_FAIL;
    }

    INT32 slice_num = pwm_gpio_to_slice_num(pin);
    UINT32 clockDiv = pwmGetClockDiv(frequency);
    UINT32 wrap = PWM_FREQ_HZ / (clockDiv * frequency);

    pwm_set_clkdiv(slice_num, clockDiv);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, TRUE);

    return SUC_OK;
}

FUNCRESULT pwmSetDuty(PinNumber pin, UINT32 frequency, FLOAT dutyCyclePercent)
{
    if (!pwmCheckPin(pin))
    {
        return ERR_INVALIDARG;
    }

    UINT32 clockDiv = pwmGetClockDiv(frequency);
    UINT32 wrap = PWM_FREQ_HZ / (clockDiv * frequency);

    pwm_set_gpio_level(pin, dutyCyclePercent * wrap);

    return SUC_OK;
}