#include "test_radio_protocol.h"
#include "radio_protocol.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

MY_TEST_FUNC(RADIO_PROTOCOL_TEST_NAME, 1)
{
    uint8_t text[] = "Hello World!";

    radio_body_t body = {
        .command = 'T',
        .payloadSize = sizeof(text) / sizeof(uint8_t),
        .payload = text,
    };

    const uint8_t EXPECTED_SERIALIZED_BUFFER[] = {0x06, 0x00, 0x00, 0x00, 0x11, 0x60, 0x04, 0x01, 0x01, 0x01, 0x54, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x00};
    uint8_t *buffer;
    size_t size;

    serializeRadioPacket(&body, &buffer, &size);

    MY_ASSERT(memcmp(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER) / sizeof(uint8_t)) == 0);

    radio_body_t body2 = {0};
    int validationResult = 0;

    deserializeRadioPacket(buffer, size, &body2, &validationResult);

    MY_ASSERT(validationResult);
    MY_ASSERT(body.command == body2.command);
    MY_ASSERT(body.payloadSize == body2.payloadSize);
    MY_ASSERT(memcmp(body.payload, body2.payload, body.payloadSize) == 0);

    free(body2.payload);
    free(buffer);

    MY_TEST_END();
}