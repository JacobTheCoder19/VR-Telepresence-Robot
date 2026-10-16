#ifndef PIEZO_BUZZER_H
#define PIEZO_BUZZER_H

#include <Arduino.h>

// Pin definition (must match wiring)
#define BUZZER_PIN 3

// Core functions
void initBuzzer();
void bootNoise();
void beep(int frequency, int duration);

#endif
