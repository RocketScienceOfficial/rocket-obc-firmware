#pragma once

#include "my_event.h"

#define MEASURE_FORMAT "%s;%s;"
#define MEASURE_FILENAME "Measurements.csv"

void myLogMeasureInit();
void myLogMeasureAddCallback(eventCallback_t callback);
void myLogMeasureBegin();
void myLogMeasure(const char *format, ...);
void myLogMeasureEnd();