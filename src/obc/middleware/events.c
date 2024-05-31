#include "events.h"
#include "syslog.h"
#include <stddef.h>

#define SYSTEM_NAME "info"

#define MESSAGES_MAX_LENGTH 16

static msg_type_t s_Messages[MESSAGES_MAX_LENGTH];
static size_t s_MessagesSize;

void events_dispatch(void)
{
    s_MessagesSize = 0;
}

void events_publish(msg_type_t msg)
{
    if (s_MessagesSize >= MESSAGES_MAX_LENGTH)
    {
        SYS_LOG("ERR: Too many messages have been published");

        return;
    }

    s_Messages[s_MessagesSize++] = msg;
}

bool events_poll(msg_type_t msg)
{
    for (size_t i = 0; i < s_MessagesSize; i++)
    {
        if (s_Messages[i] == msg)
        {
            return true;
        }
    }

    return false;
}