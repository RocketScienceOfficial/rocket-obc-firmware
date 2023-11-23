#ifndef _TIME_TRACKER_H
#define _TIME_TRACKER_H

#include <stdbool.h>

typedef unsigned long long hal_time_t; /** Time definition */

/**
 * @brief Gets the current time in milliseconds
 *
 * @return Current time in milliseconds
 */
hal_time_t hal_time_get_ms_since_boot();

/**
 * @brief Gets the current time in microseconds
 *
 * @return Current time in microseconds
 */
hal_time_t hal_time_get_us_since_boot();

/**
 * @brief Periodically checks if the time has passed
 *
 * @param ms Time in milliseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
bool hal_time_run_every_ms(hal_time_t ms, hal_time_t *timerOffset);

/**
 * @brief Periodically checks if the time has passed
 *
 * @param us Time in microseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
bool hal_time_run_every_us(hal_time_t us, hal_time_t *timerOffset);

/**
 * @brief Sleeps for a given number of milliseconds
 *
 * @param ms Time in milliseconds
 */
void hal_time_sleep_ms(hal_time_t ms);

/**
 * @brief Sleeps for a given number of microseconds
 *
 * @param us Time in microseconds
 */
void hal_time_sleep_us(hal_time_t us);

#endif