#include "remote_commands.h"

static logger_data_t s_Logger;
static int s_LoggerInitialized;

logger_data_t *myLogGetRemoteCommandLogger()
{
    if (!s_LoggerInitialized)
    {
        myLogCreateLogger(&s_Logger, MY_LOG_REMOTE_COMMAND_NAME);

        s_LoggerInitialized = 1;
    }

    return &s_Logger;
}