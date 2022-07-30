#pragma once

#include "logger.h"

logger_data_t *myLogGetRemoteCommandLogger();

#define MY_LOG_REMOTE_COMMAND_NAME "REMOTE_COMMAND"
#define MY_LOG_RPC(msg, ...) myLog(myLogGetRemoteCommandLogger(), "", msg, ##__VA_ARGS__)