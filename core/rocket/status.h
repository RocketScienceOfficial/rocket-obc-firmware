#pragma once

#include <obc/api.h>

/**
 * @brief Initializes status system
 */
VOID initStatus(VOID);

/**
 * @brief Updates status
 */
VOID updateStatus(VOID);

/**
 * @brief Sets diode to indicate LoRa TX
 *
 * @param state TRUE to turn on diode, FALSE to turn off diode
 */
VOID setLoRaTXDiode(BOOL state);