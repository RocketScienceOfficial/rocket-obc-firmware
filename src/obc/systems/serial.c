#include "serial.h"
#include "sm.h"
#include "../middleware/events.h"
#include "hal/serial_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

static uint8_t s_Buffer[512];
static size_t s_CurrentSize;
static datalink_frame_structure_serial_t s_CurrentFrame;
static bool s_FrameSet;

void serial_init(void)
{
    SERIAL_DEBUG_PRINTF("READY");
}

void serial_update(void)
{
    if (sm_get_state() == FLIGHT_STATE_STANDING || sm_get_state() == FLIGHT_STATE_LANDED)
    {
        int c = 0;

        if (hal_serial_read_char(&c))
        {
            if (c == 0)
            {
                s_FrameSet = (bool)datalink_deserialize_frame_serial(&s_CurrentFrame, s_Buffer, s_CurrentSize);
                s_CurrentSize = 0;

                if (s_FrameSet)
                {
                    events_publish(MSG_SERIAL_MESSAGE_RECEIVED);
                }
            }
            else
            {
                s_Buffer[s_CurrentSize++] = (uint8_t)c;
            }
        }
    }
}

void serial_send_message(const datalink_frame_structure_serial_t *message)
{
    uint8_t buffer[512];
    int len = sizeof(buffer);
    datalink_serialize_frame_serial(message, buffer, &len);

    hal_serial_send_buffer(buffer, len);
}

const datalink_frame_structure_serial_t *serial_get_current_message(void)
{
    return s_FrameSet ? &s_CurrentFrame : NULL;
}