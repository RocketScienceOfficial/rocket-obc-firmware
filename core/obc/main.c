#include "hal/board_control.h"
#include "hal/time_tracker.h"
#include "hal/multicore.h"
#include "hal/serial_driver.h"
#include "hal/spi_driver.h"
#include "hal/adc_driver.h"
#include "board_config.h"
#include "sensors.h"
#include "sm.h"
#include "status.h"
#include "ign.h"
#include "dataman.h"

int main()
{
    hal_board_init(5000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Hardware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(OBC_SPI, OBC_SPI_MISO_PIN, OBC_SPI_MOSI_PIN, OBC_SPI_SCK_PIN, OBC_SPI_FREQ);
    hal_adc_init_all();

    sensors_init();
    sm_init();
    status_init();
    ign_init();
    dataman_init();

    while (true)
    {
        sensors_update();
        sm_update();
        status_update();
        ign_update();
        dataman_update();
    }
}