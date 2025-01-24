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
    MSG_SERIAL_MESSAGE_RECEIVED,
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
 * @return true if success
 */
bool events_publish(msg_type_t msg);

/**
 * @brief Check if event was fired
 *
 * @param msg Message to check
 * @return True if event was fired
 */
bool events_poll(msg_type_t msg);

#endif