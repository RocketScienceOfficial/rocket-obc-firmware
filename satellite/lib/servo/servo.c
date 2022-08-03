#include "servo.h"
#include "logger.h"
#include "recorder.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

static const int PWM_FREQ_HZ = 125E6;
static const int PWM_CYCLE_TIME_NS = 1E9 / PWM_FREQ_HZ;
static const int PWM_DEFAULT_WRAP = 65535;
static const int MG995_FREQ_HZ = 50;
static const float MG995_PERIOD_MS = 1E3 / MG995_FREQ_HZ;
static const float MG995_DUTY_CYCLE_MS_0 = 0.5f;
static const float MG995_DUTY_CYCLE_MS_90 = 1.5f;
static const float MG995_DUTY_CYCLE_MS_180 = 2.5f;
static const float MG995_MAX_ANGLE_DEG = 180.0f;
static const float MG995_CLK_DIV = 64.0f;
static const float MG995_WRAP = ((MG995_PERIOD_MS * 1E6) / (PWM_CYCLE_TIME_NS * MG995_CLK_DIV));

void servoInit(servo_data_t *data)
{
    FUNCTION_PROFILE_BEGIN();

    gpio_set_function(data->pin, GPIO_FUNC_PWM);
    int slice_num = pwm_gpio_to_slice_num(data->pin);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, MG995_CLK_DIV);
    pwm_config_set_wrap(&config, MG995_WRAP);

    pwm_init(slice_num, &config, true);

    MY_LOG_CORE_INFO("Servo initialized");

    servoRotateAngle(data, 0.0f);

    FUNCTION_PROFILE_END();
}

void servoRotateAngle(servo_data_t *data, float destAngleDegrees)
{
    float duty = destAngleDegrees / MG995_MAX_ANGLE_DEG * (MG995_DUTY_CYCLE_MS_180 - MG995_DUTY_CYCLE_MS_0) + MG995_DUTY_CYCLE_MS_0;
    pwm_set_gpio_level(data->pin, MG995_WRAP * (duty / MG995_PERIOD_MS));
}