#include "radio.h"
#include "config.h"
#include "measurements.h"
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

static int s_Rssi;
static int s_RX;
static int s_TX;
static uint8_t s_Buffer[256];

void LoRaInit()
{
    SPI.begin(LORA_SCLK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN);

    delay(2000);

    Serial.println("Starting LoRa...");

    LoRa.setPins(LORA_CS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);

    if (!LoRa.begin(LORA_FREQ))
    {
        Serial.println("Starting LoRa failed!");

        while (1)
            ;
    }

    LoRa.setSignalBandwidth(LORA_BANDWIDTH);

    LoRa.receive();

    Serial.println("Starting LoRa success!");
}

void LoRaCheck()
{
    int packetSize = LoRa.parsePacket();

    if (packetSize)
    {
        Serial.print("Received packet with size: ");
        Serial.print(packetSize);
        Serial.println(" bytes");

        memset(s_Buffer, 0, sizeof(s_Buffer));

        while (LoRa.available())
        {
            uint8_t data = (uint8_t)LoRa.read();

            memcpy(s_Buffer, &data, sizeof(data));
        }

        s_Rssi = LoRa.packetRssi();
        s_RX++;

        __LoRaHandlePacket();
    }
}

int LoRaGetRssi()
{
    return s_Rssi;
}

int LoRaGetRX()
{
    return s_RX;
}

int LoRaGetTX()
{
    return s_TX;
}

void __LoRaHandlePacket()
{
    __LoRaBufferEncryptDecrypt(s_Buffer, sizeof(s_Buffer));

    RadioPacket packet = {0};

    memcpy(&packet, s_Buffer, sizeof(RadioPacket));

    if (memcmp(packet.header.signature, RADIO_PACKET_SIGNATURE, sizeof(RADIO_PACKET_SIGNATURE)) != 0)
    {
        Serial.println("Invalid packet signature!");

        return;
    }

    if (packet.body.payloadSize != sizeof(MeasurementData))
    {
        Serial.println("Invalid payload size!");

        return;
    }

    if (packet.body.command != RADIO_COMMAND_MEASUREMENT)
    {
        Serial.println("Invalid command!");

        return;
    }

    MeasurementData measurement;
    memcpy(&measurement, packet.body.payload, packet.body.payloadSize);

    SetMeasurementData(&measurement);
}

void __LoRaBufferEncryptDecrypt(uint8_t *buffer, size_t bufferSize)
{
    for (size_t i = 0; i < bufferSize; i++)
    {
        buffer[i] ^= RADIO_PACKET_KEY[i % sizeof(RADIO_PACKET_KEY)];
    }
}