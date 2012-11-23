#include <Arduino.h>
#include "globals.h"
#include "wait.h"
#include "render.h"
#include "map.h"

//private functions
void renderShotMessage(uint8_t x, uint8_t y);

void renderShot(uint8_t x, uint8_t y) {
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
}

/**
 * Renders a message that tells a player where their opponent shot.
 * @param	x	X component of the shot, will be converted to a character A-J
 * @param	y	Y component of the shot
 */
void renderShotMessage(uint8_t x, uint8_t y) {
  char xChar = 0x41 + x; //0x41 = 'A'
  
  tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
  tft.setCursor(25, 128);
  tft.print("Enemy shot at");

  tft.setCursor(55, 136);
  tft.print(xChar);
  tft.print(y);
  tft.print("!");
}
