#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Calculate CRC Checksum
 *
 * @param data Data to calculate checksum from
 * @param length Length of data
 * @return CRC Checksum
 */
uint8_t crc_calculate(const uint8_t *data, size_t length);

#endif