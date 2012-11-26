#include "end.h"
#include "render.h"
#include "joystick.h"

/**
 * Initialize the end screen.
 * @param	win	Whether the player won.
 */
void initEnd(bool win) {
  tft.fillRect(0, 0, 128, 160, ST7735_BLACK);

  if (win) {
    tft.setCursor(40, 76);
    tft.print("YOU WIN!");
  } else {
    tft.setCursor(37, 76);
    tft.print("YOU LOSE!");
  }
}

/**
 * Update the end screen.
 */
bool updateEnd() {
  static long start = millis();
  static bool flipped = false;

  long time = millis() - start;
  if (!flipped && time > 1000) {
    tft.invertDisplay(true);
    flipped = true;
  } else if (flipped && time > 2000) {
    tft.invertDisplay(false);
    flipped = false;
    start = millis();
  }

  if (buttonAPressed()) return true;
  return false;
}
