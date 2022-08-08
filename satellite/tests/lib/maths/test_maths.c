#include "test_maths.h"
#include "utils/cryptography.h"
#include <string.h>

MY_TEST_FUNC(MATHS_TEST_NAME, 1)
{
    uint8_t buffer[] = "Hello World!";
    uint8_t buffer2[] = "Hello World!";
    const uint8_t key[] = {0x30, 0x3A};

    MY_ASSERT(encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(uint8_t), key, sizeof(key) / sizeof(uint8_t)));
    MY_ASSERT(encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(uint8_t), key, sizeof(key) / sizeof(uint8_t)));
    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 2)
{
    uint8_t buffer[] = "Hello World!";
    uint8_t buffer2[] = "Hello World!";
    const uint8_t key[] = {0x48, 0x7D, 0x25};

    MY_ASSERT(encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(uint8_t), key, sizeof(key) / sizeof(uint8_t)));
    MY_ASSERT(encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(uint8_t), key, sizeof(key) / sizeof(uint8_t)));
    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 3)
{
    uint8_t buffer[] = "Hello World!";
    uint8_t buffer2[] = "Hello World!";
    const uint8_t key[] = {0x21, 0x2F, 0x5B, 0x7B};

    MY_ASSERT(encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(uint8_t), key, sizeof(key) / sizeof(uint8_t)));
    MY_ASSERT(encryptDecrypt(buffer2, sizeof(buffer2) / sizeof(uint8_t), key, sizeof(key) / sizeof(uint8_t)));
    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 4)
{
    uint8_t buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const uint8_t EXPECTED_PARITY_ROWS[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c};
    ParityData parityData = {0};

    MY_ASSERT(calculateParityRows(buffer, sizeof(buffer) / sizeof(uint8_t), &parityData));
    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_ROWS, sizeof(EXPECTED_PARITY_ROWS) / sizeof(uint8_t)) == 0);
    MY_ASSERT(clearParity(&parityData));

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 5)
{
    uint8_t buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const uint8_t EXPECTED_PARITY_COLUMNS[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    ParityData parityData = {0};

    MY_ASSERT(calculateParityColumns(buffer, sizeof(buffer) / sizeof(uint8_t), &parityData));
    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_COLUMNS, sizeof(EXPECTED_PARITY_COLUMNS) / sizeof(uint8_t)) == 0);
    MY_ASSERT(clearParity(&parityData));

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 6)
{
    uint8_t buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const uint8_t EXPECTED_PARITY[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    ParityData parityData = {0};

    MY_ASSERT(calculateParity(buffer, sizeof(buffer) / sizeof(uint8_t), &parityData));
    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY, sizeof(EXPECTED_PARITY) / sizeof(uint8_t)) == 0);
    MY_ASSERT(clearParity(&parityData));

    MY_TEST_END();
}

MY_TEST_FUNC(MATHS_TEST_NAME, 7)
{
    MY_ASSERT(encryptDecrypt(NULL, 0, NULL, 0) == false);
    MY_ASSERT(calculateParityRows(NULL, 0, NULL) == false);
    MY_ASSERT(calculateParityColumns(NULL, 0, NULL) == false);
    MY_ASSERT(calculateParity(NULL, 0, NULL) == false);
    MY_ASSERT(clearParity(NULL) == false);

    MY_TEST_END();
}