#include "test_flash.h"
#include "flash_control.h"
#include <stdio.h>
#include <stdlib.h>

#define FLASH_TEST_FILE_NAME "test_flash.txt"

MY_TEST_INIT_FUNC(FLASH_TEST_NAME)
{
    flashInitFile(FLASH_TEST_FILE_NAME);
    flashFlushFile(FLASH_TEST_FILE_NAME);

    MY_TEST_END();
}

MY_TEST_FUNC(FLASH_TEST_NAME, 1)
{
    uint8_t data[FLASH_WRITE_BUFFER_SIZE];

    for (size_t i = 0; i < FLASH_WRITE_BUFFER_SIZE; i++)
    {
        data[i] = rand() >> 16;
    }

    flashWriteFile(FLASH_TEST_FILE_NAME, data);

    uint8_t *buffer;
    size_t size;
    flashGetFile(FLASH_TEST_FILE_NAME, &buffer, &size);

    MY_ASSERT(size == FLASH_WRITE_BUFFER_SIZE);

    int mismatchIndex = -1;

    for (size_t i = 0; i < FLASH_WRITE_BUFFER_SIZE; i++)
    {
        if (data[i] != buffer[i])
        {
            mismatchIndex = i;
            break;
        }
    }

    MY_ASSERT(mismatchIndex == -1);

    MY_TEST_END();
}