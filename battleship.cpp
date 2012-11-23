#include "render.h"
#include "ship.h"
#include "map.h"
#include "placement.h"
#include "globals.h"
#include "joystick.h"

void initState(STATE newState) {
  switch (newState) {
  case PLACEMENT:
    initPlacement();
    break;

  default:
    break;
  }

  gameState = newState;
}

void setup() {
  Serial.begin(9600);

  joyInit();
  
  initRender();

  initState(gameState);
}

void loop() {
  switch (gameState) {
  case PLACEMENT:
    if (updatePlacement()) {
      initState(player == PLAYER_1 ? AIM : WAIT);
    }
    break;

  default:
    break;
  }
}
