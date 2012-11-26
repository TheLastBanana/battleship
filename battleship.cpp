#include "render.h"
#include "ship.h"
#include "map.h"
#include "placement.h"
#include "aim.h"
#include "wait.h"
#include "globals.h"
#include "joystick.h"
#include "network.h"
#include "end.h"

void initState(STATE newState) {
  switch (newState) {
  case PLACEMENT:
    initPlacement();
    break;

  case AIM:
    initAim();
    break;

  case WAIT:
    initWait();
    break;

  case WIN:
    initEnd(true);
    break;

  case LOSE:
    initEnd(false);
    break;

  default:
    break;
  }

  gameState = newState;
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(4800);

  initMap(&enemyMap, Map::UNKNOWN);

  joyInit();
  initRender();
  initState(gameState);
}

void loop() {
  switch (gameState) {
  case PLACEMENT:
    if (updatePlacement()) {
      // Get which player we are
      if (Serial1.available()) {
	char temp = Serial1.read();
	if (temp == ENQ) {
	  player = PLAYER_2;
	  String messages[] = {"Wait for the", "enemy to fire!"};
	  renderMessage(messages, 2);
	} else {
	  Serial.println("Connection error! No ENQ found.");
	  return;
	}
      } else {
	player = PLAYER_1;
	Serial1.write(ENQ);
      }

      initState(player == PLAYER_1 ? AIM : WAIT);
    }
    break;

  case AIM:
    if (updateAim()) {
      if (allShipsSunk(&enemyMap)) initState(WIN);
      else initState(WAIT);
    }
    break;

  case WAIT:
    if (updateWait()) {
      if (allShipsSunk(&myMap)) initState(LOSE);
      else initState(AIM);
    }
    break;

  case WIN:
    if (updateEnd()) initState(TITLE);
    break;

  case LOSE:
    if (updateEnd()) initState(TITLE);
    break;

  default:
    break;
  }
}
