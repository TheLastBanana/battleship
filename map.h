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
uint8_t getState(uint8_t block);
uint8_t getShipType(uint8_t block);

#endif
