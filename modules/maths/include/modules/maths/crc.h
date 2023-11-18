#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>
#include <stddef.h>

uint8_t crc_calculate(const char *sentence, size_t length);

#endif