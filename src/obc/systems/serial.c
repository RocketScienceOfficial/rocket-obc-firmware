#include "serial.h"
#include "sm.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/serial_driver.h"
#include <string.h>

#define SYSTEM_NAME "serial"

static char s_CMD[128];
static size_t s_CurrentSize;
static char s_CMDTokens[8][32];
static size_t s_CMDTokensSize;

static void _reset(void);
static void _submit_cmd(void);

void serial_init(void)
{
    _reset();

    SYS_LOG("READY");
}

void serial_update(void)
{
    if (sm_get_state() == FLIGHT_STATE_STANDING || sm_get_state() == FLIGHT_STATE_LANDED)
    {
        int c = 0;

        if (hal_serial_read_char(&c))
        {
            if (c == (int)'\r')
            {
                if (s_CurrentSize >= sizeof(s_CMD))
                {
                    _reset();
                }
                else
                {
                    s_CMD[s_CurrentSize] = '\0';

                    _submit_cmd();
                    _reset();
                }
            }
            else
            {
                if ((c >= (int)'0' && c <= (int)'9') || (c >= (int)'A' && c <= (int)'Z') || (c >= (int)'a' && c <= (int)'z') || (c == (int)'-') || (c == (int)'_') || (c == (int)'\\') || (c == (int)' ') || (c == (int)','))
                {
                    if (s_CurrentSize >= sizeof(s_CMD))
                    {
                        _reset();
                    }
                    else
                    {
                        s_CMD[s_CurrentSize++] = (char)c;
                    }
                }
            }
        }
    }
}

const char *serial_get_param_at_index(size_t index)
{
    return s_CMDTokensSize > index + 1 ? s_CMDTokens[index + 1] : NULL;
}

size_t serial_get_params_count(void)
{
    return s_CMDTokensSize;
}

static void _reset(void)
{
    memset(s_CMD, 0, sizeof(s_CMD));

    s_CurrentSize = 0;
}

static void _submit_cmd(void)
{
    SYS_LOG("Submiting command: %s", s_CMD);

    s_CMDTokensSize = 0;

    size_t cIndex = 0;

    for (size_t i = 0; i < s_CurrentSize; i++)
    {
        if (s_CMD[i] == ' ' || s_CMD[i] == ',')
        {
            s_CMDTokens[s_CMDTokensSize][cIndex] = '\0';

            s_CMDTokensSize++;
            cIndex = 0;
        }
        else
        {
            s_CMDTokens[s_CMDTokensSize][cIndex++] = s_CMD[i];
        }
    }

    s_CMDTokens[s_CMDTokensSize][cIndex] = '\0';
    s_CMDTokensSize++;

    if (strcmp(s_CMDTokens[0], "\\data-read-start") == 0)
    {
        events_publish(MSG_CMD_DATA_READ);
    }
    else if (strcmp(s_CMDTokens[0], "\\data-clear-start") == 0)
    {
        events_publish(MSG_CMD_DATA_CLEAR);
    }
    else if (strcmp(s_CMDTokens[0], "\\data-recovery-start") == 0)
    {
        events_publish(MSG_CMD_DATA_RECOVERY);
    }
    else if (strcmp(s_CMDTokens[0], "\\ign-test") == 0)
    {
        events_publish(MSG_CMD_IGN_TEST);
    }
    else if (strcmp(s_CMDTokens[0], "\\config-get") == 0)
    {
        events_publish(MSG_CMD_CONFIG_GET);
    }
    else if (strcmp(s_CMDTokens[0], "\\config-set-start") == 0)
    {
        events_publish(MSG_CMD_CONFIG_SET);
    }
}