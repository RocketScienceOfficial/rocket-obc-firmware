#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include <stdbool.h>

/**
 * @brief Fast inverse square root from Quake III. See: https://en.wikipedia.org/wiki/Fast_inverse_square_root
 *
 * @param x Number to calculate the inverse square root of
 * @return float Inverse square root of x
 */
float fast_inv_sqrt(float x);

/**
 * @brief Clamps value between min and max
 * 
 * @param x Value
 * @param min Minimum value
 * @param max Maximum value
 * @return Clamped value
*/
float clamp_value(float x, float min, float max);

/**
 * @brief Calculate the height (up to 11km!) from the barometric formula
 *
 * @param pressure Pressure in Pascals
 * @param temperature Temperature in Celsius
 * @return Height in meters
 */
float height_from_baro_formula(float pressure, float temperature);

/**
 * @brief Check if value is approximately equal desired value
 * 
 * @param val Value to be checked
 * @param des Desired value
 * @param eps Accepted error
 * @return true if value is approximately equal desired value
*/
bool value_approx_eql(float val, float des, float eps);

#endif