#include <Arduino.h>

#include "radio.h"
#include "gps.h"
#include "oled.h"

static MyGPS s_GPS;
static MyLoRa s_LoRa;
static MyOLED s_OLED;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialized Board!");

  s_GPS.Init();
  s_LoRa.Init();
  s_OLED.Init();
}

void loop()
{
  s_GPS.Check();
  s_LoRa.Check();
  s_OLED.UpdateScreen(s_LoRa.GetRssi(), s_GPS.GetLatitude(), s_GPS.GetLongitude(), s_GPS.GetAltitude());
}