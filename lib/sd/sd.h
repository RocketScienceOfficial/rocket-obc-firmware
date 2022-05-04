#pragma once

#define SD_FILES_MAX_COUNT 4

void sdInit();
void sdAttachToLogger();
void sdInitFile(const char *file);
void sdBegin(const char *file);
void sdWrite(const char *msg, const char *file);
void sdEnd(const char *file);
void sdFlush(const char *file);
void sdTerminate();