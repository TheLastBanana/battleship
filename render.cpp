#include "render.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

/*
 * Initializes rendering.
 */
void initRender() {
  tft.initR(INITR_REDTAB);
}

/*
 * Renders the grid.
 * @param	startX	The left side of the grid (in grid coordinates).
 * @param	startY	The upper side of the grid (in grid coordinates).
 * @param	width	The width of the grid (in grid coordinates).
 * @param	height	The height of the grid (in grid coordinates).
 * @param	color	The color of the grid.
 */

void renderGrid(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, uint16_t color) {
  for (uint8_t x = startX; x < startX + width; x++) {
    tft.drawFastHLine(x * GRID_WIDTH,
		      y * GRID_HEIGHT,
		      width * GRID_WIDTH,
		      color);
  }
}
