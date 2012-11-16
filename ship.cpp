#include "ship.h"

/*
 * Returns the default health for a given ship type.
 * @param	type		The ship's type.
 */
uint8_t getTypeHealth(Ship::TYPES type) {
  switch (type) {
  case Ship::PATROL:
    return PATROL_HEALTH;

  case Ship::SUBMARINE:
    return SUBMARINE_HEALTH;

  case Ship::DESTROYER:
    return DESTROYER_HEALTH;

  case Ship::BATTLESHIP:
    return BATTLESHIP_HEALTH;

  case Ship::CARRIER:
    return CARRIER_HEALTH;

  default:
    return 0;
  }
}

/*
 * Initializes a ship to default based on its type.
 * @param	ship		A pointer to the ship struct.
 * @param	type		The ship's type.
 */
void initShip(Ship* ship, Ship::TYPES type) {
  ship->x = -1;
  ship->y = -1;
  ship->direction = Ship::RIGHT;
  ship->type = type;
  ship->health = getTypeHealth(type);
}
