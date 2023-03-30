#include "crypto/xor_encryption.h"

BOOL encryptDecryptXOR(BYTE *buffer, SIZE size, const BYTE *key, SIZE keySize)
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