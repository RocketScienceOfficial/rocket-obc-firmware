#ifndef _BATTERY_UTILS_H
#define _BATTERY_UTILS_H

#include "hal/gpio_driver.h"
#include <stdint.h>

/**
 * @brief Battery interval data structure
 */
typedef struct battery_interval
{
    hal_voltage_level_t minVolts; /** Minimum voltage */
    hal_voltage_level_t maxVolts; /** Maximum voltage */
    uint8_t minPercent;           /** Minimum percent*/
    uint8_t maxPercent;           /** Maximum percent*/
} battery_interval_t;

/**
 * @brief Battery config data
 */
typedef struct battery_config
{
    battery_interval_t *intervals;         /** Battery intervals */
    uint8_t intervalsCount;                /** Battery intervals count */
    hal_voltage_level_t voltageDivider;    /** Raw voltage multiplier */
    hal_voltage_level_t oneCellMaxVoltage; /** One cell max votage */
} battery_config_t;

/**
 * @brief Battery conversion data
 */
typedef struct battery_data
{
    hal_voltage_level_t voltage; /** Total voltage */
    uint8_t percentage;          /** Charge state */
    uint8_t nCells;              /** Number of cells */
} battery_data_t;

/**
 * @brief Initialize battery
 *
 * @param config Battery config
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 * @param voltageDivider Voltage divider
 */
void battery_init(battery_config_t *config, battery_interval_t *intervals, uint8_t intervalsCount, hal_voltage_level_t voltageDivider);

/**
 * @brief Convert voltage to percent
 *
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 * @param voltage Voltage
 * @param data Data to set
 * @return Percent
 */
void battery_convert(const battery_config_t *config, hal_voltage_level_t rawVoltage, battery_data_t *data);

#endif