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