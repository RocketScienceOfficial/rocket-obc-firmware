#pragma once

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

class MyGPS
{
public:
    void Init();
    void Check();
    float GetLatitude();
    float GetLongitude();
    float GetAltitude();

private:
    SFE_UBLOX_GNSS m_GNSS;
    unsigned long m_LastUpdate;
    float m_Latitude;
    float m_Longitude;
    float m_Altitude;
};
