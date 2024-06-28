#ifndef _EVENTS_H
#define _EVENTS_H

#include <stdbool.h>

/**
 * @brief Types of available messages
 */
typedef enum msg_type
{
    MSG_SENSORS_NORMAL_READ,
    MSG_SENSORS_BARO_READ,
    MSG_SENSORS_GPS_READ,
    MSG_SENSORS_ADC_READ,
    MSG_SM_APOGEE_REACHED,
    MSG_CMD_DATA_READ,
    MSG_CMD_DATA_CLEAR,
    MSG_CMD_DATA_RECOVERY,
    MSG_CMD_IGN_TEST,
    MSG_CMD_CONFIG_GET,
    MSG_CMD_CONFIG_SET,
    MSG_RADIO_PACKET_RECEIVED,
} msg_type_t;

/**
 * @brief Dispatch all events
 */
void events_dispatch(void);

/**
 * @brief Publihs new event
 *
 * @param msg Message to publish
 */
void events_publish(msg_type_t msg);

/**
 * @brief Check if event was fired
 *
 * @param msg Message to check
 * @return True if event was fired
 */
bool events_poll(msg_type_t msg);

#endif