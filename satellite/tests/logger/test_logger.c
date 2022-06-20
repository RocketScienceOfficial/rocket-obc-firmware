#include "test_logger.h"

MY_TEST_FUNC(LOGGER_TEST_NAME, 1)
{
    MY_LOG_CORE_INFO("Initializing...");
    MY_LOG_CORE_INFO("TEST NUMBER: %d", 123);
    MY_LOG_CORE_INFO("Done!");

    MY_TEST_END();
}

MY_TEST_FUNC(LOGGER_TEST_NAME, 2)
{
    MY_LOG_MEASURE_RECEIVER_INT("Test1", 123);
    MY_LOG_MEASURE_RECEIVER_FLOAT("Test2", 546.27f);

    MY_LOG_MEASURE_INT(10);
    MY_LOG_MEASURE_FLOAT(20.36f);

    MY_TEST_END();
}