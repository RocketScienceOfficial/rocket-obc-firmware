#include "pico/stdlib.h"
#include "test_framework.h"
#include "kernel/logging/logger.h"
#include "drivers/console/console_output.h"

#include "test_cryptography.h"
#include "test_commands.h"
#include "test_logger.h"
#include "test_radio_protocol.h"
#include "test_console.h"
#include "test_flash.h"

static VOID intialize();
static VOID run();

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    intialize();
    run();

    while (TRUE)
    {
        tight_loop_contents();
    }

    return 0;
}

static VOID intialize()
{
    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    MY_LOG_CORE_INFO("Initializing...");

    MY_TESTS_BEGIN();

    MY_TEST_CASE_INIT_FUNC(COMMANDS_TEST_NAME);
    MY_TEST_CASE_INIT_FUNC(FLASH_TEST_NAME);

    MY_TESTS_END();

    MY_LOG_CORE_INFO("Initialized!");
}

static VOID run()
{
    MY_LOG_CORE_INFO("Running tests...");

    MY_TESTS_BEGIN();

    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 3);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 4);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 5);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 6);
    MY_TEST_CASE_FUNC(CRYPTOGRAPHY_TEST_NAME, 7);

    MY_TEST_CASE_FUNC(COMMANDS_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(COMMANDS_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(COMMANDS_TEST_NAME, 3);

    MY_TEST_CASE_FUNC(LOGGER_TEST_NAME, 1);

    MY_TEST_CASE_FUNC(RADIO_PROTOCOL_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(RADIO_PROTOCOL_TEST_NAME, 2);

    MY_TEST_CASE_FUNC(FLASH_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(FLASH_TEST_NAME, 2);

    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 3);

    MY_TESTS_END();

    MY_LOG_CORE_INFO("Tests finished!");
}