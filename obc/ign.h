#ifndef _IGN_H
#define _IGN_H

#include "hal/gpio_driver.h"
#include "hal/time_tracker.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Data for firing single pin
 */
typedef struct ign_pin_data
{
    hal_pin_number_t pin;
    bool setHigh;
    bool fired;
    msec_t delay;
    msec_t time;
    bool finished;
} ign_pin_data_t;

/**
 * @brief Igniter pin state data
 */
typedef struct ign_pin_state
{
    bool fuseWorking;
    bool ignPresent;
} ign_pin_state_t;

/**
 * @brief Initialize igniters
 */
void ign_init(void);

/**
 * @brief Update igniters
 */
void ign_update(void);

/**
 * @brief Check provided pin state
 *
 * @param checkPinVoltage Voltage of check pin
 * @return Pin state
 */
ign_pin_state_t ign_check_pin(hal_voltage_level_t checkPinVoltage);

#endif