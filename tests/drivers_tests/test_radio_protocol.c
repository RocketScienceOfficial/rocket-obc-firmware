#include "test_radio_protocol.h"
#include "drivers/lora/radio_protocol.h"
#include <string.h>
#include <stdlib.h>

static const BYTE EXPECTED_SERIALIZED_BUFFER[] = {
    0x25,
    0x90,
    0x08,
    0x98,
    0xf5,
    0x67,
    0xae,
    0x66,
    0x34,
    0x95,
    0xc3,
    0x62,
    0xd0,
    0x29,
    0xcd,
    0x05,
    0x0b,
    0x4e,
    0xc1,
    0x2f,
    0xa5,
    0xbe,
    0xdf,
    0xbd,
    0xed,
    0x65,
    0xca,
    0x1b,
    0x43,
    0x67,
    0xb2,
    0xa3,
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
    0xe0,
    0xdb,
    0xb3,
    0xd1,
};

MY_TEST_FUNC(RADIO_PROTOCOL_TEST_NAME, 1)
{
    BYTE text[] = "Hello World!";

    RadioBody body = {0};
    body.command = 'T';
    body.payloadSize = sizeof(text);

    memcpy(body.payload, text, sizeof(text));

    BYTE buffer[sizeof(RadioPacket)];
    BOOL serializationResult = serializeRadioPacket(&body, buffer);

    MY_ASSERT(serializationResult == TRUE);
    MY_ASSERT(memcmp(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER)) == 0);

    RadioBody body2 = {0};
    BOOL deserializationResult = deserializeRadioPacket(buffer, &body2);

    MY_ASSERT(deserializationResult == TRUE);
    MY_ASSERT(body.command == body2.command);
    MY_ASSERT(body.payloadSize == body2.payloadSize);
    MY_ASSERT(memcmp(body.payload, body2.payload, body.payloadSize) == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(RADIO_PROTOCOL_TEST_NAME, 2)
{
    BYTE buffer[sizeof(RadioPacket)];

    memcpy(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER));

    buffer[0] = 0x75;
    buffer[sizeof(RadioPacket) - 2] = 0xE5;
    buffer[sizeof(RadioPacket) - 1] = 0x9B;

    RadioBody body = {0};
    BOOL deserializationResult = deserializeRadioPacket(buffer, &body);

    MY_ASSERT(deserializationResult == FALSE);

    MY_TEST_END();
}