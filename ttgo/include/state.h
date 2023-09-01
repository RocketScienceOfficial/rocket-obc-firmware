#pragma once

#define SYSTEM_STATES_COUNT 2

enum class SystemState
{
    DASHBOARD,
    NAVIGATION,
};

void StateInit();
void StateCheck();

bool StateChanged();
SystemState StateGetCurrent();