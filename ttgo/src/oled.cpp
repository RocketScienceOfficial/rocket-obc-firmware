#include "oled.h"
#include "config.h"
#include "state.h"
#include "maths.h"
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

void OLEDUpdateScreen(OLEDInputData &data)
{
    s_Display.clear();

    switch (StateGetCurrent())
    {
    case SystemState::DASHBOARD:
        __OLEDDrawProgressBar(0, 0, 72, 9, (uint8_t)(100.0f * (data.rssi + 164.0f) / 164.0f));

        s_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(88, 0, s_Buffer, "%d", data.rssi);
        s_Display.drawStringf(127, 0, s_Buffer, "%d %%", data.batteryPercentage);
        s_Display.drawStringf(127, 13, s_Buffer, "%.2f V", data.batteryVoltage);

        s_Display.setTextAlignment(TEXT_ALIGN_LEFT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(0, 13, s_Buffer, "RX: %d", data.rx);
        s_Display.drawStringf(40, 13, s_Buffer, "TX: %d", data.tx);
        s_Display.drawStringf(0, 30, s_Buffer, "%c%.3f m", ' ', data.alt);
        s_Display.drawStringf(0, 43, s_Buffer, "%c%.7f", ' ', data.lat);
        s_Display.drawStringf(0, 52, s_Buffer, "%c%.7f", ' ', data.lon);

        __OLEDDrawBatteryIndicator(110, 36, 30, 15, data.batteryPercentage);
        break;
    case SystemState::NAVIGATION:
        s_Display.setTextAlignment(TEXT_ALIGN_LEFT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(0, 0, s_Buffer, "%c%.3f m", ' ', data.targetAlt);
        s_Display.drawStringf(0, 13, s_Buffer, "%c%.7f", ' ', data.targetLat);
        s_Display.drawStringf(0, 22, s_Buffer, "%c%.7f", ' ', data.targetLon);

        s_Display.setTextAlignment(TEXT_ALIGN_RIGHT);
        s_Display.setFont(ArialMT_Plain_10);
        s_Display.drawStringf(75, 52, s_Buffer, "%d m", (int)CalculateGeoDistance(data.lat, data.lon, data.targetLat, data.targetLon));

        __OLEDDrawCompass(103, 39, 0.0f, 0.0f, 0.0f);
        break;
    default:
        break;
    }

    s_Display.display();
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

void __OLEDDrawCompass(uint16_t x, uint16_t y, float angle, float pitch, float roll)
{
    s_Display.drawCircle(x, y, 23);
    s_Display.drawCircle(x, y, 10);
    s_Display.fillCircle(x + pitch * (50.0f), y + (roll * 50.0f), 8);

    uint8_t xp = x;
    uint8_t yp = y;

    uint8_t x0 = xp + cos(angle) * 24;
    uint8_t y0 = yp - sin(angle) * 24;
    uint8_t x1 = xp + cos(angle + 0.2) * 15;
    uint8_t y1 = yp - sin(angle + 0.2) * 15;
    uint8_t x2 = xp + cos(angle - 0.2) * 15;
    uint8_t y2 = yp - sin(angle - 0.2) * 15;

    s_Display.drawLine(2 * xp - x0, 2 * yp - y0, x0, y0);
    s_Display.drawLine(2 * xp - x0 + 1, 2 * yp - y0, x0 + 1, y0);

    s_Display.drawLine(x0, y0, x1, y1);
    s_Display.drawLine(x1, y1, x2, y2);
    s_Display.drawLine(x2, y2, x0, y0);
}

void __OLEDDrawBatteryIndicator(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int percentage)
{
    const uint8_t MAX_STEPS_COUNT = 4;
    const uint16_t TOTAL_STEP_WIDTH = width / MAX_STEPS_COUNT;
    const uint16_t STEP_WIDTH = TOTAL_STEP_WIDTH * 0.75;
    const uint16_t STEP_HEIGHT = height * 0.75;
    
    s_Display.drawRect(x - width / 2, y - height / 2, width, height);
    s_Display.fillRect(x - width / 2 - width / 10, y - height / 4, width / 10, height / 2);

    uint8_t stepsCount = percentage / (100 / MAX_STEPS_COUNT);

    for (uint8_t i = 0; i < stepsCount; i++)
    {
        s_Display.fillRect(x + TOTAL_STEP_WIDTH * (1 - i) + (TOTAL_STEP_WIDTH - STEP_WIDTH) / 2, y - STEP_HEIGHT / 2, STEP_WIDTH, STEP_HEIGHT);
    }
}