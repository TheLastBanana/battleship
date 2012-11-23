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

    tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
    if (hit) {
      tft.setCursor(43, 136);
      tft.print("You missed!");
    } else if (type != Ship::NONE) {
      tft.setCursor(4, 128);
      tft.print("You sunk the enemy's");

      String name = getTypeName(type);

      tft.setCursor(64 - (name.length() + 1) * 3, 136);
      tft.print(name);
      tft.print("!");
    } else {
      tft.setCursor(40, 136);
      tft.print("You hit!");
    }

    while (!buttonAPressed()) {}

    return true;
  }
  return false;
}

/*
 * Render the placement text at the bottom of the screen.
 */
void renderAimInfo() {
  tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
  tft.setCursor(22, 136);
  tft.print("Aim your shot!");
}
