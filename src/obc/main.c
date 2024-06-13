#include "hal/board_control.h"
#include "hal/serial_driver.h"
#include "middleware/events.h"
#include "systems/dataman.h"
#include "systems/radio.h"
#include "systems/sensors.h"
#include "systems/serial.h"
#include "systems/sm.h"

static void _init_systems(void)
{
    sensors_init();
    serial_init();
    sm_init();
    dataman_init();
    radio_init();

    hal_serial_printf("All systems initialized!\n");
}

static void _update_systems(void)
{
    serial_update();
    sensors_update();
    sm_update();
    dataman_update();
    radio_update();
}

int main()
{
    hal_board_init(5000);

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