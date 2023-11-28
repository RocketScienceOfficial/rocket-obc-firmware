#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Calculate CRC8 Checksum
 *
 * @param data Data to calculate checksum from
 * @param length Length of data
 * @return CRC Checksum
 */
uint8_t crc8_calculate(const uint8_t *data, size_t length);

/**
 * @brief Calculate CRC16 Checksum
 *
 * @param data Data to calculate checksum from
 * @param length Length of data
 * @return CRC Checksum
 */
uint16_t crc16_calculate(const uint8_t *data, size_t length);

#endif