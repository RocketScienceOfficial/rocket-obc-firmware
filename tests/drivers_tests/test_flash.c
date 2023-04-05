#include "test_flash.h"
#include "drivers/storage/flash_driver.h"
#include <obc/api.h>
#include <stdio.h>
#include <stdlib.h>

#define FLASH_TEST_FILE_INDEX 0

MY_TEST_INIT_FUNC(FLASH_TEST_NAME)
{
    MY_ASSERT(FUNCSUCCESS(flashClearFile(flashGetDefaultModule(), FLASH_TEST_FILE_INDEX)));

    MY_TEST_END();
}

MY_TEST_FUNC(FLASH_TEST_NAME, 1)
{
    SIZE dataSize = 260;
    SIZE expectedDataSize = 256;
    BYTE data[dataSize];

    for (SIZE i = 0; i < dataSize; i++)
    {
        data[i] = rand() >> 16;
    }

    MY_ASSERT(FUNCSUCCESS(flashWriteFile(flashGetDefaultModule(), FLASH_TEST_FILE_INDEX, data)));

    const BYTE *buffer;
    SIZE size;

    MY_ASSERT(FUNCSUCCESS(flashGetFile(flashGetDefaultModule(), FLASH_TEST_FILE_INDEX, &buffer, &size)));
    MY_ASSERT(size == expectedDataSize);

    INT32 mismatchIndex = -1;

    for (SIZE i = 0; i < size; i++)
    {
        if (data[i] != buffer[i])
        {
            mismatchIndex = i;
            break;
        }
    }

    MY_ASSERT(mismatchIndex == -1);
    MY_ASSERT(FUNCSUCCESS(flashClearFile(flashGetDefaultModule(), FLASH_TEST_FILE_INDEX)));
    MY_ASSERT(FUNCSUCCESS(flashTerminate(flashGetDefaultModule())));

    MY_TEST_END();
}

MY_TEST_FUNC(FLASH_TEST_NAME, 2)
{
    MY_ASSERT(FUNCFAILED(flashInit(NULL)));
    MY_ASSERT(FUNCFAILED(flashWriteFile(NULL, FLASH_FILES_COUNT + 1, NULL)));
    MY_ASSERT(FUNCFAILED(flashClearFile(NULL, FLASH_FILES_COUNT + 1)));
    MY_ASSERT(FUNCFAILED(flashTerminate(NULL)));
    MY_ASSERT(flashGetDefaultModule() != NULL);

    MY_TEST_END();
}