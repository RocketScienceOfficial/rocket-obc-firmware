#ifndef _E19_433M20SC_DRIVER_H
#define _E19_433M20SC_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"

/**
 * @brief Configuration of E19-433M20SC LoRa module
 */
typedef struct e19_433m20sc_config
{
    pin_number_t rxen; /** Switch to enable receiving */
    pin_number_t txen; /** Switch to enable transmitting */
} e19_433m20sc_config_t;

/**
 * @brief State of E19-433M20SC LoRa module
 */
typedef enum e19_433m20sc_state
{
    E19_433M20SC_STATE_RECEIVE,
    E19_433M20SC_STATE_TRANSMIT,
    E19_433M20SC_STATE_OFF,
} e19_433m20sc_state_t;

/**
 * @brief Initialize E19-433M20SC LoRa module
 *
 * @param config Configuration of E19-433M20SC LoRa module
 * @param rxen Pin number of RXEN pin
 * @param txen Pin number of TXEN pin
 */
void e19_433m20sc_init(e19_433m20sc_config_t *config, pin_number_t rxen, pin_number_t txen);

/**
 * @brief Set state of E19-433M20SC LoRa module
 *
 * @param config Configuration of E19-433M20SC LoRa module
 * @param state State of E19-433M20SC LoRa module
 */
void e19_433m20sc_set_state(e19_433m20sc_config_t *config, e19_433m20sc_state_t state);

#endif