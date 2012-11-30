#include <Arduino.h>
#include "globals.h"
#include "wait.h"
#include "render.h"
#include "map.h"
#include "network.h"
#include "joystick.h"
#include "ship.h"

//private functions
void renderShotMessage(int8_t x, int8_t y, bool hit, Ship::TYPES type);

void renderShot(int8_t x, int8_t y, bool *hit, Ship::TYPES *type) {
  uint8_t* block = &myMap.squares[indexFromPos(x, y)];
  Ship::TYPES shipType = getShipType(*block);
  if (shipType) {
    setState(block, Map::HIT);
    uint8_t *health = &(myMap.ships[getShipIndex(shipType)].health);
    (*health)--;
    *hit = true;
    if (*health == 0) *type = shipType;
    else *type = Ship::NONE;
    renderShotMessage(x, y, *hit, *type);
  } else {
    setState(block, Map::MISS);
    *hit = false;
    *type = Ship::NONE;
    renderShotMessage(x, y, *hit, *type);
  }
  renderMap(&myMap);
  renderCursor(x,y);
}

/**
 * Renders a message that tells a player where their opponent shot.
 * @param	x	X component of the shot, will be converted to a character A-J
 * @param	y	Y component of the shot
 */
void renderShotMessage(int8_t x, int8_t y, bool hit, Ship::TYPES type) {
  uint8_t length;
  if(type != Ship::NONE)  length = 3;
  else length = 2;

  char xChar = 0x41 + x; //0x41 = 'A'

  String messages[length];
  messages[0] = "Enemy shot at " + (String) xChar  + (String) y;
  if(type != Ship::NONE) {    
    messages[1] = "and sunk your";
    messages[2] = getTypeName(type) + '!';
  }
  else {
    messages[1] = "and " + (hit ? (String) "hit!" : (String) "missed!");
  }

  renderMessage(messages, length);
}

/**
 * Start the wait screen.
 */
void initWait() {
  renderMap(&myMap);
  String messages[] = {"Wait for the", "enemy to fire!"};
  renderMessage(messages, 2);
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

  Serial.println();

  turn++;

  return true;
}
