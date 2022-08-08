#pragma once

#include "utils/errors_types.h"
#include "logging/logger.h"

#define HW_CALL(func)                                                                                         \
    {                                                                                                         \
        FUNCRESULT result = func;                                                                             \
        if (FUNCFAILED(result))                                                                               \
        {                                                                                                     \
            MY_LOG_CORE_ERROR("Hardware function with code failed: %d at '%s:%d'", result, __FILE__, __LINE__); \
        }                                                                                                     \
    }
