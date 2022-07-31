#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdint-gcc.h>

void delay(uint32_t ms);
void delayMicroSeconds(uint32_t us);
uint64_t micros();
uint32_t millis();


#endif