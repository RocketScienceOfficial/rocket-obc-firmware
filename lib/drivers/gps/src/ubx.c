#include "lib/drivers/gps/ubx.h"
#include <string.h>

#define UBX_PREAMBLE_SYNC_CHAR_1 0xb5
#define UBX_PREAMBLE_SYNC_CHAR_2 0x62

size_t ubx_create_frame(uint8_t *frameBuffer, uint8_t class, uint8_t id, const uint8_t *payload, uint16_t length)
{
    frameBuffer[0] = UBX_PREAMBLE_SYNC_CHAR_1;
    frameBuffer[1] = UBX_PREAMBLE_SYNC_CHAR_2;
    frameBuffer[2] = class;
    frameBuffer[3] = id;
    frameBuffer[4] = length & 0xff;
    frameBuffer[5] = (length >> 8) & 0xff;

    memcpy(frameBuffer + 6, payload, length);

    uint8_t cka, ckb;
    ubx_checksum(&cka, &ckb, frameBuffer + 2, 4 + length);

    frameBuffer[6 + length + 0] = cka;
    frameBuffer[6 + length + 1] = ckb;

    return 6 + length + 2;
}

void ubx_checksum(uint8_t *cka, uint8_t *ckb, const uint8_t *buffer, uint8_t len)
{
    *cka = 0;
    *ckb = 0;

    for (uint8_t i = 0; i < len; i++)
    {
        *cka += buffer[i];
        *ckb += *cka;
    }

    *cka &= 0xff;
    *ckb &= 0xff;
}