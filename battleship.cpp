#include "render.h"
#include "ship.h"
#include "map.h"
#include "placement.h"
#include "globals.h"

void setup() {
  Serial.begin(9600);
  
  initRender();

  startPlacement();
}

void loop() {
  updatePlacement();
}
