#include "map.h"

/**
 * Initializes a map struct with default values.
 * Fills ships in order Patrol>Sub>Destroyer>Battleship>Carrier
 * with coordinates (-1, -1), the default ship health for each,
 * facing right, and each ship type.
 * @param	map	Pointer to the map to fill.
 * @param	initialState	The state to initialize each square to.
 * @returns		Fully initialized map.
 */
Map initMap(Map* curMap, uint8_t initialState) {
  //Patrol
  initShip(&curMap->ships[0], Ship::PATROL);

  //Submarine
  initShip(&curMap->ships[0], Ship::SUBMARINE);

  //Destroyer
  initShip(&curMap->ships[0], Ship::DESTROYER);

  //Battleship
  initShip(&curMap->ships[0], Ship::BATTLESHIP);

  //Carrier
  initShip(&curMap->ships[0], Ship::CARRIER);
  
  for(uint8_t i = 0; i < MAP_SIZE * MAP_SIZE; i++) {
    curMap->squares[i] = initialState;
  }
}

/**
 * Gets the current state from a square.
 * @param	block	The square to retrieve the state from.
 * @returns		The current state of the square.
 */

Map::STATE getState(uint8_t block) {
  return (Map::STATE)(block & 0x07);//0x07 = 00000111
}

/**
 * Gets the current Ship from a square.
 * @param	block	The square to retrieve the ship from.
 * @returns 		The current ship in the square or 0 if there's no ship.
 */
Ship::TYPES getShipType(uint8_t block) {
  return (Ship::TYPES)(block & 0xF8);//0xF8 = 11111000
}
