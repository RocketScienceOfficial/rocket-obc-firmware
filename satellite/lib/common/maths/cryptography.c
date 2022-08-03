#include "cryptography.h"
#include "logger.h"
#include "recorder.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

static size_t count1Bits(uint8_t n)
{
    FUNCTION_PROFILE_BEGIN();

    size_t count = 0;

    while (n)
    {
        count += n & 1;

        n >>= 1;
    }

    FUNCTION_PROFILE_END();

    return count;
}

void encryptDecrypt(uint8_t *buffer, size_t size, const uint8_t *key, size_t keySize)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Encrypting/decrypting buffer with size: %d...", size);

    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = buffer[i] ^ key[i % keySize];
    }

    FUNCTION_PROFILE_END();
}

void calculateParityRows(uint8_t *buffer, size_t size, parity_data_t *data_out_ptr)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Calculating parity rows...");

    data_out_ptr->size = (int)ceil(size / 8.0);
    data_out_ptr->buffer = (uint8_t *)calloc(data_out_ptr->size, sizeof(uint8_t));

    for (size_t i = 0; i < size; i++)
    {
        size_t oneBits = count1Bits(buffer[i]);
        data_out_ptr->buffer[i / 8] |= ((oneBits % 2) << (i % 8));
    }

    FUNCTION_PROFILE_END();
}

void calculateParityColumns(uint8_t *buffer, size_t size, parity_data_t *data_out_ptr)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Calculating parity columns...");

    data_out_ptr->size = (int)ceil(size / 8.0);
    data_out_ptr->buffer = (uint8_t *)calloc(data_out_ptr->size, sizeof(uint8_t));

    for (size_t i = 0; i < size; i += 8)
    {
        if (i == size)
        {
            break;
        }

        for (size_t j = 0; j < 8; j++)
        {
            uint8_t n = 0;

            for (size_t k = 0; k < 8; k++)
            {
                n |= (buffer[i + k] & (1 << j)) << j;
            }

            size_t oneBits = count1Bits(n);
            data_out_ptr->buffer[i / 8] |= (oneBits % 2) << (i % 8);
        }
    }

    FUNCTION_PROFILE_END();
}

void calculateParity(uint8_t *buffer, size_t size, parity_data_t *data_out_ptr)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Calculating parity...");

    parity_data_t rows = {0};
    parity_data_t columns = {0};

    calculateParityRows(buffer, size, &rows);
    calculateParityColumns(buffer, size, &columns);

    data_out_ptr->size = rows.size + columns.size;
    data_out_ptr->buffer = (uint8_t *)malloc(data_out_ptr->size * sizeof(uint8_t));

    memcpy(data_out_ptr->buffer, rows.buffer, rows.size);
    memcpy(data_out_ptr->buffer + rows.size, columns.buffer, columns.size);

    clearParity(&rows);
    clearParity(&columns);

    MY_LOG_CORE_INFO("Calculated parity with size: %d", data_out_ptr->size);

    FUNCTION_PROFILE_END();
}

void clearParity(parity_data_t *parity)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Clearing parity...");

    if (parity->size > 0)
    {
        free(parity->buffer);
    }

    FUNCTION_PROFILE_END();
}