#include "modules/drivers/lora/e19_433m20sc_driver.h"

void e19_433m20sc_init(e19_433m20sc_config_t *config, pin_number_t rxen, pin_number_t txen)
{
    config->rxen = rxen;
    config->txen = txen;

    gpio_init_pin(rxen, GPIO_OUTPUT);
    gpio_init_pin(txen, GPIO_OUTPUT);

    e19_433m20sc_set_state(config, E19_433M20SC_STATE_OFF);
}

void e19_433m20sc_set_state(e19_433m20sc_config_t *config, e19_433m20sc_state_t state)
{
    switch (state)
    {
    case E19_433M20SC_STATE_RECEIVE:
        gpio_set_pin_state(config->rxen, GPIO_HIGH);
        gpio_set_pin_state(config->txen, GPIO_LOW);
        break;
    case E19_433M20SC_STATE_TRANSMIT:
        gpio_set_pin_state(config->rxen, GPIO_LOW);
        gpio_set_pin_state(config->txen, GPIO_HIGH);
        break;
    case E19_433M20SC_STATE_OFF:
        gpio_set_pin_state(config->rxen, GPIO_LOW);
        gpio_set_pin_state(config->txen, GPIO_LOW);
        break;
    default:
        return;
    }
}