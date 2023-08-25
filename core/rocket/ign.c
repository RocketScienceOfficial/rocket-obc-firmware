#include "ign.h"
#include "config.h"
#include "mission_control.h"
#include "drivers/gpio/gpio_driver.h"

static BOOL s_IgnitersFired;

VOID initIgn(VOID)
{
    gpioInitPin(IGNITER_1_PIN, GPIO_OUTPUT);
    gpioInitPin(IGNITER_2_PIN, GPIO_OUTPUT);
    gpioInitPin(IGNITER_3_PIN, GPIO_OUTPUT);
    gpioInitPin(IGNITER_4_PIN, GPIO_OUTPUT);
}

VOID checkIgn(VOID)
{
    if (isMissionDone())
    {
        if (s_IgnitersFired)
        {
            gpioSetPinState(IGNITER_1_PIN, GPIO_HIGH);
            gpioSetPinState(IGNITER_2_PIN, GPIO_HIGH);
            gpioSetPinState(IGNITER_3_PIN, GPIO_HIGH);
            gpioSetPinState(IGNITER_4_PIN, GPIO_HIGH);

            s_IgnitersFired = TRUE;
        }
    }
}