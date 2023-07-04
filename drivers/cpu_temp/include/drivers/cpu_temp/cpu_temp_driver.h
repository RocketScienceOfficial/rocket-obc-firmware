#pragma once

#include <obc/api.h>

/**
 * @brief Initialize CPU Temperature Sensor
 * 
 * @return Result code
 */
FUNCRESULT cpuTempInit();

/**
 * @brief Read CPU temp
 * 
 * @param temp Temperature of CPU
 * @return Result code
 */
FUNCRESULT cpuTempReadTemp(FLOAT *temp);