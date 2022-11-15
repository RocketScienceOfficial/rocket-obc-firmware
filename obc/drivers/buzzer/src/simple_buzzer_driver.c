#include "drivers/buzzer/simple_buzzer_driver.h"
#include "drivers/gpio/pwm_driver.h"

#define FREQUENCY 500

FUNCRESULT simpleBuzzerInit(PinNumber pin)
{
    return pwmInit(pin, FREQUENCY);
}