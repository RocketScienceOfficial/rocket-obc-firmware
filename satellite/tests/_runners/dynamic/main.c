#include "pico/stdlib.h"
#include "test_framework.h"
#include "config.h"
#include "logging/logger.h"
#include "logging/log_serial.h"

#include "test_accelerometer.h"
#include "test_barometer.h"
#include "test_commands.h"
#include "test_console.h"
#include "test_flash.h"
#include "test_logger.h"
#include "test_maths.h"
#include "test_pinout_utils.h"
#include "test_radio_protocol.h"
#include "test_radio_receiver.h"
#include "test_radio_sender.h"
#include "test_sd.h"
#include "test_servo.h"
#include "test_time_tracker.h"

void start();
void initialize();
void loop();

int main()
{
    start();
    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);
}

void initialize()
{
    MY_LOG_CORE_INFO("Initializing...");

    MY_TESTS_BEGIN();

    MY_TEST_CASE_INIT_FUNC(COMMANDS_TEST_NAME);

    if (ENABLE_BAROMETER)
    {
        MY_TEST_CASE_INIT_FUNC(BAROMETER_TEST_NAME);
    }

    if (ENABLE_ACCELEROMETER)
    {
        MY_TEST_CASE_INIT_FUNC(ACCELEROMETER_TEST_NAME);
    }

    if (ENABLE_RADIO)
    {
        MY_TEST_CASE_INIT_FUNC(RADIO_RECEIVER_NAME);
        MY_TEST_CASE_INIT_FUNC(RADIO_SENDER_NAME);
    }

    if (ENABLE_SD)
    {
        MY_TEST_CASE_INIT_FUNC(SD_TEST_NAME);
    }

    if (ENABLE_SERVO)
    {
        MY_TEST_CASE_INIT_FUNC(SERVO_TEST_NAME);
    }

    MY_TEST_CASE_INIT_FUNC(TIME_TRACKER_TEST_NAME);

    MY_TESTS_END();

    MY_LOG_CORE_INFO("Initialized!");
}

void loop()
{
    MY_TEST_CASE_FUNC_DYNAMIC(CONSOLE_TEST_NAME, 1);

    if (ENABLE_BAROMETER)
    {
        MY_TEST_CASE_FUNC_DYNAMIC(BAROMETER_TEST_NAME, 1);
    }

    if (ENABLE_ACCELEROMETER)
    {
        MY_TEST_CASE_FUNC_DYNAMIC(ACCELEROMETER_TEST_NAME, 1);
    }

    if (ENABLE_RADIO)
    {
        MY_TEST_CASE_FUNC_DYNAMIC(RADIO_RECEIVER_NAME, 1);
        MY_TEST_CASE_FUNC_DYNAMIC(RADIO_SENDER_NAME, 1);
    }

    if (ENABLE_SD)
    {
        MY_TEST_CASE_FUNC_DYNAMIC(SD_TEST_NAME, 1);
    }
}