#pragma once

#include "tools/typedefs.h"

/**
 * @brief Encrypt or decrypt data.
 *
 * @param buffer Buffer to encrypt or decrypt.
 * @param size Size of buffer.
 * @param key Key to encrypt or decrypt.
 * @param keySize Key size.
 */
BOOL encryptDecryptXOR(BYTE *buffer, SIZE size, const BYTE *key, SIZE keySize);