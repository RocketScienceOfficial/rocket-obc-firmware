#include "modules/drivers/hal/multithreading.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"

static queue_t s_FIFO;

bool core_start_next(core_func_t function, size_t fifoElementSize, size_t fifoElementsCount)
{
    if (function)
    {
        queue_init(&s_FIFO, fifoElementSize, fifoElementsCount);
        multicore_launch_core1(function);

        return true;
    }
    else
    {
        return false;
    }
}

void core_set_as_victim(void)
{
    multicore_lockout_victim_init();
}

void core_start_lock(void)
{
    multicore_lockout_start_blocking();
}

void core_end_lock(void)
{
    multicore_lockout_end_blocking();
}

void core_send_data(void *data)
{
    queue_add_blocking(&s_FIFO, data);
}

bool core_receive_data(void *data)
{
    if (data)
    {
        queue_remove_blocking(&s_FIFO, data);

        return true;
    }
    else
    {
        return false;
    }
}