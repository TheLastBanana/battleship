#ifndef NETWORK_H
#define NETWORK_H

#define ENQ 0x05
#define EOT 0x04

enum DATATYPE {
  POSDATA = 0,
  RSPDATA = 1,
  ACK = 2
};

#define TXPIN	3
#define RXPIN	2
#define PTTPIN	13

#include "ship.h"

//function definitions
void send();
void receive();
void networkInit();
void sendPosition(int8_t x, int8_t y);
void getPosition(int8_t *x, int8_t *y);
void listenUntil(char c);
void sendResponse(bool hit, uint8_t type);
void getResponse(bool *hit, Ship::TYPES *type);
void determinePlayer();
#endif
