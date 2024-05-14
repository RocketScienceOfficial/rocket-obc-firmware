#include "lib/crypto/crc.h"

uint16_t crc16_ccitt_calculate(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;
    uint8_t x;

    while (length--)
    {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }

    return crc;
}

uint16_t crc16_mcrf4xx_calculate(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;
    uint8_t t;
    uint8_t L;

    while (length--)
    {
        crc ^= *data++;
        L = crc ^ (crc << 4);
        t = (L << 3) | (L >> 5);
        L ^= (t & 0x07);
        t = (t & 0xF8) ^ (((t << 1) | (t >> 7)) & 0x0F) ^ (uint8_t)(crc >> 8);
        crc = (L << 8) | t;
    }

    return crc;
}