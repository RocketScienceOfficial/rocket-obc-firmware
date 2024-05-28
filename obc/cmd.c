#include "cmd.h"
#include "hal/serial_driver.h"
#include <stddef.h>
#include <string.h>

static char s_CMD[128];
static size_t s_CurrentSize;
static cmd_type_t s_CurrentCMD;

#define MAP_CMD(cmd, type)         \
    if (strcmp(s_CMD, (cmd)) == 0) \
    {                              \
        s_CurrentCMD = (type);     \
        return;                    \
    }

static void _reset(void);
static void _submit_cmd(void);

void cmd_init(void)
{
    _reset();
}

void cmd_update(void)
{
    s_CurrentCMD = CMD_UNKNOWN;

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
            if ((c >= (int)'0' && c <= (int)'9') || (c >= (int)'A' && c <= (int)'Z') || (c >= (int)'a' && c <= (int)'z'))
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

cmd_type_t get_current_cmd(void)
{
    return s_CurrentCMD;
}

static void _reset(void)
{
    memset(s_CMD, 0, sizeof(s_CMD));

    s_CurrentSize = 0;

    s_CurrentCMD = CMD_UNKNOWN;
}

static void _submit_cmd(void)
{
    MAP_CMD("data-read", CMD_DATA_READ);

    s_CurrentCMD = CMD_UNKNOWN;
}