#include "drivers/servo/mg995_driver.h"
#include "drivers/gpio/pwm_driver.h"

static const UINT32 MG995_FREQ_HZ = 50;
static const FLOAT MG995_DUTY_CYCLE_MS_0 = 0.5f;
static const FLOAT MG995_DUTY_CYCLE_MS_90 = 1.5f;
static const FLOAT MG995_DUTY_CYCLE_MS_180 = 2.5f;
static const FLOAT MG995_MAX_ANGLE_DEG = 180.0f;

FUNCRESULT mg995Init(PinNumber pin)
{
    return pwmInit(pin, MG995_FREQ_HZ);
}

FUNCRESULT mg995RotateAngle(PinNumber pin, FLOAT destAngleDegrees)
{
    FLOAT duty = ((MG995_DUTY_CYCLE_MS_180 - MG995_DUTY_CYCLE_MS_0) / MG995_MAX_ANGLE_DEG * destAngleDegrees + MG995_DUTY_CYCLE_MS_0) * MG995_FREQ_HZ / 1000.0f;

    return pwmSetDuty(pin, MG995_FREQ_HZ, duty);
}