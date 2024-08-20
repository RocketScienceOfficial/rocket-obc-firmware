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
    sensors_init();
    ahrs_init();
    serial_init();
    sm_init();
    dataman_init();
    radio_init();
    status_init();
    ign_init();
    voltage_init();

    hal_serial_printf("All systems initialized!\n");
}

static void _update_systems(void)
{
    serial_update();
    sensors_update();
    ahrs_update();
    sm_update();
    dataman_update();
    radio_update();
    ign_update();
    status_update();
    voltage_update();
}

int main()
{
    hal_board_init(3000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Firmware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    _init_systems();

    while (true)
    {
        events_dispatch();

        _update_systems();
    }
}