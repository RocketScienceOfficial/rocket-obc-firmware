#include "gps.h"
#include "config.h"
#include <Arduino.h>

void MyGPS::Init()
{
    Serial1.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    if (!m_GNSS.begin(Serial1))
    {
        Serial.println("GPS failed to start!");

        while (1)
            ;
    }

    m_GNSS.setUART1Output(COM_TYPE_UBX);
    m_GNSS.saveConfiguration();

    Serial.println("GPS started!");
}

void MyGPS::Check()
{
    unsigned long now = millis();

    if (now - m_LastUpdate >= GPS_UPDATE_RATE_MS)
    {
        m_LastUpdate=now;

        m_Latitude = m_GNSS.getLatitude() / 1000000.0f;
        m_Longitude = m_GNSS.getLongitude() / 1000000.0f;
        m_Altitude = m_GNSS.getAltitude() / 1000.0f;
    }
}

float MyGPS::GetLatitude()
{
    return m_Latitude;
}

float MyGPS::GetLongitude()
{
    return m_Latitude;
}

float MyGPS::GetAltitude()
{
    return m_Altitude;
}