#pragma once

#include <stddef.h>

#define EVENT_MAX_LISTENERS 8

typedef void (*eventCallback_t)(void **, size_t);

typedef struct event_data
{
    eventCallback_t callback;
    int id;
} event_data_t;

typedef struct event_handler
{
    event_data_t events[EVENT_MAX_LISTENERS];
    int eventsCount;
    int code;
    int _lastId;
} event_handler_t;

void initHandler(event_handler_t *handler_out);
void callEvent(event_handler_t *handler, void **data, size_t size);
void addEvent(event_handler_t *handler, eventCallback_t callback, int *id_out);
void removeEvent(event_handler_t *handler, int *id);