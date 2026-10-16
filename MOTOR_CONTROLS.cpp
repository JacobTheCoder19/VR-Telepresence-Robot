// File: MOTOR_CONTROLS.cpp
#include <Arduino.h>
#include <Servo.h>
#include "MOTOR_CONTROLS.h"

static Servo headServo;

void initHeadServo() {
  headServo.attach(4); // Adjust pin if needed
}

void setHeadServo(int angle){
  headServo.write(angle);
}

// Optional manual movement
void lookLeft(){ setHeadServo(45); }
void lookRight(){ setHeadServo(135); }
void lookForward(){ setHeadServo(90); }

void lookLeftRight(float seconds) {
  // Optional sweep function
  const int STEP_COUNT = 200;
  const int MIN_ANGLE = 0;
  const int MAX_ANGLE = 180;
  const int DELAY_MS = 10;

  unsigned long start = millis();
  unsigned long duration = seconds * 1000;

  while (millis() - start < duration) {
    for (int i = 0; i <= STEP_COUNT; i++) {
      if (millis() - start >= duration) return;
      float t = (float)i / STEP_COUNT;
      float eased = (1 - cos(t * PI)) / 2;
      int angle = MIN_ANGLE + eased * (MAX_ANGLE - MIN_ANGLE);
      headServo.write(angle);
      delay(DELAY_MS);
    }

    for (int i = 0; i <= STEP_COUNT; i++) {
      if (millis() - start >= duration) return;
      float t = (float)i / STEP_COUNT;
      float eased = (1 - cos(t * PI)) / 2;
      int angle = MAX_ANGLE - eased * (MAX_ANGLE - MIN_ANGLE);
      headServo.write(angle);
      delay(DELAY_MS);
    }
  }
}
