#include <Arduino.h>
#include "globals.h"
#include "wait.h"
#include "render.h"
#include "map.h"
#include "network.h"
#include "joystick.h"

//private functions
void renderShotMessage(int8_t x, int8_t y);

void renderShot(int8_t x, int8_t y) {
  uint8_t* block = &myMap.squares[indexFromPos(x, y)];
  Ship::TYPES shipType = getShipType(*block);
  if(shipType) {
    setState(block, Map::HIT);
    myMap.ships[getShipIndex(shipType)].health--;
  }
  else {
    setState(block, Map::MISS);
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
  getPosition(&x, &y);
  renderShot(x, y);

  while (!buttonAPressed()) {}

  return true;
}
