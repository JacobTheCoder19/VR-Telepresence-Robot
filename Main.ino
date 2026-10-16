/* Program initialy created: 01/12/2026 3:40 pm 
   Developer: Jacob Graham 
   Purpose: To create a cool miny robot to be my best friend. 
   Last Modified: 01/14/2026. 5:53 pm
   Last Modified(Again): 06/26/2026. 12:00 AM*/

/* File: main.cpp */

#include "LED_EYE.h"
#include "PIEZO_BUZZER.h"
#include "MOTOR_CONTROLS.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

float currentValue = 0;
float smoothedValue = 0;
float smoothingFactor = 0.1; // lower = slower, smoother motion

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  initHeadServo();
  setHeadServo(90);

  Serial.begin(115200);
  Serial1.begin(115200);

  spin(255, 0, 0, 150);
  spin(0, 255, 0, 150);
  spin(0, 0, 255, 150);
  bootNoise();
  colorAll(0, 0, 255);
  //lookLeftRight(3);
  lookForward();
}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    currentValue = data.toFloat();

    // Smooth servo motion
    smoothedValue = smoothedValue + (currentValue - smoothedValue) * smoothingFactor;

    // Map relative value to servo angle
    int servoAngle = map(smoothedValue, -70, 70, 0, 180);
    servoAngle = constrain(servoAngle, 0, 180);

    setHeadServo(servoAngle);

    Serial.print("Current: ");
    Serial.print(currentValue);
    Serial.print(" | Smoothed: ");
    Serial.println(smoothedValue);
  }
}
