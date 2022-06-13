#pragma once

#include <stddef.h>

typedef struct parity_data
{
    char* buffer;
    size_t size;
} parity_data_t;

void encryptDecrypt(char *buffer, size_t size, const char* key, size_t keySize);
void calculateParityRows(char *buffer, size_t size, parity_data_t* data_out_ptr);
void calculateParityColumns(char *buffer, size_t size, parity_data_t* data_out_ptr);
void calculateParity(char* buffer, size_t size, parity_data_t* data_out_ptr);
void clearParity(parity_data_t* parity);