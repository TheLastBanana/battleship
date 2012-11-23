#ifndef AIM_H
#define AIM_H

#include <Arduino.h>

extern int8_t aimX;
extern int8_t aimY;

void initAim();
bool updateAim();

#endif
