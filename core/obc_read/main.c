#include <stdbool.h>

#include "modules/drivers/hal/board_control.h"
#include "modules/database/dataman.h"

int main()
{
    hal_board_init(10000);

    dataman_read();

    while (true)
    {
        hal_tight_loop();
    }
}