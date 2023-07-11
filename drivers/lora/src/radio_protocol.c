#include "drivers/lora/radio_protocol.h"
#include "crypto/xor_encryption.h"
#include <string.h>
#include <stdlib.h>

static const BYTE RADIO_PACKET_KEY[] = {
    0x18,
    0x07,
    0x45,
    0x95,
    0xc0,
    0x24,
    0xb0,
    0x49,
    0x06,
    0xfe,
    0x95,
    0x4c,
    0xe7,
    0x10,
    0xfd,
    0x3c,
    0x5f,
    0x4e,
    0xc1,
    0x2f,
    0xed,
    0xdb,
    0xb3,
    0xd1,
    0x82,
    0x45,
    0x9d,
    0x74,
    0x31,
    0x0b,
    0xd6,
    0x82,
    0x3d,
    0xb1,
    0x97,
    0xdd,
    0x01,
    0x30,
    0x7e,
    0x87,
    0xee,
    0x9e,
    0x15,
    0xdf,
    0x1f,
    0x6d,
    0x8c,
    0x16,
    0x99,
    0x37,
    0xb0,
    0xc0,
    0x9f,
    0x56,
    0xf8,
    0x5d,
    0xbd,
    0x5e,
    0xd4,
    0xae,
    0x4d,
    0x23,
    0x6c,
    0xa0,
};

static const BYTE RADIO_PACKET_SIGNATURE[RADIO_PACKET_SIGNATURE_LENGTH] = {
    0x3d,
    0x97,
    0x4d,
    0x0d,
    0x35,
    0x43,
    0x1e,
    0x2f,
    0x32,
    0x6b,
    0x56,
    0x2e,
    0x37,
    0x39,
    0x30,
    0x39,
};

BOOL serializeRadioPacket(RadioBody *body, BYTE *pBuffer)
{
    RadioHeader header = {0};
    memcpy(header.signature, RADIO_PACKET_SIGNATURE, sizeof(RADIO_PACKET_SIGNATURE));

    RadioPacket packet = {
        .header = header,
        .body = *body,
    };

    memcpy(pBuffer, &packet, sizeof(packet));

    if (!encryptDecryptXOR(pBuffer, sizeof(RadioPacket), RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY)))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL deserializeRadioPacket(BYTE *buffer, RadioBody *pBody)
{
    if (!encryptDecryptXOR(buffer, sizeof(RadioPacket), RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY)))
    {
        return FALSE;
    }

    RadioHeader header = {0};
    BYTE *bodyBuffer = buffer + sizeof(RadioHeader);

    memcpy(&header, buffer, sizeof(RadioHeader));
    memcpy(pBody, bodyBuffer, sizeof(RadioBody));

    if (memcmp(header.signature, RADIO_PACKET_SIGNATURE, sizeof(RADIO_PACKET_SIGNATURE)) != 0)
    {
        return FALSE;
    }

    return TRUE;
}