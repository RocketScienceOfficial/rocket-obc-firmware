#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include <stdio.h>

int main()
{
    boardInit(5000);

    printf("Hello, World!");

    while (1)
    {
        int a = 2, b = 3;

        printf("2 + 3 = %d\n", a + b);
        
        sleepMiliseconds(1000);
    }

    return 0;
}