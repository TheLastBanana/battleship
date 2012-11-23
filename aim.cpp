#include "aim.h"
#include "globals.h"
#include "render.h"
#include "joystick.h"

uint8_t aimX;
uint8_t aimY;

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
