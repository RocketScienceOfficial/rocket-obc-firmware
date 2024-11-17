#include "lib/communication/cobs.h"

size_t cobs_encode(const uint8_t *data, size_t length, uint8_t *buffer)
{
    uint8_t *encode = buffer;
    uint8_t *codep = encode++;
    uint8_t code = 1;

    for (const uint8_t *byte = data; length--; ++byte)
    {
        if (*byte)
        {
            *encode++ = *byte, ++code;
        }

        if (!*byte || code == 0xff)
        {
            *codep = code, code = 1, codep = encode;

            if (!*byte || length)
            {
                ++encode;
            }
        }
    }

    *codep = code;

    return (size_t)(encode - buffer);
}

size_t cobs_decode(const uint8_t *buffer, size_t length, uint8_t *data)
{
    const uint8_t *byte = buffer;
    uint8_t *decode = data;

    for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block)
    {
        if (block)
        {
            *decode++ = *byte++;
        }
        else
        {
            if (code != 0xff)
            {
                *decode++ = 0;
            }

            block = code = *byte++;

            if (!code)
            {
                break;
            }
        }
    }

    return (size_t)(decode - (uint8_t *)data);
}