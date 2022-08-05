#include "cryptography.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

static size_t count1Bits(uint8_t n)
{
    size_t count = 0;

    while (n)
    {
        count += n & 1;

        n >>= 1;
    }

    return count;
}

bool encryptDecrypt(uint8_t *buffer, size_t size, const uint8_t *key, size_t keySize)
{
    if (!buffer || size == 0 || !key || keySize == 0)
    {
        return false;
    }

    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = buffer[i] ^ key[i % keySize];
    }

    return true;
}

bool calculateParityRows(uint8_t *buffer, size_t size, ParityData *data_out_ptr)
{
    if (!buffer || size == 0 || !data_out_ptr)
    {
        return false;
    }

    data_out_ptr->size = (int)ceil(size / 8.0);
    data_out_ptr->buffer = (uint8_t *)calloc(data_out_ptr->size, sizeof(uint8_t));

    for (size_t i = 0; i < size; i++)
    {
        size_t oneBits = count1Bits(buffer[i]);
        data_out_ptr->buffer[i / 8] |= ((oneBits % 2) << (i % 8));
    }

    return true;
}

bool calculateParityColumns(uint8_t *buffer, size_t size, ParityData *data_out_ptr)
{
    if (!buffer || size == 0 || !data_out_ptr)
    {
        return false;
    }

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

    return true;
}

bool calculateParity(uint8_t *buffer, size_t size, ParityData *data_out_ptr)
{
    if (!buffer || size == 0 || !data_out_ptr)
    {
        return false;
    }

    bool result = false;

    ParityData rows = {0};
    ParityData columns = {0};

    result = result && calculateParityRows(buffer, size, &rows);
    result = result && calculateParityColumns(buffer, size, &columns);

    data_out_ptr->size = rows.size + columns.size;
    data_out_ptr->buffer = (uint8_t *)malloc(data_out_ptr->size * sizeof(uint8_t));

    memcpy(data_out_ptr->buffer, rows.buffer, rows.size);
    memcpy(data_out_ptr->buffer + rows.size, columns.buffer, columns.size);

    result = result && clearParity(&rows);
    result = result && clearParity(&columns);

    return result;
}

bool clearParity(ParityData *parity)
{
    if (!parity)
    {
        return false;
    }

    if (parity->size > 0)
    {
        free(parity->buffer);

        parity->size = 0;
    }

    return true;
}