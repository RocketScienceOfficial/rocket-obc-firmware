#include "hal/board_control.h"
#include "hal/serial_driver.h"
#include "hal/spi_driver.h"
#include "hal/adc_driver.h"
#include "board_config.h"
#include "ahrs.h"
#include "cmd.h"
#include "dataman.h"
#include "ign.h"
#include "radio.h"
#include "sensors.h"
#include "sm.h"
#include "status.h"

static void _init_systems(void)
{
    cmd_init();
    sm_init();
    sensors_init();
    status_init();
    ign_init();
    dataman_init();
    radio_init();
    ahrs_init();

    hal_serial_printf("All systems initialized!\n");
}

static void _update_systems(void)
{
    cmd_update();
    sm_update();
    sensors_update();
    ahrs_update();
    ign_update();
    dataman_update();
    radio_update();
    status_update();
}

int main()
{
    hal_board_init(5000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Hardware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    _init_systems();

    while (true)
    {
        _update_systems();
    }
}