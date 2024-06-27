#include "voltage.h"
#include "radio.h"
#include "board_config.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/gpio_driver.h"
#include <stdbool.h>

#define SYSTEM_NAME "voltage"

static void _init_pin(hal_pin_number_t pin);
static void _set_pin_state(bool en, bool dis, hal_pin_number_t pin);

void voltage_init(void)
{
    _init_pin(PIN_3V3);
    _init_pin(PIN_5V);
    _init_pin(PIN_VBAT);

    SYS_LOG("READY!");
}

void voltage_update(void)
{
    if (events_poll(MSG_RADIO_PACKET_RECEIVED))
    {
        _set_pin_state(radio_get_parsed_data()->v3v3_enabled, radio_get_parsed_data()->v3v3_disabled, PIN_3V3);
        _set_pin_state(radio_get_parsed_data()->v5_enabled, radio_get_parsed_data()->v5_disabled, PIN_5V);
        _set_pin_state(radio_get_parsed_data()->vbat_enabled, radio_get_parsed_data()->vbat_disabled, PIN_VBAT);
    }
}

uint8_t voltage_get_flags(void)
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

    SYS_LOG("Pin %d inititalized!", pin);
}

static void _set_pin_state(bool en, bool dis, hal_pin_number_t pin)
{
    if (en)
    {
        hal_gpio_set_pin_state(pin, GPIO_HIGH);

        SYS_LOG("Setting pin %d high", pin);
    }
    else if (dis)
    {
        hal_gpio_set_pin_state(pin, GPIO_LOW);

        SYS_LOG("Setting pin %d low", pin);
    }
}