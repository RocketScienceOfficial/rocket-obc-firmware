#pragma once

/**
 * REF: https://www.youtube.com/watch?v=rHoFqKGOPRI
 * REF: https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

typedef UINT32 WS2812COLOR; /** WS2812 color */

/**
 * @brief Initializes WS2812 LED strip
 *
 * @param pin Pin number
 * @param rgbw TRUE if RGBW LEDs are used, FALSE if RGB LEDs are used
 * @return Result code
 */
FUNCRESULT ws2812Init(PinNumber pin, BOOL rgbw);

/**
 * @brief Sets colors of all LEDs
 *
 * @param colors Array of colors
 * @param count Number of colors
 * @return Result code
 */
FUNCRESULT ws2812SetColors(WS2812COLOR *colors, SIZE count);

/**
 * @brief Get Color from RGB values. Disabled color is 0. To control brightness adjust each component.
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @return Color
 */
WS2812COLOR ws2812GetColor(UINT8 r, UINT8 g, UINT8 b);

/**
 * @brief Sets color of a single LED
 *
 * @param color Color
 * @return Result code
 */
VOID __ws2812SendColor(WS2812COLOR color);