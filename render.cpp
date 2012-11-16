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
 * Get the colour of a square based on its state.
 * @param	s	The state of the square.
 */
uint16_t getStateColor(Map::STATE s) {
  switch (s) {
  case Map::UNKNOWN:
    return tft.Color565(128, 128, 128);
    break;

  case Map::HIT:
    return ST7735_RED;
    break;

  case Map::MISS:
    return ST7735_WHITE;
    break;

  case Map::NONE:
    return ST7735_BLUE;
    break;

  default:
    return tft.Color565(255, 0, 255);
    break;
  }
}

/*
 * Renders a ship. Doesn't render it if x or y is -1.
 * @param	ship		The ship.
 */
void renderShip(Ship *ship) {
  // Get the start position and size of the ship.
  int8_t x = ship->x;
  int8_t y = ship->y;

  if (x == -1 || y == -1) return;

  int8_t size = getTypeHealth(ship->type);
  
  int8_t offset = 0;
  int8_t* i;
  int8_t count = 0;

  switch (ship->direction) {
  case Ship::RIGHT:
    i = &x;
    offset = 1;
    break;

  case Ship::LEFT:
    i = &x;
    offset = -1;
    break;

  case Ship::DOWN:
    i = &y;
    offset = 1;
    break;

  case Ship::UP:
    i = &y;
    offset = -1;
    break;

  default:
    i = &x;
    offset = 0;
    break;
  }

  // Increment i by offset, then draw the ship at the resulting position.
  while (count < size) {
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) continue;

    tft.fillCircle(GRID_X_OFFSET + GRID_WIDTH * x + GRID_WIDTH * 0.5,
		   GRID_Y_OFFSET + GRID_HEIGHT * y + GRID_HEIGHT * 0.5,
		   GRID_WIDTH * 0.5 - 1,
		   tft.Color565(64, 64, 64));
    *i += offset;
    count++;
  }
}

/*
 * Renders a grid.
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
  uint16_t color;
  uint8_t square;

  for (uint8_t x = 0; x < MAP_SIZE; x++) {
    for (uint8_t y = 0; y < MAP_SIZE; y++) {
      square = map->squares[y * MAP_SIZE + x];
      color = getStateColor(getState(square));
      tft.fillRect(GRID_X_OFFSET + x * GRID_WIDTH,
		   GRID_Y_OFFSET + y * GRID_HEIGHT,
		   GRID_WIDTH,
		   GRID_HEIGHT,
		   color);
    }
  }

  for (uint8_t i = 0; i < NUM_SHIPS; i++) {
    renderShip(&(map->ships[i]));
  }

  renderGrid(GRID_X_OFFSET, GRID_Y_OFFSET, 0, 0, 10, 10, ST7735_WHITE);
}
