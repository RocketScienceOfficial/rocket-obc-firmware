#pragma once

#include "typedefs.h"

/**
 * @brief Parity data structure.
 */
typedef struct ParityData
{
    /**
     * @brief Parity buffer.
     */
    BYTE *buffer;

    /**
     * @brief Parity size.
     */
    SIZE size;
} ParityData;

/**
 * @brief Encrypt or decrypt data.
 *
 * @param buffer Buffer to encrypt or decrypt.
 * @param size Size of buffer.
 * @param key Key to encrypt or decrypt.
 * @param keySize Key size.
 */
BOOL encryptDecrypt(BYTE *buffer, SIZE size, const BYTE *key, SIZE keySize);

/**
 * @brief Calulates parity rows.
 *
 * @param buffer Buffer to calculate parity rows.
 * @param size Size of buffer.
 * @param data_out_ptr Parity to setup.
 */
BOOL calculateParityRows(const BYTE *buffer, SIZE size, ParityData *data_out_ptr);

/**
 * @brief Calcules parity columns.
 *
 * @param buffer Buffer to calculate parity columns.
 * @param size Size of buffer.
 * @param data_out_ptr Parity to setup.
 */
BOOL calculateParityColumns(const BYTE *buffer, SIZE size, ParityData *data_out_ptr);

/**
 * @brief Calculates parity rows and columns.
 *
 * @param buffer Buffer to calculate parity rows and columns.
 * @param size Size of buffer.
 * @param data_out_ptr Parity to setup.
 */
BOOL calculateParity(const BYTE *buffer, SIZE size, ParityData *data_out_ptr);

/**
 * @brief Clears parity data.
 *
 * @param parity Parity data to clear.
 */
BOOL clearParity(ParityData *parity);