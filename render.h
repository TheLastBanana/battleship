#ifndef RENDER_H
#define RENDER_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)

#define GRID_WIDTH	12
#define GRID_HEIGHT	12
#define GRID_X_OFFSET	4
#define GRID_Y_OFFSET	4

extern Adafruit_ST7735 tft;

#include "map.h"
#include "globals.h"

void initRender();

uint8_t Color565r(uint16_t color);
uint8_t Color565g(uint16_t color);
uint8_t Color565b(uint16_t color);
uint16_t blendColor(uint16_t colorA, uint16_t colorB);
uint16_t getStateColor(Map::STATE s);

void renderShip(Ship *ship);
void renderGrid(uint8_t offsetX, uint8_t offsetY, uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, uint16_t color);
void renderMap(Map *map);
void renderCursor(uint8_t x, uint8_t y);

#endif
