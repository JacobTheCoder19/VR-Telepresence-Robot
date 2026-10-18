/* Program initialy created: 01/12/2026 3:40 pm
   Developer: Jacob Graham
   Last Modified: 01/12/2026.*/

#include "LED_EYE.h"

void colorAll(int r, int g, int b) {
    for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
}

void spin(uint8_t r, uint8_t g, uint8_t b, int delayTime) {
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.clear();
        strip.setPixelColor(i, strip.Color(r, g, b));
        strip.show();
        delay(delayTime);
    }
}
