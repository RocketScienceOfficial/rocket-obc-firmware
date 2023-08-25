#pragma once

#include <obc/api.h>

/**
 * @brief Initializes ignition system
 */
VOID initIgn(VOID);

/**
 * @brief Checks for ignition and fires if mission is done
 */
VOID checkIgn(VOID);