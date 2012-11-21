#include "render.h"
#include "ship.h"
#include "map.h"
#include "placement.h"
#include "globals.h"
#include "joystick.h"

void setup() {
  Serial.begin(9600);

  joyInit();
  
  initRender();

  startPlacement();
}

void loop() {
  updatePlacement();
}
