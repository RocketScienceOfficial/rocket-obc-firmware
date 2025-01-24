#ifndef _TIME_TRACKER_H
#define _TIME_TRACKER_H

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t hal_sec_t;  /** Second definition */
typedef uint32_t hal_msec_t; /** Milisecond definition */
typedef uint32_t hal_usec_t; /** Microsecond definition */

/**
 * @brief Gets the current time in milliseconds
 *
 * @return Current time in milliseconds
 */
hal_msec_t hal_time_get_ms_since_boot();

/**
 * @brief Gets the current time in microseconds
 *
 * @return Current time in microseconds
 */
hal_usec_t hal_time_get_us_since_boot();

/**
 * @brief Sleeps for a given number of milliseconds
 *
 * @param ms Time in milliseconds
 */
void hal_time_sleep_ms(hal_msec_t ms);

/**
 * @brief Sleeps for a given number of microseconds
 *
 * @param us Time in microseconds
 */
void hal_time_sleep_us(hal_usec_t us);

#endif