#ifndef FLASHSYNC_ATCOMMAND_H
#define FLASHSYNC_ATCOMMAND_H
#include "Arduino.h"
#include "stdio.h"
#include "Adafruit_NeoPixel.h"



void test(Adafruit_NeoPixel pixels);
void fullOpen(Adafruit_NeoPixel pixels, uint8_t R, uint8_t G, uint8_t B);
void fullOpen(Adafruit_NeoPixel pixels,uint32_t Color);
#endif //FLASHSYNC_ATCOMMAND_H
