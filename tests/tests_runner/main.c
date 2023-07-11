#include "drivers/tools/board_control.h"
#include "test_framework.h"
#include "services/logging/logger.h"
#include "drivers/console/console_output.h"

#include "test_logger.h"
#include "test_radio_protocol.h"
#include "test_console.h"
#include "test_nmea.h"

static VOID intialize();
static VOID run();

int main()
{
    boardInit(10000);

    intialize();
    run();

    while (TRUE)
    {
        tightLoop();
    }

    return 0;
}

static VOID intialize()
{
    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    MY_TESTS_BEGIN("Initialization");

    MY_TESTS_END();
}

static VOID run()
{
    MY_TESTS_BEGIN("Run");

    MY_TEST_CASE_FUNC(LOGGER_TEST_NAME, 1);

    MY_TEST_CASE_FUNC(RADIO_PROTOCOL_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(RADIO_PROTOCOL_TEST_NAME, 2);

    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 1);
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 2);
    MY_TEST_CASE_FUNC(CONSOLE_TEST_NAME, 3);

    MY_TEST_CASE_FUNC(NMEA_TEST_NAME, 1);

    MY_TESTS_END();
}