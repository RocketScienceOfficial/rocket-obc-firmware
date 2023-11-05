#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Configuration of E19-433M20SC LoRa module
 */
typedef struct E19_433M20SC_Config
{
    PinNumber rxen; /** Switch to enable receiving */
    PinNumber txen; /** Switch to enable transmitting */
} E19_433M20SC_Config;

/**
 * @brief State of E19-433M20SC LoRa module
 */
typedef enum E19_433M20SC_State
{
    E19_433M20SC_STATE_RECEIVE,
    E19_433M20SC_STATE_TRANSMIT,
    E19_433M20SC_STATE_OFF,
} E19_433M20SC_State;

/**
 * @brief Initialize E19-433M20SC LoRa module
 *
 * @param config Configuration of E19-433M20SC LoRa module
 * @param rxen Pin number of RXEN pin
 * @param txen Pin number of TXEN pin
 * @return Result code
 */
FUNCRESULT e19_433m20sc_Init(E19_433M20SC_Config *config, PinNumber rxen, PinNumber txen);

/**
 * @brief Set state of E19-433M20SC LoRa module
 *
 * @param config Configuration of E19-433M20SC LoRa module
 * @param state State of E19-433M20SC LoRa module
 * @return Result code
 */
FUNCRESULT e19_433m20sc_SetState(E19_433M20SC_Config *config, E19_433M20SC_State state);