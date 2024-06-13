#include "serial.h"
#include "sm.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/serial_driver.h"
#include <stddef.h>
#include <string.h>

#define SYSTEM_NAME "serial"

static char s_CMD[128];
static size_t s_CurrentSize;

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
                if ((c >= (int)'0' && c <= (int)'9') || (c >= (int)'A' && c <= (int)'Z') || (c >= (int)'a' && c <= (int)'z') || (c == (int)'-') || (c == (int)'_'))
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

static void _reset(void)
{
    memset(s_CMD, 0, sizeof(s_CMD));

    s_CurrentSize = 0;
}

static void _submit_cmd(void)
{
    SYS_LOG("Submiting command: %s", s_CMD);

    if (strcmp(s_CMD, "data-read") == 0)
    {
        events_publish(MSG_CMD_DATA_READ);
    }
    else if (strcmp(s_CMD, "data-clear") == 0)
    {
        events_publish(MSG_CMD_DATA_CLEAR);
    }
    else if (strcmp(s_CMD, "data-recovery") == 0)
    {
        events_publish(MSG_CMD_DATA_RECOVERY);
    }
}