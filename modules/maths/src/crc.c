#include "modules/maths/crc.h"

uint8_t crc8_calculate(const uint8_t *data, size_t length)
{
    uint8_t checksum = 0;

    for (size_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }

    return checksum;
}

uint16_t crc16_calculate(const uint8_t *data, size_t length)
{
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while (length--)
    {
        x = crc >> 8 ^ *data++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }

    return crc;
}