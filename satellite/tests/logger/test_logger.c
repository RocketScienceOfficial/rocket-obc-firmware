#include "test_logger.h"
#include "logger.h"

MY_TEST_FUNC(LOGGER_TEST_NAME, 1)
{
    MY_LOG_CORE_INFO("Initializing...");
    MY_LOG_CORE_INFO("TEST NUMBER: %d", 123);
    MY_LOG_CORE_INFO("Done!");

    MY_TEST_END();
}