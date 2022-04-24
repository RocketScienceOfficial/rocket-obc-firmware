#include <stdio.h>

const uint PIN = 14;

void servoInit(uint pin);
void servoRotateAngle(uint pin, float angle);

/*
 * video: https://www.youtube.com/watch?v=Au-oc4hxj-c
 * f = 1 / T = 50Hz
 * T = 20ms = 20000ns
 * clkT = 8ns
 * clkDiv = 1
 * wrap = T / (clkT * clkDiv)
 * duty = t / T
 * set = wrap * duty = (T / (clkT * clkDiv)) * (t / T) = t / (clkT * clkDiv)
 */