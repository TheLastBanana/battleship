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
  currentShip = 0;
  myMap.ships[currentShip].x = 0;
  myMap.ships[currentShip].y = 0;
  renderMap(&myMap);
  renderPlacementInfo();
}

/**
 * Update the ship placement screen.
 * @returns	bool true if done placement, false otherwise
 */
bool updatePlacement() {
  float vert = joyReadF(true), horiz = joyReadF(false);
  bool updated = false;
  if(vert < -joyThreshold && myMap.ships[currentShip].y > 0) {
    myMap.ships[currentShip].y--;
    updated = true;
  }
  else if(vert > joyThreshold && myMap.ships[currentShip].y < MAP_SIZE) {
    myMap.ships[currentShip].y++;
    updated = true;
  }
  
  if(horiz < -joyThreshold && myMap.ships[currentShip].x > 0) {
    myMap.ships[currentShip].x--;
    updated = true;
  }
  else if(horiz > joyThreshold && myMap.ships[currentShip].x < MAP_SIZE) {
    myMap.ships[currentShip].x++;
    updated = true;
  }
  
  if(updated) {
    renderMap(&myMap);
  }
  
  /*if(buttonBPressed()) {
    Serial.print("pushed: "); Serial.println(myMap.ships[currentShip].direction, DEC);
    myMap.ships[currentShip].direction = (Ship::DIRECTIONS) ((myMap.ships[currentShip].direction) % 4);
    Serial.println(myMap.ships[currentShip].direction, DEC);
    }*/

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
    Serial.println(myMap.squares[indexFromPos(x, y)], BIN);
    setShipType(&myMap.squares[indexFromPos(x, y)], myMap.ships[currentShip].type);
    Serial.println(myMap.squares[indexFromPos(x, y)], BIN);
    *i += offset;
  }
    
  if(currentShip++ > NUM_SHIPS) {
    return true;
  }

  myMap.ships[currentShip].x = 0;
  myMap.ships[currentShip].y = 0;
  
  renderMap(&myMap);
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
