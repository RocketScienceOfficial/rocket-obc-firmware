#include "test_servo.h"
#include "mg995.h"
#include "pinout.h"

MY_TEST_INIT_FUNC(SERVO_TEST_NAME)
{
    mg995_data_t data = {.pin = MG995_PIN};

    mg995Init(&data);

    MY_TEST_END();
}