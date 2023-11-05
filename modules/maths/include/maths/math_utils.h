#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

/**
 * @brief Fast inverse square root from Quake III. See: https://en.wikipedia.org/wiki/Fast_inverse_square_root
 *
 * @param x Number to calculate the inverse square root of
 * @return float Inverse square root of x
 */
float fast_inv_sqrt(float x);

/**
 * @brief Calculate the height (up to 11km!) from the barometric formula
 *
 * @param pressure Pressure in Pascals
 * @param temperature Temperature in Celsius
 * @return Height in meters
 */
float height_from_baro_formula(float pressure, float temperature);

#endif