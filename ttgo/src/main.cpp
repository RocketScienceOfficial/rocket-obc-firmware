
#include <LoRa.h>
#include <Arduino.h>

#include "utils.h"

static uint8_t s_LoRaBuffer[256];
static MeasurementData s_CurrentMeasurement;

static void __handlePacket();

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialized Board!");
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

  delay(2000);

  Serial.println("Starting LoRa...");

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);

  if (!LoRa.begin(LORA_FREQ))
  {
    Serial.println("Starting LoRa failed!");

    while (1)
    {
      continue;
    }
  }

  Serial.println("Starting LoRa success!");
}

void loop()
{
  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    Serial.print("Received packet with size: ");
    Serial.print(packetSize);
    Serial.println(" bytes");

    memset(s_LoRaBuffer, 0, sizeof(s_LoRaBuffer));

    while (LoRa.available())
    {
      uint8_t data = (uint8_t)LoRa.read();

      memcpy(s_LoRaBuffer, &data, sizeof(data));
    }

    __handlePacket();
  }
}

static void __handlePacket()
{
  bufferEncryptDecrypt(s_LoRaBuffer, sizeof(s_LoRaBuffer));

  RadioPacket packet = {0};

  memcpy(&packet, s_LoRaBuffer, sizeof(RadioPacket));

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