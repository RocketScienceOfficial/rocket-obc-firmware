#ifndef _COBS_H
#define _COBS_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief COBS encode data to buffer (Does not output delimiter byte). REF: https://rosettacode.org/wiki/Consistent_overhead_byte_stuffing
 * @param data Pointer to input data to encode
 * @param length Number of bytes to encode
 * @param buffer Pointer to encoded output buffer
 * @return Encoded buffer length in bytes
 */
size_t cobs_encode(const uint8_t *data, size_t length, uint8_t *buffer);

/**
 * @brief COBS decode data from buffer (Stops decoding if delimiter byte is found). REF: https://rosettacode.org/wiki/Consistent_overhead_byte_stuffing
 * @param buffer Pointer to encoded input bytes
 * @param length Number of bytes to decode
 * @param data Pointer to decoded output data
 * @return Number of bytes successfully decoded
 */
size_t cobs_decode(const uint8_t *buffer, size_t length, uint8_t *data);

#endif