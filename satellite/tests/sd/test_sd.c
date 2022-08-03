#include "test_sd.h"
#include "sd.h"
#include "logger.h"
#include "time_tracker.h"

#define TEST_FILENAME "test.txt"

static timer_t s_TimerOffset = 0;

MY_TEST_INIT_FUNC(SD_TEST_NAME)
{
    MY_ASSERT(sdInit());
    sdInitFile(TEST_FILENAME);
    MY_ASSERT(sdFlush(TEST_FILENAME));

    MY_TEST_END();
}

MY_TEST_FUNC(SD_TEST_NAME, 1)
{
    MY_ASSERT(sdBegin(TEST_FILENAME));
    MY_ASSERT(sdWrite("Hello World!", TEST_FILENAME));
    MY_ASSERT(sdEnd(TEST_FILENAME));

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(SD_TEST_NAME, 1)
{
    if (runEvery(1000, &s_TimerOffset))
    {
        MY_ASSERT(sdBegin(TEST_FILENAME));
        MY_ASSERT(sdWrite("Hello World!", TEST_FILENAME));
        MY_ASSERT(sdEnd(TEST_FILENAME));
    }

    MY_TEST_END();
}