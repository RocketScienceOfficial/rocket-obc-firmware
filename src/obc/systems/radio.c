#include "radio.h"
#include "board_config.h"
#include "sensors.h"
#include "ahrs.h"
#include "sm.h"
#include "lib/crypto/crc.h"
#include "hal/uart_driver.h"
#include "hal/time_tracker.h"
#include <math.h>

static msec_t s_Timer;
static uint8_t s_Seq;

static radio_frame_t _create_packet(void);

void radio_init(void)
{
    hal_uart_init_all(OBC_UART, OBC_UART_RX, OBC_UART_TX, OBC_UART_FREQ);
}

void radio_update(void)
{
    if (hal_time_run_every_ms(50, &s_Timer))
    {
        radio_frame_t frame = _create_packet();
        uint8_t *buffer = (uint8_t *)&frame;
        const size_t len = sizeof(frame);

        hal_uart_write(OBC_UART, buffer, len);
    }
}

static radio_frame_t _create_packet(void)
{
    radio_frame_t frame = {
        .magic = RADIO_MAGIC,
        .roll = ahrs_get_data()->rotation.x,
        .pitch = ahrs_get_data()->rotation.y,
        .yaw = ahrs_get_data()->rotation.z,
        .velocity = sqrtf(ahrs_get_data()->velocity.x * ahrs_get_data()->velocity.x + ahrs_get_data()->velocity.y * ahrs_get_data()->velocity.y + ahrs_get_data()->velocity.z * ahrs_get_data()->velocity.z),
        .batteryVoltage10 = sensors_get_frame()->batVolts * 10,
        .batteryPercentage = sensors_get_frame()->batPercent,
        .pos = sensors_get_frame()->pos,
        .state = (uint8_t)sm_get_state(),
        .seq = s_Seq,
    };

    frame.crc = crc16_mcrf4xx_calculate((const uint8_t *)&frame, sizeof(frame) - 2);

    s_Seq = s_Seq == 255 ? 0 : s_Seq + 1;

    return frame;
}