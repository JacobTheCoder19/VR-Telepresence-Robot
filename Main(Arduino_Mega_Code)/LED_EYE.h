/* Program initialy created: 01/12/2026 3:40 pm
   Developer: Jacob Graham
   Last Modified: 01/12/2026.*/

#ifndef LED_EYE_H
#define LED_EYE_H

#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define LED_COUNT 8

extern Adafruit_NeoPixel strip;

void colorAll(int r, int g, int b);
void spin(uint8_t r, uint8_t g, uint8_t b, int delayTime);

#endif
