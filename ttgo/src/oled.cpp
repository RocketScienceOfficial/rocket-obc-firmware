#include "oled.h"
#include "config.h"
#include "state.h"
#include <Wire.h>
#include <SSD1306Wire.h>

static char s_Buffer[128];
static SSD1306Wire s_Display(0x3C, OLED_SDA_PIN, OLED_SCL_PIN);

void OLEDInit()
{
    s_Display.init();
    s_Display.flipScreenVertically();
    s_Display.setTextAlignment(TEXT_ALIGN_LEFT);
    s_Display.setFont(ArialMT_Plain_10);
    s_Display.clear();

    Serial.println("Initialized OLED!");
}

void OLEDUpdateScreen(int rssi, float lat, float lon, float alt, float targetLat, float targetLon, float targetAlt)
{
    s_Display.clear();

    switch (StateGetCurrent())
    {
    case SystemState::DASHBOARD:
        __OLEDDrawProgressBar(0, 0, 72, 9, (uint8_t)(100.0f * (rssi + 164.0f) / 164.0f));

        s_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(127, 0, s_Buffer, "%d", rssi);

        s_Display.setTextAlignment(TEXT_ALIGN_LEFT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(0, 13, s_Buffer, "%.3f m", alt);
        s_Display.drawStringf(0, 43, s_Buffer, "%c%.7f", ' ', lat);
        s_Display.drawStringf(0, 52, s_Buffer, "%c%.7f", ' ', lon);
        break;
    case SystemState::LOCATION:
        s_Display.setTextAlignment(TEXT_ALIGN_LEFT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(0, 13, s_Buffer, "%.3f m", alt);
        s_Display.drawStringf(0, 43, s_Buffer, "%c%.7f", ' ', lat);
        s_Display.drawStringf(0, 52, s_Buffer, "%c%.7f", ' ', lon);

        s_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(0, 13, s_Buffer, "%.3f m", alt);
        s_Display.drawStringf(0, 43, s_Buffer, "%c%.7f", ' ', lat);
        s_Display.drawStringf(0, 52, s_Buffer, "%c%.7f", ' ', lon);
        break;
    case SystemState::NAVIGATION:
        break;
    case SystemState::RADIO:
        __OLEDDrawProgressBar(0, 0, 72, 9, (uint8_t)(100.0f * (rssi + 164.0f) / 164.0f));

        s_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(127, 0, s_Buffer, "%d", rssi);
        break;
    default:
        break;
    }

    s_Display.display();

    Serial.println("Updated OLED!");
}

void __OLEDDrawProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t progress)
{
    uint16_t radius = height / 2;
    uint16_t xRadius = x + radius;
    uint16_t yRadius = y + radius;
    uint16_t doubleRadius = 2 * radius;
    uint16_t innerRadius = radius - 2;
    uint16_t maxProgressWidth = (width - doubleRadius + 1) * progress / 100;

    s_Display.setColor(WHITE);
    s_Display.fillRect(x, y, maxProgressWidth, height);
    s_Display.drawHorizontalLine(x, y + radius, width);

    s_Display.setColor(BLACK);

    for (uint8_t i = 0; (i * 8) < width; i++)
    {
        s_Display.drawLine(x + 8 * i, y, x + 8 * i + 4, y + radius);
        s_Display.drawLine(x + 8 * i, y + height - 1, x + 8 * i + 4, y + radius);
        s_Display.drawLine(x + 8 * i + 1, y, x + 8 * i + 4 + 1, y + radius);
        s_Display.drawLine(x + 8 * i + 1, y + height - 1, x + 8 * i + 4 + 1, y + radius);
    }

    s_Display.setColor(WHITE);
}