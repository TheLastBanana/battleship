#ifndef WAIT_H
#define WATI_H

#include <Arduino.h>

void initWait();
bool updateWait();
void renderShot(int8_t x, int8_t y, bool *hit, Ship::TYPES *type);

#endif
