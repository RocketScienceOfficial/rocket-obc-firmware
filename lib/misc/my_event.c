#include "my_event.h"
#include <stdlib.h>

static int s_currentCode = 0;

void initHandler(event_handler_t *handler_out)
{
    handler_out->events = NULL;
    handler_out->eventsCount = 0;
    handler_out->code = s_currentCode;

    s_currentCode++;
}

void callEvent(event_handler_t *handler, void **data, size_t size)
{
    for (int i = 0; i < handler->eventsCount; i++)
    {
        handler->events[i].callback(data, size);
    }
}

void addEvent(event_handler_t *handler, eventCallback_t callback, int *id_out)
{
    event_data_t *oldEvents = handler->events;
    int oldEventsCount = handler->eventsCount;

    handler->events = malloc(sizeof(event_data_t) * (oldEventsCount + 1));

    for (int i = 0; i < oldEventsCount; i++)
    {
        handler->events[i] = oldEvents[i];
    }

    handler->events[oldEventsCount] = (event_data_t){callback, handler->_lastId + 1};
    handler->eventsCount++;
    handler->_lastId++;

    if (id_out)
    {
        *id_out = handler->_lastId;
    }

    free(oldEvents);
}

void removeEvent(event_handler_t *handler, int *id)
{
    event_data_t *oldEvents = handler->events;
    int oldEventsCount = handler->eventsCount;

    handler->events = malloc(sizeof(event_data_t) * (oldEventsCount - 1));

    for (int i = 0; i < oldEventsCount; i++)
    {
        if (oldEvents[i].id != *id)
        {
            handler->events[i] = oldEvents[i];
        }
    }

    handler->eventsCount--;

    free(oldEvents);
}