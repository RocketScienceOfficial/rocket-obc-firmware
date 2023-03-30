#include "test_cryptography.h"
#include <obc/api.h>
#include "crypto/xor_encryption.h"
#include "crypto/parity_check.h"
#include <string.h>

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 1)
{
    BYTE buffer[] = "Hello World!";
    BYTE buffer2[] = "Hello World!";
    const BYTE key[] = {0x30, 0x3A};

    MY_ASSERT(encryptDecryptXOR(buffer2, sizeof(buffer2) / sizeof(BYTE), key, sizeof(key) / sizeof(BYTE)));
    MY_ASSERT(encryptDecryptXOR(buffer2, sizeof(buffer2) / sizeof(BYTE), key, sizeof(key) / sizeof(BYTE)));
    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 2)
{
    BYTE buffer[] = "Hello World!";
    BYTE buffer2[] = "Hello World!";
    const BYTE key[] = {0x48, 0x7D, 0x25};

    MY_ASSERT(encryptDecryptXOR(buffer2, sizeof(buffer2) / sizeof(BYTE), key, sizeof(key) / sizeof(BYTE)));
    MY_ASSERT(encryptDecryptXOR(buffer2, sizeof(buffer2) / sizeof(BYTE), key, sizeof(key) / sizeof(BYTE)));
    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 3)
{
    BYTE buffer[] = "Hello World!";
    BYTE buffer2[] = "Hello World!";
    const BYTE key[] = {0x21, 0x2F, 0x5B, 0x7B};

    MY_ASSERT(encryptDecryptXOR(buffer2, sizeof(buffer2) / sizeof(BYTE), key, sizeof(key) / sizeof(BYTE)));
    MY_ASSERT(encryptDecryptXOR(buffer2, sizeof(buffer2) / sizeof(BYTE), key, sizeof(key) / sizeof(BYTE)));
    MY_ASSERT(strcmp(buffer, buffer2) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 4)
{
    BYTE buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const BYTE EXPECTED_PARITY_ROWS[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c};
    ParityData parityData = {0};

    MY_ASSERT(calculateParityRows(buffer, sizeof(buffer) / sizeof(BYTE), &parityData));
    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_ROWS, sizeof(EXPECTED_PARITY_ROWS) / sizeof(BYTE)) == 0);
    MY_ASSERT(clearParity(&parityData));

    MY_TEST_END();
}

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 5)
{
    BYTE buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const BYTE EXPECTED_PARITY_COLUMNS[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    ParityData parityData = {0};

    MY_ASSERT(calculateParityColumns(buffer, sizeof(buffer) / sizeof(BYTE), &parityData));
    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY_COLUMNS, sizeof(EXPECTED_PARITY_COLUMNS) / sizeof(BYTE)) == 0);
    MY_ASSERT(clearParity(&parityData));

    MY_TEST_END();
}

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 6)
{
    BYTE buffer[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    const BYTE EXPECTED_PARITY[] = {0xcb, 0xb4, 0x34, 0x2f, 0xd3, 0xd2, 0x0c, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    ParityData parityData = {0};

    MY_ASSERT(calculateParity(buffer, sizeof(buffer) / sizeof(BYTE), &parityData));
    MY_ASSERT(memcmp(parityData.buffer, EXPECTED_PARITY, sizeof(EXPECTED_PARITY) / sizeof(BYTE)) == 0);
    MY_ASSERT(clearParity(&parityData));

    MY_TEST_END();
}

MY_TEST_FUNC(CRYPTOGRAPHY_TEST_NAME, 7)
{
    MY_ASSERT(encryptDecryptXOR(NULL, 0, NULL, 0) == FALSE);
    MY_ASSERT(calculateParityRows(NULL, 0, NULL) == FALSE);
    MY_ASSERT(calculateParityColumns(NULL, 0, NULL) == FALSE);
    MY_ASSERT(calculateParity(NULL, 0, NULL) == FALSE);
    MY_ASSERT(clearParity(NULL) == FALSE);

    MY_TEST_END();
}