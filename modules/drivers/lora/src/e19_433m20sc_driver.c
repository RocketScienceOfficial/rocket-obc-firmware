#include "drivers/lora/e19_433m20sc_driver.h"

FUNCRESULT e19_433m20sc_Init(E19_433M20SC_Config *config, PinNumber rxen, PinNumber txen)
{
    config->rxen = rxen;
    config->txen = txen;

    gpioInitPin(rxen, GPIO_OUTPUT);
    gpioInitPin(txen, GPIO_OUTPUT);

    e19_433m20sc_SetState(config, E19_433M20SC_STATE_OFF);

    return SUC_OK;
}

FUNCRESULT e19_433m20sc_SetState(E19_433M20SC_Config *config, E19_433M20SC_State state)
{
    switch (state)
    {
    case E19_433M20SC_STATE_RECEIVE:
        gpioSetPinState(config->rxen, GPIO_HIGH);
        gpioSetPinState(config->txen, GPIO_LOW);
        break;
    case E19_433M20SC_STATE_TRANSMIT:
        gpioSetPinState(config->rxen, GPIO_LOW);
        gpioSetPinState(config->txen, GPIO_HIGH);
        break;
    case E19_433M20SC_STATE_OFF:
        gpioSetPinState(config->rxen, GPIO_LOW);
        gpioSetPinState(config->txen, GPIO_LOW);
        break;
    default:
        return ERR_INVALIDARG;
    }

    return SUC_OK;
}