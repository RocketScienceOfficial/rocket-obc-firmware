#include "drivers/lora/e19_433m20sc_driver.h"

FUNCRESULT e19_433m20sc_Init(E19_433M20SC_Config *config, PinNumber rxen, PinNumber txen)
{
    config->_rxen = rxen;
    config->_txen = txen;

    gpioInitPin(rxen, GPIO_OUTPUT);
    gpioInitPin(txen, GPIO_OUTPUT);

    return SUC_OK;
}

FUNCRESULT e19_433m20sc_SetState(E19_433M20SC_Config *config, E19_433M20SC_State state)
{
    switch (state)
    {
    case E19_433M20SC_STATE_RECEIVE:
        gpioSetPinState(config->_rxen, GPIO_HIGH);
        gpioSetPinState(config->_txen, GPIO_LOW);
        break;
    case E19_433M20SC_STATE_TRANSMIT:
        gpioSetPinState(config->_rxen, GPIO_LOW);
        gpioSetPinState(config->_txen, GPIO_HIGH);
        break;
    case E19_433M20SC_STATE_OFF:
        gpioSetPinState(config->_rxen, GPIO_LOW);
        gpioSetPinState(config->_txen, GPIO_LOW);
        break;
    default:
        return ERR_INVALIDARG;
    }

    return SUC_OK;
}