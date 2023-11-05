#pragma once

#include <obc/api.h>

/**
 * @brief Voltage type
 */
typedef enum VoltageType
{
    VOLTAGE_3V3,
    VOLTAGE_5V,
    VOLTAGE_VBAT,
} VoltageType;

/**
 * @brief Initialize voltage control pins
 */
VOID voltageInit(VOID);

/**
 * @brief Set voltage
 * 
 * @param type Voltage type
 * @param enable Enable or disable
 */
VOID voltageSet(VoltageType type, BOOL enable);