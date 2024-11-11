#include "radio.h"
#include "serial.h"
#include "sensors.h"
#include "voltage.h"
#include "ahrs.h"
#include "sm.h"
#include "ign.h"
#include "../middleware/events.h"
#include "board_config.h"
#include "hal/uart_driver.h"
#include "hal/time_tracker.h"
#include <math.h>
#include <string.h>
#include <stddef.h>

static uint8_t s_RecvBuffer[512];
static size_t s_BufLen = 0;
static hal_msec_t s_TimeOffset;
static hal_msec_t s_RecoveryTimeOffset;
static datalink_frame_structure_serial_t s_CurrentFrame;
static bool s_FrameSet;

static void _radio_send_message(const datalink_frame_structure_serial_t *message);
static uint16_t _get_alt(void);
static bool _check_ign_cont(uint8_t ign);
static uint8_t _get_control_flags(void);
static datalink_frame_telemetry_data_obc_t _create_packet(void);

void radio_init(void)
{
    hal_uart_init_all(OBC_UART, OBC_UART_RX, OBC_UART_TX, OBC_UART_FREQ);

    SERIAL_DEBUG_PRINTF("READY");
}

void radio_update(void)
{
    if ((s_TimeOffset != 0 && hal_time_get_ms_since_boot() - s_TimeOffset >= 250) || (hal_time_get_ms_since_boot() - s_RecoveryTimeOffset >= 2000))
    {
        datalink_frame_telemetry_data_obc_t payload = {
            .qw = ahrs_get_data()->orientation.w,
            .qx = ahrs_get_data()->orientation.x,
            .qy = ahrs_get_data()->orientation.y,
            .qz = ahrs_get_data()->orientation.z,
            .velocity = sqrtf(ahrs_get_data()->velocity.x * ahrs_get_data()->velocity.x + ahrs_get_data()->velocity.y * ahrs_get_data()->velocity.y + ahrs_get_data()->velocity.z * ahrs_get_data()->velocity.z),
            .batteryVoltage10 = sensors_get_frame()->batVolts * 10,
            .batteryPercentage = sensors_get_frame()->batPercent,
            .lat = sensors_get_frame()->pos.lat,
            .lon = sensors_get_frame()->pos.lon,
            .alt = _get_alt(),
            .state = (uint8_t)sm_get_state(),
            .controlFlags = _get_control_flags(),
        };
        datalink_frame_structure_serial_t frame = {
            .msgId = DATALINK_MESSAGE_TELEMETRY_DATA_OBC,
            .len = sizeof(payload),
        };
        memcpy(frame.payload, &payload, sizeof(payload));

        _radio_send_message(&frame);

        s_TimeOffset = 0;
        s_RecoveryTimeOffset = hal_time_get_ms_since_boot();
    }

    while (hal_uart_is_readable(OBC_UART))
    {
        uint8_t byte;
        hal_uart_read(OBC_UART, &byte, 1);

        if (byte == 0)
        {
            s_FrameSet = (bool)datalink_deserialize_frame_serial(&s_CurrentFrame, s_RecvBuffer, s_BufLen);
            s_BufLen = 0;

            if (s_FrameSet)
            {
                s_TimeOffset = hal_time_get_ms_since_boot();

                events_publish(MSG_RADIO_PACKET_RECEIVED);

                SERIAL_DEBUG_PRINTF("Packet is valid!");
            }
        }
        else
        {
            s_RecvBuffer[s_BufLen++] = byte;
        }
    }
}

const datalink_frame_structure_serial_t *radio_get_current_message(void)
{
    return s_FrameSet ? &s_CurrentFrame : NULL;
}

static void _radio_send_message(const datalink_frame_structure_serial_t *message)
{
    uint8_t buffer[512];
    int len = sizeof(buffer);
    datalink_serialize_frame_serial(message, buffer, &len);

    hal_uart_write(OBC_UART, buffer, len);

    SERIAL_DEBUG_PRINTF("Sent %d bytes", len);
}

static uint16_t _get_alt(void)
{
    float tmp = ahrs_get_data()->position.z;

    return tmp > 0 ? (uint16_t)tmp : 0;
}

static bool _check_ign_cont(uint8_t ign)
{
    uint8_t flags = ign_get_cont_flags(ign);

    return (flags & IGN_CONT_FLAG_IGN_PRESENT) && (flags & IGN_CONT_FLAG_FUSE_WORKING);
}

static uint8_t _get_control_flags(void)
{
    uint8_t flags = 0;
    uint8_t voltageFlags = voltage_get_pins_flags();

    if (sm_is_armed())
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_ARM_ENABLED;
    }
    if (voltageFlags & VOLTAGE_PIN_STATE_3V3)
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_3V3_ENABLED;
    }
    if (voltageFlags & VOLTAGE_PIN_STATE_5V)
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_5V_ENABLED;
    }
    if (voltageFlags & VOLTAGE_PIN_STATE_VBAT)
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_VBAT_ENABLED;
    }
    if (_check_ign_cont(1))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_1;
    }
    if (_check_ign_cont(2))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_2;
    }
    if (_check_ign_cont(3))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_3;
    }
    if (_check_ign_cont(4))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_4;
    }

    return flags;
}