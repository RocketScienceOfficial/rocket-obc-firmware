#ifndef _SD_CARD_CONTROLLER_H
#define _SD_CARD_CONTROLLER_H

#define LOG_FILENAME "log.txt"
#define MEASURE_FILENAME "measure.csv"

void sd_init();
void sd_write();
void sd_terminate();

#endif