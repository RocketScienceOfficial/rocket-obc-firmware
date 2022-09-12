#include "shared/tick.h"
#include "logging/logger.h"

void tick(TickData *data)
{
    if (runEvery(TICK_DELAY_MS, &data->_timer))
    {
        MY_LOG_CORE_INFO("TICK");
    }
}