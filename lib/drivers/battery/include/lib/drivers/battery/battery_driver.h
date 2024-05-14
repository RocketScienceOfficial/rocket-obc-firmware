#ifndef _BATTERY_DRIVER_H
#define _BATTERY_DRIVER_H

#include "hal/gpio_driver.h"
#include "hal/adc_driver.h"
#include <stdint.h>

/**
 * @brief Battery interval data structure
 */
typedef struct battery_interval
{
    float minVolts;   /** Minimum voltage */
    float maxVolts;   /** Maximum voltage */
    uint8_t minPercent; /** Minimum percent*/
    uint8_t maxPercent; /** Maximum percent*/
} battery_interval_t;

/**
 * @brief Battery config data
 */
typedef struct battery_config
{
    hal_adc_input_t input;         /** ADC input */
    battery_interval_t *intervals; /** Battery intervals */
    uint8_t intervalsCount;        /** Battery intervals count */
} battery_config_t;

/**
 * @brief Initialize battery
 *
 * @param config Battery data pointer
 * @param input ADC input
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 */
void battery_init(battery_config_t *config, hal_adc_input_t input, battery_interval_t *intervals, uint8_t intervalsCount);

/**
 * @brief Read battery data
 *
 * @param config Battery data pointer
 * @param percent Percentage of battery
 * @param voltage Voltage of battery
 */
void battery_read(battery_config_t *config, uint8_t *percent, hal_voltage_level_t *voltage);

/**
 * @brief Convert voltage to percent
 *
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 * @param voltage Voltage
 * @return Percent
 */
uint8_t battery_convert_voltage_to_percent(battery_interval_t *intervals, uint8_t intervalsCount, hal_voltage_level_t voltage);

#endif