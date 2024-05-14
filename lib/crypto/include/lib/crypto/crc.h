#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Calculate CRC-16/CCITT Checksum. See: https://stackoverflow.com/a/23726131/14697550 & https://www.lammertbies.nl/comm/info/crc-calculation
 *
 * @param data Data to calculate checksum from
 * @param length Length of data
 * @return CRC Checksum
 */
uint16_t crc16_ccitt_calculate(const uint8_t *data, size_t length);

/**
 * @brief Calculate CRC-16/MCRF4XX Checksum. See: https://gist.github.com/aurelj/270bb8af82f65fa645c1
 *
 * @param data Data to calculate checksum from
 * @param length Length of data
 * @return CRC Checksum
 */
uint16_t crc16_mcrf4xx_calculate(const uint8_t *data, size_t length);

#endif