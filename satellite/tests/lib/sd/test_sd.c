#include "test_sd.h"
#include "myhardware/sd.h"
#include "logging/logger.h"
#include "utils/time_tracker.h"

#define TEST_FILENAME "test.txt"

static SDCard s_SDCard;
static Timer s_TimerOffset = 0;

MY_TEST_INIT_FUNC(SD_TEST_NAME)
{
    MY_ASSERT(FUNCSUCCESS(sdInit(&s_SDCard)));
    MY_ASSERT(FUNCSUCCESS(sdInitFile(&s_SDCard, TEST_FILENAME)));
    MY_ASSERT(FUNCSUCCESS(sdClearFile(&s_SDCard, TEST_FILENAME)));

    MY_TEST_END();
}

MY_TEST_FUNC(SD_TEST_NAME, 1)
{
    MY_ASSERT(FUNCSUCCESS(sdBegin(&s_SDCard, TEST_FILENAME)));
    MY_ASSERT(FUNCSUCCESS(sdWrite(&s_SDCard, "Hello World!", TEST_FILENAME)));
    MY_ASSERT(FUNCSUCCESS(sdEnd(&s_SDCard, TEST_FILENAME)));
    MY_ASSERT(FUNCSUCCESS(sdTerminate(&s_SDCard)));

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(SD_TEST_NAME, 1)
{
    if (runEvery(1000, &s_TimerOffset))
    {
        MY_LOG_CORE_INFO("Writing 'Hello World!' to SD card...");

        MY_ASSERT(FUNCSUCCESS(sdBegin(&s_SDCard, TEST_FILENAME)));
        MY_ASSERT(FUNCSUCCESS(sdWrite(&s_SDCard, "Hello World!", TEST_FILENAME)));
        MY_ASSERT(FUNCSUCCESS(sdEnd(&s_SDCard, TEST_FILENAME)));
    }

    MY_TEST_END();
}