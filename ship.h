#ifndef SHIP_H
#define SHIP_H

#include "Arduino.h"

#define PATROL_HEALTH     2
#define SUBMARINE_HEALTH  3
#define DESTROYER_HEALTH  3
#define BATTLESHIP_HEALTH 4
#define CARRIER_HEALTH    5

typedef struct {
  int8_t x;
  int8_t y;
  uint8_t health;

  enum DIRECTIONS {
    RIGHT = 0,
    UP = 1,
    LEFT = 2,
    DOWN = 3
  } direction;

  enum TYPES {
    NONE = 0x00,
    PATROL = 0x08,
    SUBMARINE = 0x10,
    DESTROYER = 0x20,
    BATTLESHIP = 0x40,
    CARRIER = 0x80,
  } type;
} Ship;

uint8_t getTypeHealth(Ship::TYPES type);
String getTypeName(Ship::TYPES type);
void initShip(Ship* ship, Ship::TYPES type);

#endif
