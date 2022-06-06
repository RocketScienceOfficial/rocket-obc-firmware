#pragma once

#include <stddef.h>

void encryptDecrypt(char *buffer, size_t size, const char* key, size_t keySize);
void calculateParityRows(char *buffer, size_t size, char **parityOut, size_t* paritySizeOut);
void calculateParityColumns(char *buffer, size_t size, char **parityOut, size_t* paritySizeOut);
void calculateParity(char* buffer, size_t size, char **parityOut, size_t *paritySizeOut);