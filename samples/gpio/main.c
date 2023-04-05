#include "drivers/gpio/gpio_driver.h"
#include "pico/stdlib.h"

#define PIN 29
#define DELAY 1000

int main()
{
    gpioInitPin(PIN, GPIO_OUTPUT);

    while (TRUE)
    {
        gpioSetPinState(PIN, GPIO_HIGH);
        sleep_ms(DELAY);
        
        gpioSetPinState(PIN, GPIO_LOW);
        sleep_ms(DELAY);
    }

    return 0;
}