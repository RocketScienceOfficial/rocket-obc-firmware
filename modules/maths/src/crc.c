#include "modules/maths/crc.h"

uint8_t crc_calculate(const char *sentence, size_t length)
{
    uint8_t checksum = 0;

    for (size_t i = 0; i < length; i++)
    {
        checksum ^= sentence[i];
    }

    return checksum;
}