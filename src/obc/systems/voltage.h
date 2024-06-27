#ifndef _VOLTAGE_H
#define _VOLTAGE_H

#include <stdint.h>

/**
 * @brief Possible flags of voltage pins
 */
typedef enum voltage_pins_state
{
    VOLTAGE_PIN_STATE_3V3 = 1 << 0,
    VOLTAGE_PIN_STATE_5V = 1 << 1,
    VOLTAGE_PIN_STATE_VBAT = 1 << 2,
} voltage_pins_state_t;

/**
 * @brief Initialize voltage pins
 */
void voltage_init(void);

/**
 * @brief Update voltage control pins
 */
void voltage_update(void);

/**
 * @brief Get current voltage flags
 *
 * @return Flags
 */
uint8_t voltage_get_flags(void);

#endif