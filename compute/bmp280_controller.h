#ifndef BMP_280_CONTROLLER_H
#define BMP_280_CONTROLLER_H

void bmp280_init();
void bmp280_read_values(int32_t* temperature, int32_t* pressure);

#endif