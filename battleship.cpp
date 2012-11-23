#include "render.h"
#include "ship.h"
#include "map.h"
#include "placement.h"
#include "aim.h"
#include "globals.h"
#include "joystick.h"
#include "network.h"

void initState(STATE newState) {
  switch (newState) {
  case PLACEMENT:
    initPlacement();
    break;

  case AIM:
    initAim();
    break;

  default:
    break;
  }

  gameState = newState;
}

void setup() {
  Serial.begin(9600);

  initMap(&enemyMap, Map::UNKNOWN);

  joyInit();
  initRender();
  initState(gameState);
}

void loop() {
  switch (gameState) {
  case PLACEMENT:
    if (updatePlacement()) initState(player == PLAYER_1 ? AIM : WAIT);
    break;

  case AIM:
    if (updateAim()) initState(WAIT);
    break;

  default:
    break;
  }
}
