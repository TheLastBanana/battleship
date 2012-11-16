#include "placement.h"
#include "globals.h"
#include "render.h"
#include "ship.h"

uint8_t currentShip;
uint8_t posX;
uint8_t posY;

/*
 * Start the ship placement screen.
 */
void startPlacement() {
  initMap(&myMap, Map::NONE);
  renderMap(&myMap);
  currentShip = 0;

  renderPlacementInfo();
}


/*
 * Update the ship placement screen.
 */
void updatePlacement() {
  
}

/*
 * Render the placement text at the bottom of the screen.
 */
void renderPlacementInfo() {
  tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
  tft.setCursor(34, 128);
  tft.print("Place your");

  String name = getTypeName(myMap.ships[currentShip].type);

  tft.setCursor(64 - (name.length() + 1) * 3, 136);
  tft.print(name);
  tft.print("!");
}
