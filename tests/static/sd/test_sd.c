#include "test_sd.h"
#include "sd.h"
#include "logger.h"

#define TEST_FILENAME "test.txt"

MY_TEST_INIT_FUNC(SD_TEST_NAME)
{
    MY_ASSERT(sdInit());
    sdInitFile(TEST_FILENAME);
    MY_ASSERT(sdFlush(TEST_FILENAME));
    sdAttachToCoreLogger(TEST_FILENAME);

    MY_TEST_END();
}

MY_TEST_FUNC(SD_TEST_NAME, 1)
{
    MY_ASSERT(sdBegin(TEST_FILENAME));

    MY_LOG_CORE_INFO("Hello world!");
    
    MY_ASSERT(sdEnd(TEST_FILENAME));

    MY_TEST_END();
}