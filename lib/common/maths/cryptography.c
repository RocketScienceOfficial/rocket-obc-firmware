#include "cryptography.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

static size_t count1Bits(char n)
{
    size_t count = 0;

    while (n)
    {
        count += n & 1;

        n >>= 1;
    }

    return count;
}

void encryptDecrypt(char *buffer, size_t size, const char *key, size_t keySize)
{
    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = buffer[i] ^ key[i % keySize];
    }
}

void calculateParityRows(char *buffer, size_t size, char **parityOut, size_t *paritySizeOut)
{
    *paritySizeOut = (int)ceil(size / 8.0);
    *parityOut = (char *)calloc(*paritySizeOut, sizeof(char));

    for (size_t i = 0; i < size; i++)
    {
        size_t oneBits = count1Bits(buffer[i]);
        (*parityOut)[i / 8] |= ((oneBits % 2) << (i % 8));
    }
}

void calculateParityColumns(char *buffer, size_t size, char **parityOut, size_t *paritySizeOut)
{
    *paritySizeOut = (int)ceil(size / 8.0);
    *parityOut = (char *)calloc(*paritySizeOut, sizeof(char));

    for (size_t i = 0; i < size; i += 8)
    {
        if (i == size)
        {
            break;
        }

        for (size_t j = 0; j < 8; j++)
        {
            char n = 0;

            for (size_t k = 0; k < 8; k++)
            {
                n |= (buffer[i + k] & (1 << j)) << j;
            }

            size_t oneBits = count1Bits(n);
            (*parityOut)[i / 8] |= (oneBits % 2) << (i % 8);
        }
    }
}

void calculateParity(char *buffer, size_t size, char **parityOut, size_t *paritySizeOut)
{
    char *parityRows = NULL;
    size_t parityRowsSize = 0;
    char *parityColumns = NULL;
    size_t parityColumnsSize = 0;

    calculateParityRows(buffer, size, &parityRows, &parityRowsSize);
    calculateParityColumns(buffer, size, &parityColumns, &parityColumnsSize);

    *paritySizeOut = parityRowsSize + parityColumnsSize;
    *parityOut = (char *)malloc(*paritySizeOut * sizeof(char));

    memcpy(*parityOut, parityRows, parityRowsSize);
    memcpy(*parityOut + parityRowsSize, parityColumns, parityColumnsSize);

    free(parityRows);
    free(parityColumns);
}