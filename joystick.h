/*
 * Routines for using a joystick.
 */

#ifndef _JOYSTICK_H
#define _JOYSTICK_H

#include <Arduino.h>

#define SEL_PIN   9  // Digital pin for the joystick's SEL
#define VERT_PIN  0  // Analog pin for the joystick's VERT
#define HORZ_PIN  1  // Analog pin for the joystick's HORZ

extern int32_t vert0;
extern int32_t horz0;

/*
 * Initializes the joystick.
 */
void joyInit();

/*
* Returns the joystick's analog data as an int16_t (-512 to 512).
* vert: true for vertical input, false for horizontal input
*/
int32_t joyRead(boolean vert);

/*
* Returns the joystick's analog data as a float (-1 to 1).
* vert: true for vertical input, false for horizontal input
*/
float joyReadF(boolean vert);

#endif
