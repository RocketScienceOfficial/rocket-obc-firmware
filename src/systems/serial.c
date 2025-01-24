#include "serial.h"
#include "sm.h"
#include "../middleware/events.h"
#include <cobs.h>
#include <hal/serial_driver.h>
#include <hal/board_control.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

static uint8_t s_Buffer[512];
static size_t s_CurrentSize;
static datalink_frame_structure_serial_t s_CurrentFrame;
static bool s_FrameSet;

static void _check_usb_connection(void);

void serial_init(void)
{
    SERIAL_DEBUG_LOG("READY");
}

void serial_update(void)
{
    _check_usb_connection();

    static uint8_t cobsDecodedBuffer[512];

    if (sm_get_state() == FLIGHT_STATE_STANDING || sm_get_state() == FLIGHT_STATE_LANDED)
    {
        int c = 0;

        if (hal_serial_read_char(&c))
        {
            if (s_CurrentSize >= sizeof(s_Buffer))
            {
                s_CurrentSize = 0;
            }

            s_Buffer[s_CurrentSize++] = (uint8_t)c;

            if (c == 0x00)
            {
                int newLen = cobs_decode(s_Buffer, s_CurrentSize, cobsDecodedBuffer) - 1;

                s_FrameSet = (bool)datalink_deserialize_frame_serial(&s_CurrentFrame, cobsDecodedBuffer, newLen);
                s_CurrentSize = 0;

                if (s_FrameSet)
                {
                    events_publish(MSG_SERIAL_MESSAGE_RECEIVED);
                }
            }
        }
    }
}

void serial_send_message(const datalink_frame_structure_serial_t *message)
{
    static uint8_t buffer[512];
    static uint8_t cobsEncodedBuffer[512];

    int len = sizeof(buffer);

    if (datalink_serialize_frame_serial(message, buffer, &len))
    {
        int newLen = cobs_encode(buffer, len, cobsEncodedBuffer);
        cobsEncodedBuffer[newLen++] = 0x00;

        hal_serial_send_buffer(cobsEncodedBuffer, newLen);
    }
}

const datalink_frame_structure_serial_t *serial_get_current_message(void)
{
    return s_FrameSet ? &s_CurrentFrame : NULL;
}

static void _check_usb_connection(void)
{
    static bool usbConnected;

    if (!usbConnected && hal_board_is_usb_connected())
    {
        usbConnected = true;

        int c = 0;
        while (hal_serial_read_char(&c))
        {
            continue;
        }
    }
}