#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct parity_data
{
    uint8_t* buffer;
    size_t size;
} parity_data_t;

void encryptDecrypt(uint8_t *buffer, size_t size, const uint8_t* key, size_t keySize);
void calculateParityRows(uint8_t *buffer, size_t size, parity_data_t* data_out_ptr);
void calculateParityColumns(uint8_t *buffer, size_t size, parity_data_t* data_out_ptr);
void calculateParity(uint8_t* buffer, size_t size, parity_data_t* data_out_ptr);
void clearParity(parity_data_t* parity);