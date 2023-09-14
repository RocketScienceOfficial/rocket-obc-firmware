#include "drivers/tools/multithreading.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"

static queue_t s_FIFO;

BOOL startOtherCore(PicoCoreFunction function, SIZE fifoElementSize, SIZE fifoElementsCount)
{
    if (function)
    {
        queue_init(&s_FIFO, fifoElementSize, fifoElementsCount);
        multicore_launch_core1(function);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

VOID setAsVictimCore(VOID)
{
    multicore_lockout_victim_init();
}

VOID coreStartLockout(VOID)
{
    multicore_lockout_start_blocking();
}

VOID coreEndLockout(VOID)
{
    multicore_lockout_end_blocking();
}

BOOL sendToOtherCore(VOID *data)
{
    queue_add_blocking(&s_FIFO, data);

    return TRUE;
}

BOOL receiveFromOtherCore(VOID *data)
{
    if (data)
    {
        queue_remove_blocking(&s_FIFO, data);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}