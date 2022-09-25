#include "tools/cryptography.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

static SIZE count1Bits(BYTE n)
{
    SIZE count = 0;

    while (n)
    {
        count += n & 1;

        n >>= 1;
    }

    return count;
}

BOOL encryptDecrypt(BYTE *buffer, SIZE size, const BYTE *key, SIZE keySize)
{
    if (!buffer || size == 0 || !key || keySize == 0)
    {
        return FALSE;
    }

    for (SIZE i = 0; i < size; i++)
    {
        buffer[i] = buffer[i] ^ key[i % keySize];
    }

    return TRUE;
}

BOOL calculateParityRows(const BYTE *buffer, SIZE size, ParityData *data_out_ptr)
{
    if (!buffer || size == 0 || !data_out_ptr)
    {
        return FALSE;
    }

    data_out_ptr->size = (SIZE)ceil(size / 8.0);
    data_out_ptr->buffer = (BYTE *)calloc(data_out_ptr->size, sizeof(BYTE));

    for (SIZE i = 0; i < size; i++)
    {
        SIZE oneBits = count1Bits(buffer[i]);
        data_out_ptr->buffer[i / 8] |= ((oneBits % 2) << (i % 8));
    }

    return TRUE;
}

BOOL calculateParityColumns(const BYTE *buffer, SIZE size, ParityData *data_out_ptr)
{
    if (!buffer || size == 0 || !data_out_ptr)
    {
        return FALSE;
    }

    data_out_ptr->size = (SIZE)ceil(size / 8.0);
    data_out_ptr->buffer = (BYTE *)calloc(data_out_ptr->size, sizeof(BYTE));

    for (SIZE i = 0; i < size; i += 8)
    {
        if (i == size)
        {
            break;
        }

        for (SIZE j = 0; j < 8; j++)
        {
            BYTE n = 0;

            for (SIZE k = 0; k < 8; k++)
            {
                n |= (buffer[i + k] & (1 << j)) << j;
            }

            SIZE oneBits = count1Bits(n);
            data_out_ptr->buffer[i / 8] |= (oneBits % 2) << (i % 8);
        }
    }

    return TRUE;
}

BOOL calculateParity(const BYTE *buffer, SIZE size, ParityData *data_out_ptr)
{
    if (!buffer || size == 0 || !data_out_ptr)
    {
        return FALSE;
    }

    BOOL result = TRUE;

    ParityData rows = {0};
    ParityData columns = {0};

    result = result && calculateParityRows(buffer, size, &rows);
    result = result && calculateParityColumns(buffer, size, &columns);

    data_out_ptr->size = rows.size + columns.size;
    data_out_ptr->buffer = (BYTE *)malloc(data_out_ptr->size * sizeof(BYTE));

    memcpy(data_out_ptr->buffer, rows.buffer, rows.size);
    memcpy(data_out_ptr->buffer + rows.size, columns.buffer, columns.size);

    result = result && clearParity(&rows);
    result = result && clearParity(&columns);

    return result;
}

BOOL clearParity(ParityData *parity)
{
    if (!parity)
    {
        return FALSE;
    }

    if (parity->size > 0)
    {
        free(parity->buffer);

        parity->size = 0;
    }

    return TRUE;
}