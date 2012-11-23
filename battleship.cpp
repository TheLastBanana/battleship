#include "render.h"
#include "ship.h"
#include "map.h"
#include "placement.h"
#include "globals.h"
#include "joystick.h"

bool placingDone = false;

void setup() {
  Serial.begin(9600);

  joyInit();
  
  initRender();

  startPlacement();
}

void loop() {
  if(!placingDone && updatePlacement()) {//If placing done is true, then updatePlacement will never evaluate.
    placingDone = true;
    return;
  }
}
