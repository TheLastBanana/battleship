#ifndef NETWORK_H
#define NETWORK_H

#define EOT 0x04

//function definitions
void sendPosition(int8_t x, int8_t y);
void getPosition(int8_t *x, int8_t *y);
void listenUntil(char c);
void sendResponse(bool hit, uint8_t type);

#endif
