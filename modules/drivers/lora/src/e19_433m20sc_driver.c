#include "modules/drivers/lora/e19_433m20sc_driver.h"

void e19_433m20sc_init(e19_433m20sc_config_t *config, hal_pin_number_t rxen, hal_pin_number_t txen)
{
    config->rxen = rxen;
    config->txen = txen;

    hal_gpio_init_pin(rxen, GPIO_OUTPUT);
    hal_gpio_init_pin(txen, GPIO_OUTPUT);

    e19_433m20sc_set_state(config, E19_433M20SC_STATE_OFF);
}

void e19_433m20sc_set_state(e19_433m20sc_config_t *config, e19_433m20sc_state_t state)
{
    switch (state)
    {
    case E19_433M20SC_STATE_RECEIVE:
        hal_gpio_set_pin_state(config->rxen, GPIO_HIGH);
        hal_gpio_set_pin_state(config->txen, GPIO_LOW);
        break;
    case E19_433M20SC_STATE_TRANSMIT:
        hal_gpio_set_pin_state(config->rxen, GPIO_LOW);
        hal_gpio_set_pin_state(config->txen, GPIO_HIGH);
        break;
    case E19_433M20SC_STATE_OFF:
        hal_gpio_set_pin_state(config->rxen, GPIO_LOW);
        hal_gpio_set_pin_state(config->txen, GPIO_LOW);
        break;
    default:
        return;
    }
}