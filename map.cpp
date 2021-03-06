#include "map.h"

/**
 * Initializes a map struct with default values.
 * Fills ships in order Patrol>Sub>Destroyer>Battleship>Carrier
 * with coordinates (-1, -1), the default ship health for each,
 * facing right, and each ship type.
 * @param	map 		Pointer to the map to fill.
 * @param 	initialState 	The state to initialize each square to.
 * @returns 			Fully initialized map.
 */
Map initMap(Map* curMap, uint8_t initialState) {
  //Patrol
  initShip(&curMap->ships[0], Ship::PATROL);

  //Submarine
  initShip(&curMap->ships[1], Ship::SUBMARINE);

  //Destroyer
  initShip(&curMap->ships[2], Ship::DESTROYER);

  //Battleship
  initShip(&curMap->ships[3], Ship::BATTLESHIP);

  //Carrier
  initShip(&curMap->ships[4], Ship::CARRIER);
  
  for(uint8_t i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
    curMap->squares[i] = initialState;
  }
}

/**
 * Gets the current state from a square.
 * @param 	block 	The square to retrieve the state from.
 * @returns 		The current state of the square.
 */

Map::STATE getState(uint8_t block) {
  return (Map::STATE)(block & 0x07);//0x07 = 00000111
}

/**
 * Sets the current state to a square.
 * @param 	block 	The square to set the state to.
 * @param	state	The state to set.
 * @returns 		The current state of the square.
 */

void setState(uint8_t* block, Map::STATE state) {
  *block = (*block & 0xF8) | state;
}

/**
 * Gets the current Ship from a square.
 * @param	block	The square to retrieve the ship from.
 * @returns 		The current ship in the square or 0 if there's no ship.
 */
Ship::TYPES getShipType(uint8_t block) {
  return (Ship::TYPES)(block & 0xF8);//0xF8 = 11111000
}

/**
 * Sets the current Ship to a square.
 * @param      block	The square to set the ship to.
 * @returns   		The current ship in the square or 0 if there's no ship.
 */
void setShipType(uint8_t* block, Ship::TYPES type) {
  *block = (*block & 0x07) | type;
}

/*
 * Gets the index in squares from the given position.
 * @param	x	The x-position.
 * @param	y	The y-position.
 */
uint8_t indexFromPos(uint8_t x, uint8_t y) {
  return x + y * MAP_SIZE;
}

/*
 * Gets the position from a given index.
 * @param	index	The index in question.
 * @param	x	A pointer to the x-position.
 * @param	y	A pointer to the y-position.
 */
void posFromIndex(uint8_t index, uint8_t *x, uint8_t *y) {
  *x = index % MAP_SIZE;
  *y = index / MAP_SIZE;
}

/**
 * Checks that a ship does not overlap with anything else (or the edge of the screen).
 * @param	map		The map.
 * @param	ship		The ship.
 * @return	Whether the area is clear of everything.
 */
bool shipClear(Map* map, Ship* ship) {
  // Get the start position and size of the ship.
  int8_t x = ship->x;
  int8_t y = ship->y;

  if (x == -1 || y == -1) return false;

  int8_t size = getTypeHealth(ship->type);
  
  int8_t offset = 0;
  int8_t* i;
  int8_t count = 0;

  switch (ship->direction) {
  case Ship::RIGHT:
    i = &x;
    offset = 1;
    break;

  case Ship::LEFT:
    i = &x;
    offset = -1;
    break;

  case Ship::DOWN:
    i = &y;
    offset = 1;
    break;

  case Ship::UP:
    i = &y;
    offset = -1;
    break;

  default:
    i = &x;
    offset = 0;
    break;
  }

  // Increment i by offset, then draw the ship at the resulting position.
  while (count < size) {
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) return false;

    Ship::TYPES type = getShipType(map->squares[indexFromPos(x, y)]);
    if (type != Ship::NONE && type != ship->type) return false;
    *i += offset;
    count++;
  }

  return true;
}

/**
 * Gives a ship's index in the map.ships array based on its type.
 * @param	type	Type of ship
 * @return		The index of the ship.
 */
uint8_t getShipIndex(Ship::TYPES type) {
  switch(type) {
  case Ship::PATROL:
    return 0;
    break;
  case Ship::SUBMARINE:
    return 1;
    break;
  case Ship::DESTROYER:
    return 2;
    break;
  case Ship::BATTLESHIP:
    return 3;
    break;
  case Ship::CARRIER:
    return 4;
    break;
  default:
    return 0;
    break;
  }
}

/**
 * Returns whether all of the ships on a given map are sunk.
 * @return	Whether all ships are sunk.
 */
bool allShipsSunk(Map* map) {
  for (uint8_t i = 0; i < NUM_SHIPS; i++) {
    if (map->ships[i].health > 0) return false;
  }

  return true;
}
