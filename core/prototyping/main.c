#include "modules/drivers/hal/board_control.h"
#include "modules/logger/logger.h"

int main()
{
    hal_board_init(7000);

    OBC_INFO("Initialized board!");
    OBC_INFO("Hardware version: 1.0");
    OBC_INFO("Author: Filip Gawlik");
}