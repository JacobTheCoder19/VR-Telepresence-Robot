#include <WiFi.h>

const char* ssid = "RobotControl";
const char* password = "YOUR_PASSWORD";

WiFiClient client;
IPAddress serverIP(192, 168, 4, 1); // AP IP
const int serverPort = 80;

// Serial1 pins for ESP32 to Mega
#define ESP32_TX 17
#define ESP32_RX 16

void setup() {
  Serial.begin(115200);           // debug
  Serial1.begin(115200, SERIAL_8N1, ESP32_RX, ESP32_TX); // send to Mega

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Failed to connect to server");
  }
}

void loop() {
  if (client.available()) {
    String data = client.readStringUntil('\n');
    Serial.println("Received data: " + data); 
    Serial1.println(data);                    // forward to Mega
  }
}
