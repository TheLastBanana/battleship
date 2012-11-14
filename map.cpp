#include <map.h>
#include <ship.h>

map initMap(Map* map, uint8_t initialState) {
  map->ships[0] = {
    -1,
    -1, 
    PATROL_HEALTH,
    0,
    TYPES.PATROL
  };
  map->ships[0] = {
    -1,
    -1, 
    SUBMARINE_HEALTH,
    0,
    TYPES.SUBMARINE
  };
  map->ships[0] = {
    -1,
    -1, 
    DESTROYER_HEALTH,
    0,
    TYPES.DESTROYER
  };
  map->ships[0] = {
    -1,
    -1, 
    BATTLESHIP_HEALTH,
    0,
    TYPES.BATTLESHIP
  };
  map->ships[0] = {
    -1,
    -1, 
    CARRIER_HEALTH,
    0,
    TYPES.CARRIER
  };
  
  for(uint8_t i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
    map->squares[i] = initialState;
  }
}


uint8_t getState(uint8_t block) {
  return block & 0x07;//0x07 = 00000111
}

uint8_t getShipType(uint8_t block) {
  return block & 0xF8;//0xF8 = 11111000
}
