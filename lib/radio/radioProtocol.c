#include "radioProtocol.h"
#include "cryptography.h"
#include <string.h>
#include "logger.h"

void radioSendPacket(lora_data_t *lora, radio_packet_t *packet)
{
    encryptDecrypt(packet->buffer, PACKET_KEY);

    loraBeginPacket(lora, 0);
    loraWrite_str_s(lora, packet->buffer, sizeof(radio_packet_t) - (MAX_PAYLOAD_SIZE - packet->payloadSize));
    loraEndPacket(lora, 0);
}

int radioReceivePacket(lora_data_t *lora, radio_packet_t *packet)
{
    int packetSize = loraParsePacket(lora, 0);

    if (packetSize)
    {
        char buffer[packetSize];
        int i = 0;

        while (loraAvailable(lora))
        {
            buffer[i] = (char)loraRead(lora);
            i++;
        }

        encryptDecrypt(buffer, PACKET_KEY);

        memcpy(packet->buffer, buffer, packetSize);

        return 1;
    }

    return 0;
}