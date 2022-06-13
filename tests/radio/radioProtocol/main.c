#include "pico/stdlib.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "radio_protocol.h"

void start();
void initialize();
void loop();

int main()
{
    start();
    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

#include <stdio.h>

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());

    MY_LOG_CORE_INFO("Initializing...");

    char text[] = "Hello World!";

    radio_body_t body = {
        .command = 'T',
        .payloadSize = sizeof(text) / sizeof(char),
        .payload = text,
    };

    const char EXPECTED_SERIALIZED_BUFFER[] = {0x06, 0x00, 0x00, 0x00, 0x11, 0x60, 0x04, 0x01, 0x01, 0x01, 0x54, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x00};
    char *buffer;
    size_t size;

    serializeRadioPacket(&body, &buffer, &size);

    MY_ASSERT(memcmp(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER) / sizeof(char)) == 0);

    radio_body_t body2 = {0};
    int validationResult = 0;

    deserializeRadioPacket(buffer, size, &body2, &validationResult);

    MY_ASSERT(validationResult);
    MY_ASSERT(body.command == body2.command);
    MY_ASSERT(body.payloadSize == body2.payloadSize);
    MY_ASSERT(memcmp(body.payload, body2.payload, body.payloadSize) == 0);

    free(body2.payload);
    free(buffer);

    MY_LOG_CORE_INFO("Done!");
}

static radio_body_t s_TestBody;

void loop()
{
    tight_loop_contents();
}