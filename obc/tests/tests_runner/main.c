#include "pico/stdlib.h"
#include "test_framework.h"
#include "kernel/logging/logger.h"
#include "drivers/console/console_output.h"

#include "test_console.h"
#include "test_flash.h"
#include "test_commands.h"
#include "test_logger.h"
#include "test_radio_protocol.h"
#include "test_cryptography.h"

void start();
void initialize();
void runTests();
void loop();

int main()
{
    start();

    initialize();
    runTests();

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
    MY_TEST_CASE_INIT_FUNC(FLASH_TEST_NAME);
    MY_TEST_CASE_INIT_FUNC(COMMANDS_TEST_NAME);
    MY_TESTS_END();

    MY_LOG_CORE_INFO("Initialized!");
}

void runTests()
{
    MY_LOG_CORE_INFO("Running normal tests...");

    MY_TESTS_BEGIN();
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 3);
    MY_TEST_CASE_FUNC(FLASH_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(FLASH_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(COMMANDS_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(COMMANDS_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(COMMANDS_TEST_NAME, 3);
    MY_TEST_CASE_FUNC(LOGGER_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(RADIO_PROTOCOL_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(RADIO_PROTOCOL_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 3);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 4);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 5);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 6);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 7);
    MY_TESTS_END();

    MY_LOG_CORE_INFO("Tests finished!");
}

void loop()
{
    tight_loop_contents();
}