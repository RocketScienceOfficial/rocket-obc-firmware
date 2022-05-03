#include <stdio.h>
#include "mg995.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

const int PWM_FREQ_HZ = 125E6;
const int PWM_CYCLE_TIME_NS = 1E9 / PWM_FREQ_HZ;
const int PWM_DEFAULT_WRAP = 65535;
const int MG995_FREQ_HZ = 50;
const float MG995_PERIOD_MS = 1E3 / MG995_FREQ_HZ;
const float MG995_DUTY_CYCLE_MS_0 = 0.5f;
const float MG995_DUTY_CYCLE_MS_90 = 1.5f;
const float MG995_DUTY_CYCLE_MS_180 = 2.5f;
const float MG995_MAX_ANGLE_DEG = 180.0f;
const float MG995_CLK_DIV = 64.0f;
const float MG995_WRAP = ((MG995_PERIOD_MS * 1E6) / (PWM_CYCLE_TIME_NS * MG995_CLK_DIV));

void mg995Init(mg995_data_t *data)
{
    gpio_set_function(data->pin, GPIO_FUNC_PWM);
    int slice_num = pwm_gpio_to_slice_num(data->pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, MG995_CLK_DIV);
    pwm_config_set_wrap(&config, MG995_WRAP);

    pwm_init(slice_num, &config, true);

    mg995RotateAngle(data, 0.0f);
}

void mg995RotateAngle(mg995_data_t *data, float angleDegrees)
{
    float duty = angleDegrees / MG995_MAX_ANGLE_DEG * (MG995_DUTY_CYCLE_MS_180 - MG995_DUTY_CYCLE_MS_0) + MG995_DUTY_CYCLE_MS_0;
    pwm_set_gpio_level(data->pin, MG995_WRAP * (duty / MG995_PERIOD_MS));
}