#ifndef _TIME_TRACKER_H
#define _TIME_TRACKER_H

#include <stdbool.h>

typedef unsigned long long time_t; /** Time definition */

/**
 * @brief Gets the current time in milliseconds
 *
 * @return Current time in milliseconds
 */
time_t time_get_ms_since_boot();

/**
 * @brief Gets the current time in microseconds
 *
 * @return Current time in microseconds
 */
time_t time_get_us_since_boot();

/**
 * @brief Periodically checks if the time has passed
 *
 * @param ms Time in milliseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
bool time_run_every_ms(time_t ms, time_t *timerOffset);

/**
 * @brief Periodically checks if the time has passed
 *
 * @param us Time in microseconds
 * @param timerOffset Start time offset
 * @return True if the time has passed, false otherwise
 */
bool time_run_every_us(time_t us, time_t *timerOffset);

/**
 * @brief Sleeps for a given number of milliseconds
 *
 * @param ms Time in milliseconds
 */
void time_sleep_ms(time_t ms);

/**
 * @brief Sleeps for a given number of microseconds
 *
 * @param us Time in microseconds
 */
void time_sleep_us(time_t us);

#endif