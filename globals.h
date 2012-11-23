#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "map.h"

enum STATE {
  TITLE = 0,
  PLACEMENT = 1,
  AIM = 2,
  WAIT = 3,
  WIN = 4,
  LOSE = 5
};

enum PLAYER {
  PLAYER_1 = 1,
  PLAYER_2 = 2
};

extern Map myMap;
extern Map enemyMap;
extern STATE gameState;
extern PLAYER player;

#endif
