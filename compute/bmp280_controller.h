#ifndef BMP_280_CONTROLLER_H
#define BMP_280_CONTROLLER_H

void bmp_init();
void bmp_getParams(int32_t* temperature, int32_t* pressure);

#endif