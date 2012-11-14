#include "render.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

/*
 * Initializes rendering.
 */
void initRender() {
  tft.initR(INITR_REDTAB);
  tft.fillRect(0, 0, 128, 160, 0);
}

/*
 * Renders the grid.
 * @param	offsetX		The pixel x offset of the grid.
 * @param	offsetY		The pixel y offset of the grid.
 * @param	startX		The left side of the grid (in grid coordinates).
 * @param	startY		The upper side of the grid (in grid coordinates).
 * @param	width		The width of the grid (in grid coordinates).
 * @param	height		The height of the grid (in grid coordinates).
 * @param	color		The color of the grid.
 */
void renderGrid(uint8_t offsetX, uint8_t offsetY, uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, uint16_t color) {
  for (uint8_t x = startX; x <= startX + width; x++) {
    tft.drawFastVLine(offsetX + x * GRID_WIDTH,
		      offsetY + startY * GRID_HEIGHT,
		      width * GRID_WIDTH,
		      color);
  }

  for (uint8_t y = startY; y <= startY + height; y++) {
    tft.drawFastHLine(offsetX + startX * GRID_WIDTH,
		      offsetY + y * GRID_HEIGHT,
		      height * GRID_HEIGHT + 1,
		      color);
  }
}

/*
 * Renders a map.
 * @param	map	The map to render.
 */
void renderMap(Map *map) {
  renderGrid(4, 4, 0, 0, 10, 10, ST7735_WHITE);
}
