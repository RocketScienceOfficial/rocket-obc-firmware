#include "drivers/led/w2812_driver.h"
#include "pico/stdlib.h"

#define PIN 15
#define RGBW FALSE
#define PIXELS_SIZE 5

int main()
{
    ws2812Init(PIN, RGBW);

    WS2812COLOR colors[PIXELS_SIZE] = {
        ws2812GetColor(255, 0, 0),
        ws2812GetColor(0, 255, 0),
        ws2812GetColor(0, 0, 255),
        ws2812GetColor(255, 0, 255),
        ws2812GetColor(255, 255, 0),
    };

    ws2812SetColors(colors, PIXELS_SIZE);

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}