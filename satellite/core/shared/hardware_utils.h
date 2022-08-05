#pragma once

#include "errors_types.h"
#include "logger.h"

#define HW_CALL(func)                                                \
    {                                                                \
        FUNCRESULT result = func;                                    \
        if (FUNCFAILED(result))                                      \
        {                                                            \
            MY_LOG_CORE_ERROR("Hardware function failed: ", result); \
        }                                                            \
    }
