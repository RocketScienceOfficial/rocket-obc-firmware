#include "hal/board_control.h"
#include "hal/serial_driver.h"
#include "middleware/events.h"
#include "systems/ahrs.h"
#include "systems/dataman.h"
#include "systems/ign.h"
#include "systems/radio.h"
#include "systems/sensors.h"
#include "systems/serial.h"
#include "systems/sm.h"
#include "systems/status.h"
#include "systems/voltage.h"

static void _init_systems(void)
{
    ahrs_init();
    dataman_init();
    ign_init();
    radio_init();
    sensors_init();
    serial_init();
    sm_init();
    status_init();
    voltage_init();

    SERIAL_DEBUG_LOG("All systems initialized!");
}

static void _update_systems(void)
{
    serial_update();
    radio_update();
    sensors_update();
    ahrs_update();
    sm_update();
    dataman_update();
    ign_update();
    status_update();
    voltage_update();
}

int main()
{
    hal_board_init(3000, false);

    SERIAL_DEBUG_LOG("Initialized board!");

    _init_systems();

    while (true)
    {
        events_dispatch();

        _update_systems();
    }
}