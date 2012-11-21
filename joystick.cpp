#include "joystick.h"

int32_t vert0 = 512;
int32_t horz0 = 512;
const float joyThreshold = .2f;

void joyInit() {
  pinMode(SEL_A_PIN, INPUT);
  pinMode(SEL_B_PIN, INPUT);
  digitalWrite(SEL_A_PIN, HIGH);
  digitalWrite(SEL_B_PIN, HIGH);
  horz0 = analogRead(HORZ_PIN);
  vert0 = analogRead(VERT_PIN);
}

int32_t joyRead(boolean vert) {
  return constrain(analogRead(vert ? VERT_PIN : HORZ_PIN) - (vert ? vert0 : horz0), -512, 512);
}

float joyReadF(boolean vert) {
  return (float(joyRead(vert))) / 512.f;
}

bool buttonAPressed() {
  if(digitalRead(SEL_A_PIN) == LOW) {
    return true;
  }
  return false;
}

bool buttonBPressed() {
  if(digitalRead(SEL_B_PIN) == LOW) {
    return true;
  }
  return false;
}
