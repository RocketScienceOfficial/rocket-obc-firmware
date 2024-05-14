#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include <stdbool.h>

/**
 * @brief Clamps float type value between min and max
 * 
 * @param x Value
 * @param min Minimum value
 * @param max Maximum value
 * @return Clamped value
*/
float clamp_value(float x, float min, float max);

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