#include "my_event.h"
#include <stdlib.h>

static int s_currentCode = 0;

void initHandler(event_handler_t *handler_out)
{
    handler_out->eventsCount = 0;
    handler_out->code = s_currentCode;
    handler_out->_lastId = 0;

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
    *id_out = handler->_lastId;
    handler->events[handler->eventsCount] = (event_data_t){callback, *id_out};
    handler->eventsCount++;
    handler->_lastId++;
}

void removeEvent(event_handler_t *handler, int *id)
{
    int index = -1;

    for (int i = 0; i < handler->eventsCount; i++)
    {
        if (handler->events[i].id == *id)
        {
            handler->events[i].callback = NULL;
            handler->events[i].id = -1;

            index = i;

            break;
        }
    }

    if (index != -1)
    {
        for (int i = index; i < handler->eventsCount - 1; i++)
        {
            handler->events[i] = handler->events[i + 1];
        }

        handler->eventsCount--;
    }
}