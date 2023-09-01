#include "state.h"
#include "config.h"
#include <Arduino.h>

static int s_CurrentStateIndex = 0;
static int s_LastStateIndex = 0;
static int s_ButtonState = LOW;
static bool s_ButtonPressed = false;
static bool s_Initialized = false;

void StateInit()
{
    pinMode(BUTTON_PIN, INPUT);
}

void StateCheck()
{
    s_ButtonState = digitalRead(BUTTON_PIN);

    s_LastStateIndex = s_CurrentStateIndex;

    if (s_ButtonState == HIGH)
    {
        if (!s_Initialized)
        {
            return;
        }

        if (!s_ButtonPressed)
        {
            s_CurrentStateIndex++;

            if (s_CurrentStateIndex >= SYSTEM_STATES_COUNT)
            {
                s_CurrentStateIndex = 0;
            }

            s_ButtonPressed = true;
        }
    }
    else
    {
        s_ButtonPressed = false;
        s_Initialized = true;
    }
}

bool StateChanged()
{
    return s_CurrentStateIndex != s_LastStateIndex;
}

SystemState StateGetCurrent()
{
    return (SystemState)s_CurrentStateIndex;
}