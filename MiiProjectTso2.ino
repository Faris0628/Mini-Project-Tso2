#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* scriptURL = "https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec";
const int sensorPin = 13;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void loop() {
  long duration, distance;
  pinMode(sensorPin, OUTPUT);
  digitalWrite(sensorPin, LOW); delayMicroseconds(2);
  digitalWrite(sensorPin, HIGH); delayMicroseconds(10);
  digitalWrite(sensorPin, LOW);

  pinMode(sensorPin, INPUT);
  duration = pulseIn(sensorPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: "); Serial.println(distance);
  sendData(distance);
  delay(10000);
}

void sendData(long distance) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    http.addHeader("Content-Type", "application/json");
    String json = "{\"distance\":" + String(distance) + "}";
    http.POST(json);
    http.end();
  }
}