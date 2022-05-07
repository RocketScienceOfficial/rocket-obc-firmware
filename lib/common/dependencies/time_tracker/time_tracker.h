#pragma once

unsigned int getMsSinceBoot();
char* getTimestamp();
int runEvery(unsigned int ms);
int runEvery_offset(unsigned int ms, unsigned int offset);