#include "hal/board_control.h"
#include "hal/time_tracker.h"
#include "hal/multicore.h"
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

int main()
{
    hal_board_init(5000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Hardware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    cmd_init();
    sm_init();
    sensors_init();
    status_init();
    ign_init();
    dataman_init();
    radio_init();
    ahrs_init();

    while (true)
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
}