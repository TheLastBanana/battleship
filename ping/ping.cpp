#include <Arduino.h>
uint8_t i = 0;

void loop() {
  Serial1.write(i);
  if(Serial1.available()) {
    Serial.print("Received: ");
    Serial.println(Serial1.read(), HEX);
  }
  else {
    Serial.println(i, DEC);
  }
  i++;
  delay(10);
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(4800);
}
