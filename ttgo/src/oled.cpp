#include "oled.h"

void MyOLED::Init()
{
    m_Display.init();
    m_Display.setContrast(255);
    m_Display.setTextAlignment(TEXT_ALIGN_LEFT);
    m_Display.setFont(ArialMT_Plain_10);
    m_Display.display();

    Serial.println("Initialized OLED!");
}

void MyOLED::UpdateScreen(int rssi, float lat, float lon, float alt)
{
    unsigned long now = millis();

    if (now - m_LastUpdate >= OLED_UPDATE_INTERVAL)
    {
        m_LastUpdate = now;

        m_Display.clear();

        DrawProgressBar(0, 0, 72, 9, (uint8_t)(100.0f * (rssi + 164.0f) / 164.0f));
        m_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        m_Display.setFont(ArialMT_Plain_10);
        m_Display.drawStringf(127, 0, m_Buffer, "%d", rssi);

        m_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        m_Display.setFont(ArialMT_Plain_10);
        m_Display.drawStringf(0, 13, m_Buffer, "%.3f m", alt);
        m_Display.drawStringf(0, 43, m_Buffer, "%c%.7f", lat);
        m_Display.drawStringf(0, 52, m_Buffer, "%c%.7f", lon);
    }
}

void MyOLED::DrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress)
{
    uint16_t radius = height / 2;
    uint16_t xRadius = x + radius;
    uint16_t yRadius = y + radius;
    uint16_t doubleRadius = 2 * radius;
    uint16_t innerRadius = radius - 2;
    uint16_t maxProgressWidth = (width - doubleRadius + 1) * progress / 100;

    m_Display.setColor(WHITE);
    m_Display.fillRect(x, y, maxProgressWidth, height);
    m_Display.drawHorizontalLine(x, y + radius, width);

    m_Display.setColor(BLACK);

    for (uint8_t i = 0; (i * 8) < width; i++)
    {
        // drawVerticalLine(x + 8*i, y, height);
        m_Display.drawLine(x + 8 * i, y, x + 8 * i + 4, y + radius);
        m_Display.drawLine(x + 8 * i, y + height - 1, x + 8 * i + 4, y + radius);
        m_Display.drawLine(x + 8 * i + 1, y, x + 8 * i + 4 + 1, y + radius);
        m_Display.drawLine(x + 8 * i + 1, y + height - 1, x + 8 * i + 4 + 1, y + radius);
    }

    m_Display.setColor(WHITE);
}