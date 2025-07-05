#include "radio.h"
#include "serial.h"
#include "sensors.h"
#include "voltage.h"
#include "ahrs.h"
#include "sm.h"
#include "ign.h"
#include "board_config.h"
#include "../middleware/events.h"
#include <cobs.h>
#include <hal/uart_driver.h>
#include <hal/time_tracker.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

#define PACKETS_WITHOUT_RESPONSE_COUNT 10
#define RADIO_TX_DONE_RECOVERY_TIME_MS 100
#define RADIO_RESPONSE_RECOVERY_TIME_MS 2000
#define RADIO_PACKET_SEND_DELAY_MS 50

static uint8_t s_RecvBuffer[512];
static size_t s_BufLen = 0;
static datalink_frame_structure_serial_t s_CurrentFrame;
static bool s_FrameSet;
static hal_msec_t s_RadioTXDoneRecoveryTimeOffset;
static hal_msec_t s_RadioResponseRecoveryTimeOffset;
static hal_msec_t s_PacketTimer;
static int s_PacketCounterForResponse;
static bool s_WaitingForResponse;

static void _handle_protocol(void);
static void _handle_uart_communication(void);
static void _process_new_frame(void);
static void _send_message(const datalink_frame_structure_serial_t *message);
static uint16_t _packet_get_alt(void);
static uint8_t _packet_get_control_flags(void);
static bool _packet_check_ign_cont(uint8_t ign);

void radio_init(void)
{
    hal_uart_init_all(OBC_UART, OBC_UART_RX, OBC_UART_TX, OBC_UART_FREQ);

    s_PacketTimer = hal_time_get_ms_since_boot();

    SERIAL_DEBUG_LOG("READY");
}

void radio_update(void)
{
    _handle_protocol();
    _handle_uart_communication();
}

const datalink_frame_structure_serial_t *radio_get_current_message(void)
{
    return s_FrameSet ? &s_CurrentFrame : NULL;
}

static void _handle_protocol(void)
{
    if (!s_WaitingForResponse && s_RadioTXDoneRecoveryTimeOffset != 0 && hal_time_get_ms_since_boot() - s_RadioTXDoneRecoveryTimeOffset >= RADIO_TX_DONE_RECOVERY_TIME_MS)
    {
        s_RadioTXDoneRecoveryTimeOffset = 0;
        s_PacketTimer = hal_time_get_ms_since_boot();

        SERIAL_DEBUG_LOG("Didn't receive ack from radio module!");
    }

    if (s_WaitingForResponse && s_RadioResponseRecoveryTimeOffset != 0 && hal_time_get_ms_since_boot() - s_RadioResponseRecoveryTimeOffset >= RADIO_RESPONSE_RECOVERY_TIME_MS)
    {
        s_RadioResponseRecoveryTimeOffset = 0;
        s_WaitingForResponse = false;
        s_PacketTimer = hal_time_get_ms_since_boot();

        SERIAL_DEBUG_LOG("Didn't receive response from GCS!");
    }

    if (s_PacketTimer != 0 && hal_time_get_ms_since_boot() - s_PacketTimer >= RADIO_PACKET_SEND_DELAY_MS)
    {
        datalink_frame_telemetry_data_obc_t payload = {
            .qw = ahrs_get_data()->orientation.w,
            .qx = ahrs_get_data()->orientation.x,
            .qy = ahrs_get_data()->orientation.y,
            .qz = ahrs_get_data()->orientation.z,
            .velocity = (uint16_t)(sqrtf(ahrs_get_data()->velocity.x * ahrs_get_data()->velocity.x + ahrs_get_data()->velocity.y * ahrs_get_data()->velocity.y + ahrs_get_data()->velocity.z * ahrs_get_data()->velocity.z) * 3.6f),
            .batteryVoltage100 = sensors_get_frame()->batVolts * 100,
            .batteryPercentage = sensors_get_frame()->batPercent,
            .lat = sensors_get_frame()->pos.lat,
            .lon = sensors_get_frame()->pos.lon,
            .alt = _packet_get_alt(),
            .state = (uint8_t)sm_get_state(),
            .controlFlags = _packet_get_control_flags(),
        };
        datalink_frame_structure_serial_t frame = {
            .msgId = s_PacketCounterForResponse == PACKETS_WITHOUT_RESPONSE_COUNT ? DATALINK_MESSAGE_TELEMETRY_DATA_OBC_WITH_RESPONSE : DATALINK_MESSAGE_TELEMETRY_DATA_OBC,
            .len = sizeof(payload),
        };
        memcpy(frame.payload, &payload, sizeof(payload));

        _send_message(&frame);

        s_PacketTimer = 0;
        s_RadioTXDoneRecoveryTimeOffset = hal_time_get_ms_since_boot();

        if (s_PacketCounterForResponse == PACKETS_WITHOUT_RESPONSE_COUNT)
        {
            s_WaitingForResponse = true;
            s_RadioResponseRecoveryTimeOffset = hal_time_get_ms_since_boot();
            s_PacketCounterForResponse = 0;
        }
        else
        {
            s_PacketCounterForResponse++;
        }
    }
}

static void _handle_uart_communication(void)
{
    static uint8_t cobsDecodedBuffer[512];

    while (hal_uart_is_readable(OBC_UART))
    {
        uint8_t byte;
        hal_uart_read(OBC_UART, &byte, 1);

        if (s_BufLen >= sizeof(s_RecvBuffer))
        {
            s_BufLen = 0;
        }

        s_RecvBuffer[s_BufLen++] = byte;

        if (byte == 0x00)
        {
            int newLen = cobs_decode(s_RecvBuffer, s_BufLen, cobsDecodedBuffer) - 1;

            s_FrameSet = (bool)datalink_deserialize_frame_serial(&s_CurrentFrame, cobsDecodedBuffer, newLen);
            s_BufLen = 0;

            if (s_FrameSet)
            {
                _process_new_frame();
            }
        }
    }
}

static void _process_new_frame(void)
{
    if (s_CurrentFrame.msgId == DATALINK_MESSAGE_RADIO_MODULE_TX_DONE)
    {
        s_RadioTXDoneRecoveryTimeOffset = 0;
        s_FrameSet = false;

        if (!s_WaitingForResponse)
        {
            s_PacketTimer = hal_time_get_ms_since_boot();
        }
    }
    else if (s_CurrentFrame.msgId == DATALINK_MESSAGE_TELEMETRY_RESPONSE)
    {
        s_RadioResponseRecoveryTimeOffset = 0;
        s_WaitingForResponse = false;
        s_PacketTimer = hal_time_get_ms_since_boot();

        events_publish(MSG_RADIO_PACKET_RECEIVED);
    }
}

static void _send_message(const datalink_frame_structure_serial_t *message)
{
    static uint8_t buffer[512];
    static uint8_t cobsEncodedBuffer[512];

    int len = sizeof(buffer);

    if (datalink_serialize_frame_serial(message, buffer, &len))
    {
        int newLen = cobs_encode(buffer, len, cobsEncodedBuffer);
        cobsEncodedBuffer[newLen++] = 0x00;

        hal_uart_write(OBC_UART, cobsEncodedBuffer, newLen);
    }
}

static uint16_t _packet_get_alt(void)
{
    float tmp = ahrs_get_data()->position.z;

    return tmp > 0 ? (uint16_t)tmp : 0;
}

static uint8_t _packet_get_control_flags(void)
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
    if (_packet_check_ign_cont(1))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_1;
    }
    if (_packet_check_ign_cont(2))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_2;
    }
    if (_packet_check_ign_cont(3))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_3;
    }
    if (_packet_check_ign_cont(4))
    {
        flags |= DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_4;
    }

    return flags;
}

static bool _packet_check_ign_cont(uint8_t ign)
{
    uint8_t flags = ign_get_cont_flags(ign);

    return (flags & IGN_CONT_FLAG_IGN_PRESENT) && (flags & IGN_CONT_FLAG_FUSE_WORKING);
}