#include "test_servo.h"
#include "servo.h"
#include "pinout.h"

MY_TEST_INIT_FUNC(SERVO_TEST_NAME)
{
    servo_data_t data = {.pin = MG995_PIN};

    servoInit(&data);

    MY_TEST_END();
}