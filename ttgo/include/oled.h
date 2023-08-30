#pragma once

#include <Arduino.h>

void OLEDInit();
void OLEDUpdateScreen(int rssi, float lat, float lon, float alt, float targetLat, float targetLon, float targetAlt);
void __OLEDDrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);