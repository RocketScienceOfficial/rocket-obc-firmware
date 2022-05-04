#include "measurements_logger.h"
#include "logging_utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static const int BUFFER_SIZE = 64;

static event_handler_t s_eventHandler;
static char *buffer = NULL;

void myLogMeasureInit()
{
    initHandler(&s_eventHandler);
}

void myLogMeasureAddCallback(eventCallback_t callback)
{
    addEvent(&s_eventHandler, callback, NULL);
}

void myLogMeasureBegin()
{
    buffer = calloc(BUFFER_SIZE, sizeof(char));
}

void myLogMeasure(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    char *message = vlogBase(format, ap);
    va_end(ap);

    strcat(buffer, message);

    free(message);
}

void myLogMeasureEnd()
{
    char *txt = logBase("%s\n", buffer);
    void *data[1] = {txt};

    callEvent(&s_eventHandler, data, 1);

    free(txt);
    free(buffer);
}