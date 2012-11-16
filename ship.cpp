#include "ship.h"

/*
 * Returns the default health for a given ship type.
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
