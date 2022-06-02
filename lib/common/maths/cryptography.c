#include "cryptography.h"
#include <string.h>

void encryptDecrypt(char *buffer, char key)
{
    int size = strlen(buffer);

    for (int i = 0; i < size; i++)
    {
        buffer[i] = buffer[i] ^ key;
    }
}

void calculateChecksum(char *buffer, char *checksum)
{
}

static unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;

    while (n)
    {
        count += n & 1;
        
        n >>= 1;
    }

    return count;
}