#include <Arduino.h>

#include "config.h"
#include "radio.h"
#include "gps.h"
#include "oled.h"
#include "state.h"
#include "measurements.h"
#include "power.h"

static unsigned long s_LastUpdate;
static unsigned long s_Now;

static void UpdateScreen();

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialized Board!");

  GPSInit();
  LoRaInit();
  OLEDInit();
  StateInit();
  PMUInit();

  delay(1000);
}

void loop()
{
  GPSCheck();
  LoRaCheck();
  StateCheck();
  PMURead();

  s_Now = millis();

  if (s_Now - s_LastUpdate >= OLED_UPDATE_INTERVAL || StateChanged())
  {
    s_LastUpdate = s_Now;

    UpdateScreen();
  }
}

static void UpdateScreen()
{
  OLEDInputData data = {
      .rssi = LoRaGetRssi(),
      .rx = LoRaGetRX(),
      .tx = LoRaGetTX(),
      .batteryVoltage = PMUGetCurrentData().batteryVoltage,
      .batteryPercentage = PMUGetCurrentData().batteryPercentage,
      .lat = GPSGetLatitude(),
      .lon = GPSGetLongitude(),
      .alt = GPSGetAltitude(),
      .targetLat = GetCurrentMeasurement().latitude,
      .targetLon = GetCurrentMeasurement().longitude,
      .targetAlt = GetCurrentMeasurement().altitude,
  };

  OLEDUpdateScreen(data);
}

// #include <SPI.h>
// #include <LoRa.h>
// #include <Wire.h>

// #define SCK 5   // GPIO5  -- SX1278's SCK
// #define MISO 19 // GPIO19 -- SX1278's MISnO
// #define MOSI 27 // GPIO27 -- SX1278's MOSI
// #define SS 18   // GPIO18 -- SX1278's CS
// #define RST 23  // GPIO14 -- SX1278's RESET
// #define DI0 26  // GPIO26 -- SX1278's IRQ(Interrupt Request)
// #define BAND 433E6

// unsigned int counter = 0;

// String rssi = "RSSI --";
// String packSize = "--";
// String packet;

// void setup()
// {
//   Serial.begin(115200);
//   while (!Serial)
//     ;
//   Serial.println();
//   Serial.println("LoRa Sender Test");

//   SPI.begin(SCK, MISO, MOSI, SS);
//   LoRa.setPins(SS, RST, DI0);
//   if (!LoRa.begin(BAND))
//   {
//     Serial.println("Starting LoRa failed!");
//     while (1)
//       ;
//   }
//   // LoRa.onReceive(cbk);
//   //  LoRa.receive();
//   Serial.println("init ok");

//   delay(1500);
// }

// void loop()
// {
//   Serial.println(String(counter));

//   // send packet
//   LoRa.beginPacket();
//   LoRa.print("hello ");
//   LoRa.print(counter);
//   LoRa.endPacket();

//   counter++;
//   delay(1000); // wait for a second
// }