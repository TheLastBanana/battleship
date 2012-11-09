#ifndef SHIP_H
#define SHIP_H

#define PATROL_HEALTH     2
#define SUBMARINE_HEALTH  3
#define BATTLESHIP_HEALTH 4
#define DESTROYER_HEALTH  4
#define CARRIER_HEALTH    5

typedef struct {
  uint8_t x;
  uint8_t y;
  enum DIRECTIONS {
    RIGHT = 0,
    UP = 1,
    LEFT = 2,
    DOWN = 3
  } direction;
  enum TYPES {
    PATROL = 0x08,
    SUBMARINE = 0x10,
    BATTLESHIP = 0x20,
    DESTROYER = 0x40,
    CARRIER = 0x80,
  } type;
  uint8_t health;
} ship;

#endif SHIP_H
