#ifndef MAP_H
#define MAP_H

#include <ship.h>

#define MAP_SIZE  10
#define NUM_SHIPS 5

typedef struct {
  Ship ships[5];
  uint8_t squares[MAP_SIZE * MAP_SIZE];
} Map;

enum STATE {
  UNKOWN = 0,
  HIT = 1,
  MISS = 2,
  NONE = 4
};

//Function definitions
Map initMap(Map* map, uint8_t initialState);
uint8_t getState(uint8_t block);
uint8_t getShipType(uint8_t block);

#endif
