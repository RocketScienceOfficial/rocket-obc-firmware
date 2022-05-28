#include "console_colors.h"
#include <stdio.h>

void resetColorsAndEffects()
{
    //printf("\033[0m");
}

void setConsoleForegroundColor(consoleColor_t color)
{
    //printf("\033[%dm", ((int)color) + 30);
}

void setConsoleBackgroundgroundColor(consoleColor_t color)
{
    //printf("\033[%dm", ((int)color) + 40);
}

void setConsoleBold(int bold)
{
    //printf("\033[%dm", !bold ? 1 : 22);
}

void setConsoleItalic(int italic)
{
    //printf("\033[%dm", !italic ? 3 : 23);
}