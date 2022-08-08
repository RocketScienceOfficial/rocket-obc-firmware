#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Parity data structure.
 */
typedef struct ParityData
{
    /**
     * @brief Parity buffer.
     */
    uint8_t *buffer;

    /**
     * @brief Parity size.
     */
    size_t size;
} ParityData;

/**
 * @brief Encrypt or decrypt data.
 * 
 * @param buffer Buffer to encrypt or decrypt.
 * @param size Size of buffer.
 * @param key Key to encrypt or decrypt.
 * @param keySize Key size.
 */
bool encryptDecrypt(uint8_t *buffer, size_t size, const uint8_t *key, size_t keySize);

/**
 * @brief Calulates parity rows.
 * 
 * @param buffer Buffer to calculate parity rows.
 * @param size Size of buffer.
 * @param data_out_ptr Parity to setup.
 */
bool calculateParityRows(uint8_t *buffer, size_t size, ParityData *data_out_ptr);

/**
 * @brief Calcules parity columns.
 * 
 * @param buffer Buffer to calculate parity columns.
 * @param size Size of buffer.
 * @param data_out_ptr Parity to setup.
 */
bool calculateParityColumns(uint8_t *buffer, size_t size, ParityData *data_out_ptr);

/**
 * @brief Calculates parity rows and columns.
 * 
 * @param buffer Buffer to calculate parity rows and columns.
 * @param size Size of buffer.
 * @param data_out_ptr Parity to setup.
 */
bool calculateParity(uint8_t *buffer, size_t size, ParityData *data_out_ptr);

/**
 * @brief Clears parity data.
 * 
 * @param parity Parity data to clear.
 */
bool clearParity(ParityData *parity);