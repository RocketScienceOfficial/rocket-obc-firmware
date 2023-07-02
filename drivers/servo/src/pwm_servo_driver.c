#include "drivers/servo/pwm_servo_driver.h"
#include "drivers/gpio/pwm_driver.h"

static const UINT32 SERVO_FREQ_HZ = 50;
static const FLOAT SERVO_DUTY_CYCLE_MS_0 = 0.5f;
static const FLOAT SERVO_DUTY_CYCLE_MS_90 = 1.5f;
static const FLOAT SERVO_DUTY_CYCLE_MS_180 = 2.5f;
static const FLOAT SERVO_MAX_ANGLE_DEG = 180.0f;

FUNCRESULT pwmServoInit(PinNumber pin)
{
    if (FUNCFAILED(pwmInit(pin, SERVO_FREQ_HZ)))
    {
        return ERR_FAIL;
    }

    BOOL connected = FALSE;

    if (FUNCFAILED(pwmServoCheck(pin, &connected)))
    {
        return ERR_UNEXPECTED;
    }

    if (!connected)
    {
        return ERR_ACCESSDENIED;
    }

    return SUC_OK;
}

FUNCRESULT pwmServoCheck(PinNumber pin, BOOL *result)
{
    GPIOState state = GPIO_LOW;

    if (FUNCFAILED(gpioGetPinState(pin, &state)))
    {
        return ERR_UNEXPECTED;
    }

    *result = state == GPIO_HIGH;

    return SUC_OK;
}

FUNCRESULT pwmServoRotateAngle(PinNumber pin, FLOAT destAngleDegrees)
{
    FLOAT duty = ((SERVO_DUTY_CYCLE_MS_180 - SERVO_DUTY_CYCLE_MS_0) / SERVO_MAX_ANGLE_DEG * destAngleDegrees + SERVO_DUTY_CYCLE_MS_0) * SERVO_FREQ_HZ / 1000.0f;

    return pwmSetDuty(pin, SERVO_FREQ_HZ, duty);
}