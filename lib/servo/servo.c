#include <stdio.h>
#include "servo.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

void setMillis(int servoPin, float millis)
{
    pwm_set_gpio_level(servoPin, (millis / 20000.f) * 39062.0f);
}

void setServo(int servoPin, float startMillis)
{
    gpio_set_function(servoPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(servoPin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 64.0f);
    pwm_config_set_wrap(&config, 39062.0f);

    pwm_init(slice_num, &config, true);

    setMillis(servoPin, startMillis);
}