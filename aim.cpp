#include "aim.h"
#include "globals.h"
#include "render.h"
#include "joystick.h"
#include "network.h"
#include "ship.h"

int8_t aimX;
int8_t aimY;

//private functions
void renderAimInfo();

/*
 * Start the aim screen.
 */
void initAim() {
  aimX = 0;
  aimY = 0;
  renderMap(&enemyMap);
  renderCursor(aimX, aimY);
  renderAimInfo();
}

/**
 * Update the aim screen.
 * @returns	bool true if the player has fired, false otherwise
 */
bool updateAim() {
  float vert = joyReadF(true), horiz = joyReadF(false);
  bool updated = false;
  if(vert < -joyThreshold && aimY > 0) {
    aimY--;
    updated = true;
  }
  else if(vert > joyThreshold && aimY < MAP_SIZE - 1) {
    aimY++;
    updated = true;
  }
  
  if(horiz < -joyThreshold && aimX > 0) {
    aimX--;
    updated = true;
  }
  else if(horiz > joyThreshold && aimX < MAP_SIZE - 1) {
    aimX++;
    updated = true;
  }
  
  if (updated) {
    renderMap(&enemyMap);
    renderCursor(aimX, aimY);
  }

  if(buttonAPressed() && getState(enemyMap.squares[indexFromPos(aimX, aimY)]) == Map::UNKNOWN) {
    // Fire the shot!
    listenUntil(ENQ);
    sendPosition(aimX, aimY);
    
    // Get back a response
    bool hit = false;
    Ship::TYPES type = Ship::NONE;
    getResponse(&hit, &type);
    Serial.print("Hit: ");//DEBUG
    Serial.println(hit);//DEBUG
    Serial.print("Type: ");//DEBUG
    Serial.println(getTypeName(type));//DEBUG

    if (hit) {
      if (type != Ship::NONE) {
	String name = getTypeName(type);
	String message[] = {"You sunk the enemy's", name + "!"};
	renderMessage(message, 2);
      } else {
	String message[] = {"You hit!"};
	renderMessage(message, 1);
      }
    } else {
      String message[] = {"You missed!"};
      renderMessage(message, 1);
    }

    setState(&enemyMap.squares[indexFromPos(aimX, aimY)], hit ? Map::HIT : Map::MISS);
    renderMap(&enemyMap);

    while (!buttonAPressed()) {}

    return true;
  }
  return false;
}

/*
 * Render the placement text at the bottom of the screen.
 */
void renderAimInfo() {
  String message[] = {"Aim your shot!"};
  renderMessage(message, 1);
}
