#ifndef MOTOR_CONTROLS_H
#define MOTOR_CONTROLS_H

#include <Arduino.h>

// Initialize the head servo
void initHeadServo();

// Set the head servo angle
void setHeadServo(int angle);

// Convenience movement functions
void lookLeft();
void lookRight();
void lookForward();

// Sweep the head left and right
void lookLeftRight(float seconds);

#endif
