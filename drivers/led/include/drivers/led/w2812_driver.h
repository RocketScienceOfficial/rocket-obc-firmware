#pragma once

/**
 * REF: https://www.youtube.com/watch?v=rHoFqKGOPRI
 * REF: https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */
#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

typedef UINT32 WS2812COLOR; /** Color typedef. */

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
FUNCRESULT ws2812SetPixel(WS2812COLOR color);

/**
 * @brief Get Color from RGB values
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @return Color
 */
WS2812COLOR ws2812GetColor(UINT8 r, UINT8 g, UINT8 b);