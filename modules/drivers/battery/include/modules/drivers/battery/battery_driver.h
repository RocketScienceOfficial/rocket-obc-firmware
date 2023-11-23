#ifndef _BATTERY_DRIVER_H
#define _BATTERY_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/adc_driver.h"
#include <stdint.h>

/**
 * @brief Battery interval data structure
 */
typedef struct battery_interval
{
    float minVolts;   /** Minimum voltage */
    float maxVolts;   /** Maximum voltage */
    float minPercent; /** Minimum percent*/
    float maxPercent; /** Maximum percent*/
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
 * @brief Read battery percentage
 *
 * @param config Battery data pointer
 * @return Percentage of battery
 */
float battery_read_percent(battery_config_t *config);

/**
 * @brief Convert voltage to percent
 *
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 * @param voltage Voltage
 * @return Percent
 */
float battery_convert_voltage_to_percent(battery_interval_t *intervals, uint8_t intervalsCount, hal_voltage_level_t voltage);

#endif