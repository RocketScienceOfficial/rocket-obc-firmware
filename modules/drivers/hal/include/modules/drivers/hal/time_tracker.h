#ifndef _TIME_TRACKER_H
#define _TIME_TRACKER_H

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t sec_t;  /** Second definition */
typedef uint32_t msec_t; /** Milisecond definition */
typedef uint32_t usec_t; /** Microsecond definition */

/**
 * @brief Gets the current time in milliseconds
 *
 * @return Current time in milliseconds
 */
msec_t hal_time_get_ms_since_boot();

/**
 * @brief Gets the current time in microseconds
 *
 * @return Current time in microseconds
 */
usec_t hal_time_get_us_since_boot();

/**
 * @brief Periodically checks if the time has passed
 *
 * @param ms Time in milliseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
bool hal_time_run_every_ms(msec_t ms, msec_t *timerOffset);

/**
 * @brief Periodically checks if the time has passed
 *
 * @param us Time in microseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
bool hal_time_run_every_us(usec_t us, usec_t *timerOffset);

/**
 * @brief Sleeps for a given number of milliseconds
 *
 * @param ms Time in milliseconds
 */
void hal_time_sleep_ms(msec_t ms);

/**
 * @brief Sleeps for a given number of microseconds
 *
 * @param us Time in microseconds
 */
void hal_time_sleep_us(usec_t us);

#endif