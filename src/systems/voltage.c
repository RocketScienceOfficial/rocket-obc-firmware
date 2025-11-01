#include "voltage.h"
#include "radio.h"
#include "serial.h"
#include "board_config.h"
#include "../middleware/events.h"
#include <hal/gpio_driver.h>
#include <stdbool.h>

static void _init_pin(hal_pin_number_t pin);
static void _set_pin_state(int enable, hal_pin_number_t pin);

void voltage_init(void)
{
    _init_pin(PIN_3V3);
    _init_pin(PIN_5V);
    _init_pin(PIN_VBAT);

    SERIAL_DEBUG_LOG("READY!");
}

void voltage_update(void)
{
    if (events_poll(MSG_RADIO_PACKET_RECEIVED))
    {
        const datalink_frame_structure_serial_t *msg = radio_get_current_message();

        if (msg && msg->msgId == DATALINK_MESSAGE_TELEMETRY_RESPONSE)
        {
            const datalink_frame_telemetry_response_t *frame = (const datalink_frame_telemetry_response_t *)msg->payload;

            _set_pin_state(frame->flags & DATALINK_FLAGS_TELEMETRY_RESPONSE_CONTROL_3V3_ENABLED, PIN_3V3);
            _set_pin_state(frame->flags & DATALINK_FLAGS_TELEMETRY_RESPONSE_CONTROL_5V_ENABLED, PIN_5V);
            _set_pin_state(frame->flags & DATALINK_FLAGS_TELEMETRY_RESPONSE_CONTROL_VBAT_ENABLED, PIN_VBAT);
        }
    }
}

uint8_t voltage_get_pins_flags(void)
{
    uint8_t flags = 0;

    if (hal_gpio_get_pin_state(PIN_3V3) == GPIO_HIGH)
    {
        flags |= VOLTAGE_PIN_STATE_3V3;
    }
    if (hal_gpio_get_pin_state(PIN_5V) == GPIO_HIGH)
    {
        flags |= VOLTAGE_PIN_STATE_5V;
    }
    if (hal_gpio_get_pin_state(PIN_VBAT) == GPIO_HIGH)
    {
        flags |= VOLTAGE_PIN_STATE_VBAT;
    }

    return flags;
}

static void _init_pin(hal_pin_number_t pin)
{
    hal_gpio_init_pin(pin, GPIO_OUTPUT);
    hal_gpio_set_pin_state(pin, GPIO_LOW);

    SERIAL_DEBUG_LOG("Pin %d inititalized!", pin);
}

static void _set_pin_state(int enable, hal_pin_number_t pin)
{
    if (hal_gpio_get_pin_state(pin) != (enable ? GPIO_HIGH : GPIO_LOW))
    {
        hal_gpio_set_pin_state(pin, enable ? GPIO_HIGH : GPIO_LOW);

        SERIAL_DEBUG_LOG("Setting pin %d to %s", pin, enable ? "high" : "low");
    }
}