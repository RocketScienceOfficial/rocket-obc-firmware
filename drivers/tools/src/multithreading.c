#include "drivers/tools/multithreading.h"
#include "pico/multicore.h"

BOOL startCore1(PicoCoreFunction function)
{
    if (function)
    {
        multicore_launch_core1(function);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL sendToOtherCore(UINT32 data)
{
    multicore_fifo_push_blocking(data);

    return TRUE;
}

BOOL receiveFromOtherCore(UINT32 *data)
{
    if (data)
    {
        *data = multicore_fifo_pop_blocking();

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}