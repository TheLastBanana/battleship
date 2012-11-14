#include "Arduino.h"
#include <map.h>
#include <ship.h>

/**
 * Initializes a map struct with default values.
 * Fills ships in order Patrol>Sub>Destroyer>Battleship>Carrier
 * with coordinates (-1, -1), the default ship health for each,
 * facing right, and each ship type.
 * @param map Pointer to the map to fill.
 * @param initialState The state to initialize each square to.
 */
Map initMap(Map* curMap, uint8_t initialState) {
  //Patrol
  curMap->ships[0].x = -1;
  curMap->ships[0].y = -1;
  curMap->ships[0].health = PATROL_HEALTH;
  curMap->ships[0].direction = Ship::RIGHT;
  curMap->ships[0].type = Ship::PATROL;

  //Submarine
  curMap->ships[1].x = -1;
  curMap->ships[1].y = -1;
  curMap->ships[1].health = SUBMARINE_HEALTH;
  curMap->ships[1].direction = Ship::RIGHT;
  curMap->ships[1].type = Ship::SUBMARINE;

  //Destroyer
  curMap->ships[2].x = -1;
  curMap->ships[2].y = -1;
  curMap->ships[2].health = DESTROYER_HEALTH;
  curMap->ships[2].direction = Ship::RIGHT;
  curMap->ships[2].type = Ship::DESTROYER;

  //Battleship
  curMap->ships[3].x = -1;
  curMap->ships[3].y = -1;
  curMap->ships[3].health = BATTLESHIP_HEALTH;
  curMap->ships[3].direction = Ship::RIGHT;
  curMap->ships[3].type = Ship::BATTLESHIP;

  //Carrier
  curMap->ships[4].x = -1;
  curMap->ships[4].y = -1;
  curMap->ships[4].health = CARRIER_HEALTH;
  curMap->ships[4].direction = Ship::RIGHT;
  curMap->ships[4].type = Ship::CARRIER;

  
  for(uint8_t i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
    curMap->squares[i] = initialState;
  }
}


uint8_t getState(uint8_t block) {
  return block & 0x07;//0x07 = 00000111
}

uint8_t getShipType(uint8_t block) {
  return block & 0xF8;//0xF8 = 11111000
}
