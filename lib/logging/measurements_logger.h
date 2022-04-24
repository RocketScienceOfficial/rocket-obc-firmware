#pragma once

#define MEASURE_FORMAT "%s;"
#define MEASURE_FILENAME "Measurements.csv"

typedef void (*logMeasureCallback_t)(const char *text);

void myLogMeasureSetCallback(logMeasureCallback_t callback);
void myLogMeasure(const char *format, ...);
void myLogMeasureEnd();