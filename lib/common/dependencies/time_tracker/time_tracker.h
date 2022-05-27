#pragma once

unsigned int getMsSinceBoot();
int runEvery(unsigned int ms, unsigned int* timerOffset);
int runEvery_offset(unsigned int ms, unsigned int offset);