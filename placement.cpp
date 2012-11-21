#include <Arduino.h>
#include "placement.h"
#include "globals.h"
#include "render.h"
#include "ship.h"
#include "joystick.h"

uint8_t currentShip;

//private functions
bool nextShip();

/*
 * Start the ship placement screen.
 */
void startPlacement() {
  initMap(&myMap, Map::NONE);
  renderMap(&myMap);
  currentShip = 0;
  myMap.ships[currentShip].x = 0;
  myMap.ships[currentShip].y = 0;

  renderPlacementInfo();
}

/**
 * Update the ship placement screen.
 * @returns	bool true if done placement, false otherwise
 */
bool updatePlacement() {
  float vert = joyReadF(true), horiz = joyReadF(false);
  if(vert < -joyThreshold) {
    myMap.ships[currentShip].y--;
  }
  else if(vert > joyThreshold) {
    myMap.ships[currentShip].y++;
  }
  
  if(horiz < -joyThreshold) {
    myMap.ships[currentShip].x--;
  }
  else if(vert > joyThreshold) {
    myMap.ships[currentShip].x++;
  }

  renderShip(&myMap.ships[currentShip]);
  
  if(buttonAPressed()) {
    return nextShip();
  }
  return false;
}

/**
 * Update which ship is currrently being placed
 * @returns	bool true if reached NUM_SHIPS, false otherwise
 */
bool nextShip() {
  if(currentShip++ > NUM_SHIPS) {
    return true;
  }
  
  myMap.ships[currentShip].x = 0;
  myMap.ships[currentShip].y = 0;
  
  renderPlacementInfo();
  return false;
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
