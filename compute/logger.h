#pragma once

#define LOG_FORMAT "[%s %s]: %s\n"
#define MEASURE_FORMAT "%s;"

void my_log(const char *format, ...);
void my_log_error(const char *format, ...);
void my_log_notout(const char *format, ...);
void my_log_error_notout(const char *format, ...);

void my_log_measure_init();
void my_log_measure_start();
void my_log_measure(const char *format, ...);
void my_log_measure_end();