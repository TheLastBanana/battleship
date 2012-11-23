#include <Arduino.h>
#include "placement.h"
#include "globals.h"
#include "render.h"
#include "ship.h"
#include "joystick.h"

uint8_t currentShip;

//private functions
bool nextShip();
void renderInfo();

/*
 * Start the ship placement screen.
 */
void initPlacement() {
  initMap(&myMap, Map::NONE);
  currentShip = 0;
  myMap.ships[currentShip].x = 0;
  myMap.ships[currentShip].y = 0;
  renderMap(&myMap);
  renderInfo();
}

/**
 * Update the ship placement screen.
 * @returns	bool true if done placement, false otherwise
 */
bool updatePlacement() {
  Ship *ship = &myMap.ships[currentShip];
  float vert = joyReadF(true), horiz = joyReadF(false);
  bool updated = false;
  if(vert < -joyThreshold && myMap.ships[currentShip].y > 0) {
    ship->y--;
    updated = true;
  }
  else if(vert > joyThreshold && myMap.ships[currentShip].y < MAP_SIZE - 1) {
    ship->y++;
    updated = true;
  }
  
  if(horiz < -joyThreshold && myMap.ships[currentShip].x > 0) {
    ship->x--;
    updated = true;
  }
  else if(horiz > joyThreshold && myMap.ships[currentShip].x < MAP_SIZE - 1) {
    ship->x++;
    updated = true;
  }
  
  if(updated) {
    renderMap(&myMap);
  }
  
  // Rotate the ship.
  if(buttonBPressed()) {
    ship->direction = Ship::DIRECTIONS(((int)ship->direction + 1) % 4);
    renderMap(&myMap);
  }

  if(buttonAPressed() && shipClear(&myMap, ship)) {
    return nextShip();
  }
  return false;
}

/**
 * Update which ship is currrently being placed
 * @returns	bool true if reached NUM_SHIPS, false otherwise
 */
bool nextShip() {
  // Get the start position and size of the ship.
  int8_t x = myMap.ships[currentShip].x;
  int8_t y = myMap.ships[currentShip].y;

  int8_t size = getTypeHealth(myMap.ships[currentShip].type);
  
  int8_t offset = 0;
  int8_t* i;

  switch (myMap.ships[currentShip].direction) {
  case Ship::RIGHT:
    i = &x;
    offset = 1;
    break;

  case Ship::LEFT:
    i = &x;
    offset = -1;
    break;

  case Ship::DOWN:
    i = &y;
    offset = 1;
    break;

  case Ship::UP:
    i = &y;
    offset = -1;
    break;

  default:
    i = &x;
    offset = 0;
    break;
  }
  for(uint8_t c = 0; c < size; c++) {
    setShipType(&myMap.squares[indexFromPos(x, y)], myMap.ships[currentShip].type);
    *i += offset;
  }
    
  if(currentShip >= NUM_SHIPS - 1) {
    return true;
  }
  else {
    currentShip++;
  }

  myMap.ships[currentShip].x = 0;
  myMap.ships[currentShip].y = 0;
  
  renderMap(&myMap);
  renderInfo();
  return false;
}

/*
 * Render the placement text at the bottom of the screen.
 */
void renderInfo() {
  tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
  tft.setCursor(34, 128);
  tft.print("Place your");

  String name = getTypeName(myMap.ships[currentShip].type);

  tft.setCursor(64 - (name.length() + 1) * 3, 136);
  tft.print(name);
  tft.print("!");
}
