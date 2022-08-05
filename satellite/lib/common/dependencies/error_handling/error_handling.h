#pragma once

#include "pico/stdlib.h"

/**
 * @brief Crashes system.
 */
#define REPORT_CRASH(error) panic(error)