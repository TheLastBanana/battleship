#include "render.h"
#include "ship.h"
#include "map.h"

void setup() {
  initRender();

  Map testMap;
  initMap(&testMap, Map::UNKNOWN);
  renderMap(&testMap);
}

void loop() {

}
