#include <Arduino.h>
//pin del LED
#define PIN 14
//set up
void setup() {

  Serial.begin(115200);
  pinMode(PIN, OUTPUT);
}
//loop
void loop() {

  digitalWrite(PIN, HIGH);
  Serial.println("ON");
  delay(500);
  digitalWrite(PIN, LOW);
  Serial.println("OFF");
  delay(500);
}