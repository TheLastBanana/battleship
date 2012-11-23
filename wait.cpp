#include <Arduino.h>
#include "globals.h"
#include "wait.h"
#include "render.h"
#include "map.h"
#include "network.h"
#include "joystick.h"

//private functions
void renderShotMessage(int8_t x, int8_t y);

void renderShot(int8_t x, int8_t y, bool *hit, Ship::TYPES *type) {
  uint8_t* block = &myMap.squares[indexFromPos(x, y)];
  Ship::TYPES shipType = getShipType(*block);
  if(shipType) {
    setState(block, Map::HIT);
    myMap.ships[getShipIndex(shipType)].health--;
    *hit = true;
    *type = shipType;
  }
  else {
    setState(block, Map::MISS);
    *hit = false;
    *type = Ship::NONE;
  }
  renderShotMessage(x, y);
  renderMap(&myMap);
}

/**
 * Renders a message that tells a player where their opponent shot.
 * @param	x	X component of the shot, will be converted to a character A-J
 * @param	y	Y component of the shot
 */
void renderShotMessage(int8_t x, int8_t y) {
  char xChar = 0x41 + x; //0x41 = 'A'
  
  tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
  tft.setCursor(25, 128);
  tft.print("Enemy shot at");

  tft.setCursor(55, 136);
  tft.print(xChar);
  tft.print(y);
  tft.print("!");
}

/**
 * Start the wait screen.
 */
void initWait() {
  renderMap(&myMap);
  Serial1.write(ENQ);
}

/**
 * Update the wait screen.
 * @returns	bool true if the enemy has fired, false otherwise
 */
bool updateWait() {
  int8_t x, y;
  bool hit;
  Ship::TYPES type;
  getPosition(&x, &y);
  renderShot(x, y, &hit, &type);
  Serial.print("Hit: ");//DEBUG
  Serial.println(hit);//DEBUG
  Serial.print("Type: ");//DEBUG
  Serial.println(getTypeName(type));//DEBUG
  sendResponse(hit, type);

  while (!buttonAPressed()) {}

  return true;
}
