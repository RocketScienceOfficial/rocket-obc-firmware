#include <Arduino.h>

#include "config.h"
#include "radio.h"
#include "gps.h"
#include "oled.h"
#include "state.h"
#include "measurements.h"

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
}

void loop()
{
  GPSCheck();
  LoRaCheck();
  StateCheck();

  s_Now = millis();

  if (s_Now - s_LastUpdate >= OLED_UPDATE_INTERVAL || StateChanged())
  {
    s_LastUpdate = s_Now;

    UpdateScreen();
  }
}

static void UpdateScreen()
{
  OLEDUpdateScreen(LoRaGetRssi(),
                   GPSGetLatitude(),
                   GPSGetLongitude(),
                   GPSGetAltitude(),
                   GetCurrentMeasurement().latitude,
                   GetCurrentMeasurement().longitude,
                   GetCurrentMeasurement().altitude);
}