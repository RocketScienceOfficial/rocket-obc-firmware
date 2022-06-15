#include "test_maths.h"
#include "cryptography.h"
#include <string.h>

MY_TEST_FUNC(MATHS_TEST_NAME, 1)
{
    char buffer[] = "Hello World!";
    char buffer2[] = "Hello World!";
    char key[] = {0x30, 0x3A};

    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));
    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));

    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 2)
{
    char buffer[] = "Hello World!";
    char buffer2[] = "Hello World!";
    char key[] = {0x48, 0x7D, 0x25};

    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));
    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));

    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 3)
{
    char buffer[] = "Hello World!";
    char buffer2[] = "Hello World!";
    char key[] = {0x21, 0x2F, 0x5B, 0x7B};

    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));
    encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(char), key, sizeof(key) / sizeof(char));

    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 4)
{
    char buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const char EXPECTED_PARITY_ROWS[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c};
    parity_data_t parityData = {0};

    calculateParityRows(buffer, sizeof(buffer) / sizeof(char), &parityData);

    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_ROWS, sizeof(EXPECTED_PARITY_ROWS) / sizeof(char)) == 0);

    clearParity(&parityData);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 5)
{
    char buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const char EXPECTED_PARITY_COLUMNS[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    parity_data_t parityData = {0};

    calculateParityColumns(buffer, sizeof(buffer) / sizeof(char), &parityData);

    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_COLUMNS, sizeof(EXPECTED_PARITY_COLUMNS) / sizeof(char)) == 0);

    clearParity(&parityData);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 6)
{
    char buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const char EXPECTED_PARITY[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    parity_data_t parityData = {0};

    calculateParity(buffer, sizeof(buffer) / sizeof(char), &parityData);

    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY, sizeof(EXPECTED_PARITY) / sizeof(char)) == 0);

    clearParity(&parityData);

    MY_TEST_END();
}