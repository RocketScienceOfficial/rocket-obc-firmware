#include "radio.h"
#include "config.h"
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

static float s_Rssi;
static uint8_t s_Buffer[256];
static MeasurementData s_CurrentMeasurement;

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

        __LoRaHandlePacket();
    }
}

int LoRaGetRssi()
{
    return s_Rssi;
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

    memcpy(&s_CurrentMeasurement, packet.body.payload, packet.body.payloadSize);

    Serial.print("/*");
    Serial.print(s_CurrentMeasurement.pos_x);
    Serial.print(s_CurrentMeasurement.pos_y);
    Serial.print(s_CurrentMeasurement.pos_z);
    Serial.print(s_CurrentMeasurement.roll);
    Serial.print(s_CurrentMeasurement.pitch);
    Serial.print(s_CurrentMeasurement.yaw);
    Serial.print(s_CurrentMeasurement.latitude);
    Serial.print(s_CurrentMeasurement.longitude);
    Serial.print(s_CurrentMeasurement.altitude);
    Serial.print(s_CurrentMeasurement.velocity);
    Serial.println("*/");
}

void __LoRaBufferEncryptDecrypt(uint8_t *buffer, size_t bufferSize)
{
    for (size_t i = 0; i < bufferSize; i++)
    {
        buffer[i] ^= RADIO_PACKET_KEY[i % sizeof(RADIO_PACKET_KEY)];
    }
}