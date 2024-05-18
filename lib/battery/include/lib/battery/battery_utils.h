#ifndef _BATTERY_UTILS_H
#define _BATTERY_UTILS_H

#include "hal/gpio_driver.h"
#include <stdint.h>

/**
 * @brief Battery interval data structure
 */
typedef struct battery_interval
{
    float minVolts;     /** Minimum voltage */
    float maxVolts;     /** Maximum voltage */
    uint8_t minPercent; /** Minimum percent*/
    uint8_t maxPercent; /** Maximum percent*/
} battery_interval_t;

/**
 * @brief Battery config data
 */
typedef struct battery_config
{
    battery_interval_t *intervals; /** Battery intervals */
    uint8_t intervalsCount;        /** Battery intervals count */
} battery_config_t;

/**
 * @brief Initialize battery
 *
 * @param config Battery config
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 */
void battery_init(battery_config_t *config, battery_interval_t *intervals, uint8_t intervalsCount);

/**
 * @brief Convert voltage to percent
 *
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 * @param voltage Voltage
 * @return Percent
 */
uint8_t battery_convert(battery_config_t *config, hal_voltage_level_t voltage);

#endif