#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP8266 Begin!!");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  Serial.println("One Loop End!!");
}
