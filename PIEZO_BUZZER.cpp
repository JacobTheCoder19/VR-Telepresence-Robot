#include <Arduino.h>
#include "PIEZO_BUZZER.h"

void bootNoise() {
    tone(BUZZER_PIN, 500);
    delay(500);
    tone(BUZZER_PIN, 1000);
    delay(500);
    tone(BUZZER_PIN, 2000);
    delay(500);
    noTone(BUZZER_PIN);
}
