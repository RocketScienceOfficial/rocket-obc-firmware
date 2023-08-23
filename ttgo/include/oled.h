#pragma once

#include <SSD1306Wire.h>
#include "config.h"

class MyOLED
{
public:
    MyOLED() : m_Display(0x3c, OLED_SDA_PIN, OLED_SCL_PIN) {}

    void Init();
    void UpdateScreen(int rssi, float lat, float lon, float alt);

private:
    char m_Buffer[128];
    unsigned long m_LastUpdate;
    SSD1306Wire m_Display;

    void DrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress);
};