#pragma once

/**
 * REF: https://www.youtube.com/watch?v=rHoFqKGOPRI
 * REF: https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
*/
#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Creates 32-bit color value from RGB values
 */
#define WS2812_URGB(r, g, b) ((BYTE)(((BYTE)(r) << 16) | ((BYTE)(g) << 8) | (BYTE)(b)))

/**
 * @brief Initializes WS2812 LED strip
 *
 * @param pin Pin number
 * @param rgbw TRUE if RGBW LEDs are used, FALSE if RGB LEDs are used
 * @return Result code
 */
FUNCRESULT ws2812Init(PinNumber pin, BOOL rgbw);

/**
 * @brief Sets color of a single LED
 *
 * @param color Color
 * @return Result code
 */
FUNCRESULT ws2812SetPixel(UINT32 color);