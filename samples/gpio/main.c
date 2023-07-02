#include "drivers/gpio/gpio_driver.h"
#include "drivers/tools/time_tracker.h"

#define PIN 29
#define DELAY 1000

int main()
{
    gpioInitPin(PIN, GPIO_OUTPUT);

    while (TRUE)
    {
        gpioSetPinState(PIN, GPIO_HIGH);
        sleepMiliseconds(DELAY);
        
        gpioSetPinState(PIN, GPIO_LOW);
        sleepMiliseconds(DELAY);
    }

    return 0;
}