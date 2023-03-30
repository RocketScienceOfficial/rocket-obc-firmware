#pragma once

#include <obc/api.h>
#include "kernel/logging/logger.h"

/**
 * @brief Call a function which returns FUNCRESULT and handle result
 */
#define DRIVER_CALL(func)                                                                                       \
    {                                                                                                           \
        FUNCRESULT result = func;                                                                               \
        if (FUNCFAILED(result))                                                                                 \
        {                                                                                                       \
            MY_LOG_CORE_ERROR("Hardware function failed with code: %d at '%s:%d'", result, __FILE__, __LINE__); \
        }                                                                                                       \
    }
