#pragma once

#define SYSTEM_STATES_COUNT 4

enum class SystemState
{
    DASHBOARD,
    LOCATION,
    NAVIGATION,
    RADIO,
};

void StateInit();
void StateCheck();

bool StateChanged();
SystemState StateGetCurrent();