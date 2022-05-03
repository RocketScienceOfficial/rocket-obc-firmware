#pragma once

#define SD_CARD_TIMEOUT_MS 4000

typedef struct
{
    char *filename;
    char *text;
} sd_entry_t;

void sdInit();
void sdAttachToLogger();
void sdBegin();
void sdWrite();
void sdEnd();
void sdFlush();
void sdTerminate();