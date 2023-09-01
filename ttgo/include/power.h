#pragma once

struct PMUData
{
    bool isCharging;
    bool isDischarge;
    bool isVBusIn;
    bool isBatteryConnected;
    float batteryVoltage;
    float VBusVoltage;
    float systemVoltage;
    int batteryPercentage;
};

void PMUInit();
void PMURead();
PMUData PMUGetCurrentData();