#ifndef _BATTERY_UTILS_H
#define _BATTERY_UTILS_H

#include "hal/gpio_driver.h"
#include <stdint.h>

/**
 * @brief Battery entry data structure
 */
typedef struct battery_table_entry
{
    hal_voltage_level_t voltage; /** Voltage */
    uint8_t percentage;          /** Percentage */
} battery_table_entry_t;

/**
 * @brief Battery config data
 */
typedef struct battery_config
{
    battery_table_entry_t *entries;        /** Battery entries */
    uint8_t entriesCount;                  /** Battery entries count */
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
 * @param entries Entries in decreasing order for a single cell
 * @param entriesCount Count of entries. Must be at least 2!
 * @param voltageDivider Voltage divider
 */
void battery_init(battery_config_t *config, battery_table_entry_t *entries, uint8_t entriesCount, hal_voltage_level_t voltageDivider);

/**
 * @brief Convert voltage to percent
 *
 * @param entries Entries
 * @param entriesCount Count of entries
 * @param voltage Voltage
 * @param data Data to set
 * @return Percent
 */
void battery_convert(const battery_config_t *config, hal_voltage_level_t rawVoltage, battery_data_t *data);

#endif