#include <Arduino.h>

#include "radio.h"
#include "gps.h"
#include "oled.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialized Board!");

  GPSInit();
  LoRaInit();
  OLEDInit();
}

void loop()
{
  GPSCheck();
  LoRaCheck();
  OLEDUpdateScreen(LoRaGetRssi(), GPSGetLatitude(), GPSGetLongitude(), GPSGetAltitude());
}