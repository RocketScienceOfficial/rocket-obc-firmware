#include "utils/multithreading.h"
#include "pico/multicore.h"

bool startCore1(PicoCoreFunction function)
{
    if (function)
    {
        multicore_launch_core1(function);

        return true;
    }
    else
    {
        return false;
    }
}

bool sendToOtherCore(uint32_t data)
{
    multicore_fifo_push_blocking(data);

    return true;
}

bool receiveFromOtherCore(uint32_t *data)
{
    if (data)
    {
        *data = multicore_fifo_pop_blocking();

        return true;
    }
    else
    {
        return false;
    }
}