#pragma once

#include <obc/api.h>

/**
 * @brief Fast inverse square root from Quake III. https://en.wikipedia.org/wiki/Fast_inverse_square_root
 *
 * @param x Number to calculate the inverse square root of
 * @return FLOAT Inverse square root of x
 */
FLOAT fastInverseSqrt(FLOAT x);

/**
 * @brief Clamp a value between a minimum and maximum
 *
 * @param value Value to clamp
 * @param min Minimum value
 * @param max Maximum value
 * @return FLOAT Clamped value
 */
FLOAT clampValue(FLOAT value, FLOAT min, FLOAT max);

/**
 * @brief Calculate the height (up to 11km!) from the barometric formula
 *
 * @param pressure Pressure in Pascals
 * @param temperature Temperature in Celsius
 * @return Height in meters
 */
FLOAT heightFromBarometricFormula(FLOAT pressure, FLOAT temperature);