// HEADSET ESP32 (transmitter)
// Reads head yaw from the MPU6050 and streams it to the robot over WiFi.
//
// FUNCTIONS:
//  - Recenter button: look straight ahead, press it, that direction = "forward"
//  - Status LED (onboard blue LED, GPIO 2):
//      slow blink (3s)   = just powered on, set the headset down flat
//      fast blink        = calibrating, DON'T TOUCH
//      short blip / sec  = ready, waiting for the robot to connect
//      solid on          = connected and streaming to the robot
//      quick flicker     = recentered
//  
//
// WIRING:
//  MPU6050: SDA -> 21, SCL -> 22, VCC -> 3V3, GND -> GND
//  Button:  one leg -> GPIO 18, other leg -> GND (no resistor needed)

#include <Wire.h>
#include <MPU6050_tockn.h>
#include <WiFi.h>
#include <Ticker.h>

MPU6050 mpu6050(Wire);

const char* ssid = "RobotControl";
const char* password = "YOUR_PASSWORD"; // 8+ characters, must match on both ESP32s

WiFiServer server(80);
WiFiClient robot;

#define LED_PIN     2    // Internal LED on board
#define BUTTON_PIN  18   // recenter button

const unsigned long SEND_INTERVAL_MS = 50;  // 20 updates per second

float centerOffset = 0;        // the angle that counts as "straight ahead"
unsigned long lastSend = 0;
unsigned long flashUntil = 0;  // recenter flicker timer

// Button debounce
bool lastButtonReading = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 30;

// Blinking during setup (runs in the background while calibration blocks)
Ticker blinker;
volatile bool ledState = false;
void toggleLed() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}

void recenter() {
  centerOffset = mpu6050.getAngleZ();
  flashUntil = millis() + 400;
  Serial.println(">>> Recentered! <<<");
}

void checkButton() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > DEBOUNCE_MS && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) {  // pressed (pulled to GND)
      recenter();
    }
  }
  lastButtonReading = reading;
}

void updateStatusLed(bool connected) {
  unsigned long now = millis();

  if (now < flashUntil) {                 // recenter feedback
    digitalWrite(LED_PIN, (now / 60) % 2);
  } else if (connected) {                 // streaming
    digitalWrite(LED_PIN, HIGH);
  } else {                                // ready, waiting for robot
    digitalWrite(LED_PIN, (now % 1000) < 100);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(21, 22);  // SDA, SCL
  mpu6050.begin();

  // 1. Slow blink: gives you 3 seconds to set the headset down flat
  Serial.println("Set the headset down flat...");
  blinker.attach_ms(500, toggleLed);
  delay(3000);
  blinker.detach();

  // 2. Fast blink: calibrating, keep it perfectly still
  Serial.println("Calibrating, don't touch!");
  blinker.attach_ms(100, toggleLed);
  mpu6050.calcGyroOffsets(true);
  blinker.detach();

  mpu6050.update();
  centerOffset = mpu6050.getAngleZ();

  // 3. Start the WiFi network for the robot
  WiFi.softAP(ssid, password);
  WiFi.setSleep(false);  // less lag
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  Serial.println("Ready! Put it on, look straight ahead, press the button.");
}

void loop() {
  mpu6050.update();

  // Robot (re)connected? Swap out any old dead connection.
  if (server.hasClient()) {
    if (robot && robot.connected()) {
      robot.stop();
      Serial.println("Replacing old robot connection");
    }
    robot = server.available();
    Serial.println("Robot connected");
  }

  checkButton();

  bool connected = robot && robot.connected();
  updateStatusLed(connected);

  if (connected && millis() - lastSend >= SEND_INTERVAL_MS) {
    lastSend = millis();
    float angle = mpu6050.getAngleZ() - centerOffset;
    robot.println(angle, 2);
    Serial.println(angle);
  }
}
