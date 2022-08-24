#include "test_flash.h"
#include "myhardware/flash.h"
#include <stdio.h>
#include <stdlib.h>

#define FLASH_TEST_FILE_INDEX 0

MY_TEST_INIT_FUNC(FLASH_TEST_NAME)
{
    MY_ASSERT(FUNCSUCCESS(flashClearFile(getDefaultFlashModule(), FLASH_TEST_FILE_INDEX)));

    MY_TEST_END();
}

MY_TEST_FUNC(FLASH_TEST_NAME, 1)
{
    size_t dataSize = 260;
    size_t expectedDataSize = 256;
    uint8_t data[dataSize];

    for (size_t i = 0; i < dataSize; i++)
    {
        data[i] = rand() >> 16;
    }

    MY_ASSERT(FUNCSUCCESS(flashWriteFile(getDefaultFlashModule(), FLASH_TEST_FILE_INDEX, data)));

    const uint8_t *buffer;
    size_t size;

    MY_ASSERT(FUNCSUCCESS(flashGetFile(getDefaultFlashModule(), FLASH_TEST_FILE_INDEX, &buffer, &size)));
    MY_ASSERT(size == expectedDataSize);

    int mismatchIndex = -1;

    for (size_t i = 0; i < size; i++)
    {
        if (data[i] != buffer[i])
        {
            mismatchIndex = i;
            break;
        }
    }

    MY_ASSERT(mismatchIndex == -1);
    MY_ASSERT(FUNCSUCCESS(flashClearFile(getDefaultFlashModule(), FLASH_TEST_FILE_INDEX)));
    MY_ASSERT(FUNCSUCCESS(flashTerminate(getDefaultFlashModule())));

    MY_TEST_END();
}

MY_TEST_FUNC(FLASH_TEST_NAME, 2)
{
    MY_ASSERT(FUNCFAILED(flashInit(NULL)));
    MY_ASSERT(FUNCFAILED(flashWriteFile(NULL, FLASH_FILES_COUNT + 1, NULL)));
    MY_ASSERT(FUNCFAILED(flashClearFile(NULL, FLASH_FILES_COUNT + 1)));
    MY_ASSERT(FUNCFAILED(flashTerminate(NULL)));
    MY_ASSERT(getDefaultFlashModule() != NULL);

    MY_TEST_END();
}