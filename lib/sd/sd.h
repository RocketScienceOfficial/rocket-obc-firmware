#pragma once

#define SD_CARD_TIMEOUT_MS 4000

typedef struct
{
    char *filename;
    char *text;
} sd_entry_t;

void sdInit();
void sdWrite();
void sdFlush();
void sdTerminate();