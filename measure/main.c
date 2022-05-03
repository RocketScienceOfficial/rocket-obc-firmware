#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout_config.h"
#include "logger.h"
#include "measurements_logger.h"
#include "log_printer.h"
#include "sd.h"
#include "bmp280.h"
#include "mpu6050.h"

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    sdInit();
    sdAttachToLogger();

    attachPrinterToLog();
    attachPrinterToMeasureLog();

    while (1)
    {
        sdBegin();
        sdEnd();

        sleep_ms(500);
    }

    sdTerminate();

    return 0;
}