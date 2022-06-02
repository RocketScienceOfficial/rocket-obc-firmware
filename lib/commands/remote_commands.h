#pragma once

#include "logger.h"

logger_data_t *myLogGetRemoteCommandLogger();

#define MY_LOG_REMOTE_COMMAND_CLIENT_NAME "CLIENT"
#define MY_LOG_REMOTE_COMMAND_SERVER_NAME "SERVER"
#define MY_LOG_REMOTE_COMMAND_NAME "REMOTE_COMMAND"
#define MY_LOG_REMOTE_COMMAND_CLIENT(msg, ...) myLog(myLogGetRemoteCommandLogger(), MY_LOG_REMOTE_COMMAND_CLIENT_NAME, msg, ##__VA_ARGS__)
#define MY_LOG_REMOTE_COMMAND_SERVER(msg, ...) myLog(myLogGetRemoteCommandLogger(), MY_LOG_REMOTE_COMMAND_SERVER_NAME, msg, ##__VA_ARGS__)