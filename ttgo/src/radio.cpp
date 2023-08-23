#include "radio.h"
#include "config.h"
#include <Arduino.h>
#include <SPI.h>

void MyLoRa::Init()
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

void MyLoRa::Check()
{
    int packetSize = LoRa.parsePacket();

    if (packetSize)
    {
        Serial.print("Received packet with size: ");
        Serial.print(packetSize);
        Serial.println(" bytes");

        memset(m_Buffer, 0, sizeof(m_Buffer));

        while (LoRa.available())
        {
            uint8_t data = (uint8_t)LoRa.read();

            memcpy(m_Buffer, &data, sizeof(data));
        }

        m_Rssi = LoRa.packetRssi();

        HandlePacket();
    }
}

int MyLoRa::GetRssi()
{
    return m_Rssi;
}

void MyLoRa::HandlePacket()
{
    BufferEncryptDecrypt(m_Buffer, sizeof(m_Buffer));

    RadioPacket packet = {0};

    memcpy(&packet, m_Buffer, sizeof(RadioPacket));

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

    memcpy(&m_CurrentMeasurement, packet.body.payload, packet.body.payloadSize);

    Serial.print("/*");
    Serial.print(m_CurrentMeasurement.pos_x);
    Serial.print(m_CurrentMeasurement.pos_y);
    Serial.print(m_CurrentMeasurement.pos_z);
    Serial.print(m_CurrentMeasurement.roll);
    Serial.print(m_CurrentMeasurement.pitch);
    Serial.print(m_CurrentMeasurement.yaw);
    Serial.print(m_CurrentMeasurement.latitude);
    Serial.print(m_CurrentMeasurement.longitude);
    Serial.print(m_CurrentMeasurement.altitude);
    Serial.print(m_CurrentMeasurement.velocity);
    Serial.println("*/");
}

void MyLoRa::BufferEncryptDecrypt(uint8_t *buffer, size_t bufferSize)
{
    for (size_t i = 0; i < bufferSize; i++)
    {
        buffer[i] ^= RADIO_PACKET_KEY[i % sizeof(RADIO_PACKET_KEY)];
    }
}