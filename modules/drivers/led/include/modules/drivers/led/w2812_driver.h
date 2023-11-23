#ifndef _W2812_DRIVER_H
#define _W2812_DRIVER_H

/**
 * REF: https://www.youtube.com/watch?v=rHoFqKGOPRI
 * REF: https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */

#include "modules/drivers/hal/gpio_driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef unsigned int WS2812COLOR; /** WS2812 color */

/**
 * @brief Initializes WS2812 LED strip
 *
 * @param pin Pin number
 * @param rgbw true if RGBW LEDs are used, false if RGB LEDs are used
 */
void ws2812_init(hal_pin_number_t pin, bool rgbw);

/**
 * @brief Sets colors of all LEDs
 *
 * @param colors Array of colors
 * @param count Number of colors
 */
void ws2812_set_colors(WS2812COLOR *colors, size_t count);

/**
 * @brief Get Color from RGB values. Disabled color is 0. To control brightness adjust each component.
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @return Color
 */
WS2812COLOR ws2812_get_color(uint8_t r, uint8_t g, uint8_t b);

#endif