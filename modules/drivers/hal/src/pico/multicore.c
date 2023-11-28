#include "modules/drivers/hal/multicore.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"

static queue_t s_FIFOs[MULTICORE_MAX_FIFOS];

void hal_core_start_next(hal_core_func_t function)
{
    multicore_launch_core1(function);
}

void hal_core_init_fifo(size_t index, size_t fifoElementSize, size_t fifoElementsCount)
{
    queue_init(&s_FIFOs[index], fifoElementSize, fifoElementsCount);
}

void hal_core_send_data(size_t index, void *data)
{
    queue_add_blocking(&s_FIFOs[index], data);
}

void hal_core_receive_data(size_t index, void *data)
{
    queue_remove_blocking(&s_FIFOs[index], data);
}

void hal_core_set_as_victim(void)
{
    multicore_lockout_victim_init();
}

void hal_core_start_lock(void)
{
    multicore_lockout_start_blocking();
}

void hal_core_end_lock(void)
{
    multicore_lockout_end_blocking();
}