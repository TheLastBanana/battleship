#include "render.h"
#include "ship.h"
#include "map.h"

void setup() {
  Serial.begin(9600);
  
  initRender();

  Map testMap;
  initMap(&testMap, Map::NONE);
  renderMap(&testMap);
}

void loop() {

}
