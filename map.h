#ifndef MAP_H
#define MAP_H

#include <ship.h>

#define MAP_SIZE  10
#define NUM_SHIPS 5

typedef struct {
  Ship ships[5];
  uint8_t squares[MAP_SIZE * MAP_SIZE];

  enum STATE {
    UNKNOWN = 0x00,
    HIT = 0x01,
    MISS = 0x02,
    NONE = 0x04
  };
} Map;

//Function definitions
Map initMap(Map* map, uint8_t initialState);
Map::STATE getState(uint8_t block);
void setState(uint8_t* block, Map::STATE state);
Ship::TYPES getShipType(uint8_t block);
void setShipType(uint8_t* block, Ship::TYPES type);
uint8_t indexFromPos(uint8_t x, uint8_t y);
void posFromIndex(uint8_t index, uint8_t *x, uint8_t *y);
bool shipClear(Map* map, Ship* ship);
uint8_t getShipIndex(Ship::TYPES type);

#endif
