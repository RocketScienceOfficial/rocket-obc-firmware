#include "voltage_control.h"
#include "config.h"
#include "drivers/gpio/gpio_driver.h"

VOID voltageInit(VOID)
{
    gpioInitPin(ENABLE_3V3_PIN, GPIO_OUTPUT);
    gpioInitPin(ENABLE_5V_PIN, GPIO_OUTPUT);
    gpioInitPin(ENABLE_VBAT_PIN, GPIO_OUTPUT);
}

VOID voltageSet(VoltageType type, BOOL enable)
{
    PinNumber pin = 0;

    switch (type)
    {
    case VOLTAGE_3V3:
        pin = ENABLE_3V3_PIN;
        break;
    case VOLTAGE_5V:
        pin = ENABLE_5V_PIN;
        break;
    case VOLTAGE_VBAT:
        pin = ENABLE_VBAT_PIN;
        break;
    default:
        return;
    }

    gpioSetPinState(pin, enable ? GPIO_HIGH : GPIO_LOW);
}