#pragma once

typedef enum consoleColor
{
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7,
    DEFAULT = 9,
} consoleColor_t;

void resetColorsAndEffects();
void setConsoleForegroundColor(consoleColor_t color);
void setConsoleBackgroundgroundColor(consoleColor_t color);
void setConsoleBold(int bold);
void setConsoleItalic(int italic);