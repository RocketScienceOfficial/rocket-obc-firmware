#include "drivers/lora/radio_protocol.h"
#include "crypto/parity_check.h"
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

BOOL serializeRadioPacket(RadioBody *body, BYTE **buffer_out_ptr, SIZE *size_out)
{
    SIZE bodySize = sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE) + sizeof(BYTE) * body->payloadSize;
    BYTE bodyBuffer[bodySize];

    memcpy(bodyBuffer, &body->command, sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE));
    memcpy(bodyBuffer + sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE), body->payload, sizeof(BYTE) * body->payloadSize);

    ParityData parity = {0};

    if (!calculateParity(bodyBuffer, bodySize, &parity))
    {
        return FALSE;
    }

    RadioHeader header = {
        .paritySize = parity.size,
        .parity = parity.buffer,
    };

    SIZE headerSize = sizeof(SIZE) + sizeof(BYTE) * header.paritySize + sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH;
    BYTE headerBuffer[headerSize];

    memcpy(headerBuffer, &header.paritySize, sizeof(SIZE));
    memcpy(headerBuffer + sizeof(SIZE), header.parity, sizeof(BYTE) * header.paritySize);
    memcpy(headerBuffer + sizeof(SIZE) + sizeof(BYTE) * header.paritySize, RADIO_PACKET_SIGNATURE, sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH);

    if (!clearParity(&parity))
    {
        return FALSE;
    }

    SIZE bufferSize = headerSize + bodySize;

    *buffer_out_ptr = (BYTE *)malloc(bufferSize);
    *size_out = bufferSize;

    memcpy(*buffer_out_ptr, headerBuffer, headerSize);
    memcpy(*buffer_out_ptr + headerSize, bodyBuffer, bodySize);

    if (!encryptDecryptXOR(*buffer_out_ptr, bufferSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(BYTE)))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL deserializeRadioPacket(BYTE *buffer, SIZE size, RadioBody *body_out)
{
    RadioPacket packet = {0};

    if (!encryptDecryptXOR(buffer, size, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(BYTE)))
    {
        return FALSE;
    }

    SIZE currentBufferOffset = 0;

    if (currentBufferOffset + sizeof(SIZE) > size)
    {
        return FALSE;
    }

    memcpy(&packet.header.paritySize, buffer + currentBufferOffset, sizeof(SIZE));

    currentBufferOffset += sizeof(SIZE);

    if (currentBufferOffset + sizeof(BYTE) * packet.header.paritySize > size)
    {
        return FALSE;
    }

    packet.header.parity = (BYTE *)malloc(sizeof(BYTE) * packet.header.paritySize);

    memcpy(packet.header.parity, buffer + currentBufferOffset, sizeof(BYTE) * packet.header.paritySize);

    currentBufferOffset += sizeof(BYTE) * packet.header.paritySize;

    if (currentBufferOffset + sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH > size)
    {
        free(packet.header.parity);

        return FALSE;
    }

    memcpy(packet.header.signature, buffer + currentBufferOffset, sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH);

    BOOL compSig = memcmp(packet.header.signature, RADIO_PACKET_SIGNATURE, sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH) == 0;

    if (!compSig)
    {
        free(packet.header.parity);

        return FALSE;
    }

    SIZE bodyOffset = currentBufferOffset + sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH;
    SIZE parityBufferSize = size - bodyOffset;
    ParityData parity = {0};

    if (!calculateParity(buffer + bodyOffset, parityBufferSize, &parity))
    {
        return FALSE;
    }

    BOOL compParity = memcmp(packet.header.parity, parity.buffer, sizeof(BYTE) * packet.header.paritySize) == 0;

    free(packet.header.parity);

    if (!clearParity(&parity))
    {
        return FALSE;
    }

    if (!compParity)
    {
        return FALSE;
    }

    currentBufferOffset += sizeof(BYTE) * RADIO_PACKET_SIGNATURE_LENGTH;

    if (currentBufferOffset + sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE) > size)
    {
        return FALSE;
    }

    memcpy(&packet.body, buffer + currentBufferOffset, sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE));

    currentBufferOffset += sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE);

    if (currentBufferOffset + sizeof(BYTE) * packet.body.payloadSize > size)
    {
        return FALSE;
    }

    packet.body.payload = (BYTE *)malloc(sizeof(BYTE) * packet.body.payloadSize);

    memcpy(packet.body.payload, buffer + currentBufferOffset, sizeof(BYTE) * packet.body.payloadSize);

    *body_out = packet.body;

    return TRUE;
}

VOID radioClearPacket(RadioBody *body)
{
    if (!body)
    {
        return;
    }

    if (body->payload)
    {
        free(body->payload);
    }
}