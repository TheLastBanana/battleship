#include "render.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

/**
 * Initializes rendering.
 */
void initRender() {
  tft.initR(INITR_REDTAB);
  tft.fillRect(0, 0, 128, 160, 0);
}

/**
 * Gets the red component of a 5-6-5 packed color.
 * @param	color		The color.
 */
uint8_t Color565r(uint16_t color) {
  return (color & 0xF800) >> 8;
}

/**
 * Gets the green component of a 5-6-5 packed color.
 * @param	color		The color.
 */
uint8_t Color565g(uint16_t color) {
  return (color & 0x7E0) >> 3;
}

/**
 * Gets the blue component of a 5-6-5 packed color.
 * @param	color		The color.
 */
uint8_t Color565b(uint16_t color) {
  return (color & 0x1F) << 3;
}

/**
 * Blends two 5-6-5 packed colors together (multiply).
 * @param	colorA		The first color.
 * @param	colorB		The second color.
 */

uint16_t blendColor(uint16_t colorA, uint16_t colorB) {
  float ra = Color565r(colorA) / 248.f;
  float ga = Color565g(colorA) / 252.f;
  float ba = Color565b(colorA) / 248.f;
  float rb = Color565r(colorB) / 248.f;
  float gb = Color565g(colorB) / 252.f;
  float bb = Color565b(colorB) / 248.f;

  uint8_t r = ra * rb * 255;
  uint8_t g = ga * gb * 255;
  uint8_t b = ba * bb * 255;

  return tft.Color565(r, g, b);
}

/**
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

/**
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

  uint16_t color = tft.Color565(128, 128, 128);
  if (!shipClear(&myMap, ship)) color = blendColor(color, ST7735_RED);

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
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) break;

    tft.fillCircle(GRID_X_OFFSET + GRID_WIDTH * x + GRID_WIDTH * 0.5,
		   GRID_Y_OFFSET + GRID_HEIGHT * y + GRID_HEIGHT * 0.5,
		   GRID_WIDTH * 0.5 - 1,
		   color);
    *i += offset;
    count++;
  }
}

/**
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

/**
 * Renders a map.
 * @param	map	The map to render.
 */
void renderMap(Map *map) {
  uint16_t color;
  uint8_t square;

  for (uint8_t x = 0; x < MAP_SIZE; x++) {
    for (uint8_t y = 0; y < MAP_SIZE; y++) {
      square = map->squares[indexFromPos(x, y)];
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

/**
 * Renders the cursor.
 * @param	x	The cursor's x position, in grid coordinates.
 * @param	y	The cursor's y position, in grid coordinates.
 */
 void renderCursor(uint8_t x, uint8_t y) {
   x = x * GRID_WIDTH + 4;
   y = y * GRID_WIDTH + 4;
   tft.drawFastVLine(x + GRID_WIDTH * 0.5, y + 1, GRID_HEIGHT - 1, ST7735_YELLOW);
   tft.drawFastHLine(x + 1, y + GRID_HEIGHT * 0.5, GRID_WIDTH - 1, ST7735_YELLOW);
   tft.drawRect(x, y, GRID_WIDTH + 1, GRID_HEIGHT + 1, ST7735_YELLOW);
 }

/**
 * Clears the text area, then renders a new message in its place.
 * Theoretically, passing in length = 0 will result in an empty screen.
 * @param	messages	Array of messages to render
 * @param	length		The number of lines to render, maximum four.
 */
void renderMessage(String messages[], uint8_t length) {
  if(length > 4) length = 4;

  tft.fillRect(0, 128, 128, 32, ST7735_BLACK);
  for(uint8_t i = 0; i < length; i++) {
    tft.setCursor(64 - (messages[i].length() / 2), 128 + (i *8));
    tft.print(messages[i]);
  }
}
