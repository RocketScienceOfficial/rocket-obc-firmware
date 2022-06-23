#include "pico/stdlib.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "console_commands.h"
#include "remote_commands.h"

#include "test_commands.h"
#include "test_console.h"
#include "test_logger.h"
#include "test_maths.h"
#include "test_bmp280.h"
#include "test_mpu6050.h"
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
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);

    attachSerialToLog(myLogGetCoreLogger());
    attachSerialToLog(myLogGetMeasureLogger());
    attachSerialToLog(myLogGetCommandOutputLogger());
    attachSerialToLog(myLogGetRemoteCommandLogger());
}

void initialize()
{
    MY_LOG_CORE_INFO("Initializing...");

    MY_TEST_CASE_INIT_FUNC(COMMANDS_TEST_NAME);
    MY_TEST_CASE_INIT_FUNC(BMP280_TEST_NAME);
    MY_TEST_CASE_INIT_FUNC(MPU6050_TEST_NAME);
    MY_TEST_CASE_INIT_FUNC(RADIO_RECEIVER_NAME);
    MY_TEST_CASE_INIT_FUNC(RADIO_SENDER_NAME);
    MY_TEST_CASE_INIT_FUNC(SD_TEST_NAME);
    MY_TEST_CASE_INIT_FUNC(TIME_TRACKER_TEST_NAME);

    MY_LOG_CORE_INFO("Initialized!");
}

void loop()
{
    MY_TEST_CASE_FUNC_DYNAMIC(CONSOLE_TEST_NAME, 1);
    MY_TEST_CASE_FUNC_DYNAMIC(BMP280_TEST_NAME, 1);
    MY_TEST_CASE_FUNC_DYNAMIC(MPU6050_TEST_NAME, 1);
    MY_TEST_CASE_FUNC_DYNAMIC(RADIO_RECEIVER_NAME, 1);
    MY_TEST_CASE_FUNC_DYNAMIC(RADIO_SENDER_NAME, 1);
    MY_TEST_CASE_FUNC_DYNAMIC(SD_TEST_NAME, 1);
}