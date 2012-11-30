#include <Arduino.h>
#include <VirtualWire.h>
#include "network.h"
#include "globals.h"

/**
 * Initializes the network code.
 */
void networkInit() {
  vw_set_tx_pin(TXPIN);
  vw_set_rx_pin(RXPIN);
  vw_set_ptt_pin(PTTPIN);
  vw_setup(2000);
  vw_rx_start();
}

/**
 * Sends an (x,y) position to the opposing arduino over Serial1.
 * @param	x	The x position.
 * @param	y	The y position.
 */
void sendPosition(int8_t x, int8_t y) {
  for (int i = 0; i < 3; i++) {
    uint8_t len = 3;
    uint8_t buf[len];
    buf[0] = POSDATA;
    buf[1] = x;
    buf[2] = y;

    vw_send(buf, len);
    vw_wait_tx();

    delay(250);
  }
}

/**
 * Reads an (x,y) position from the opposing arduino over Serial1.
 * Will wait until three bytes are waiting in the buffer before proceeding with the read.
 * If the third byte read is not EOT then the position will default to offscreen.
 * @param	x	Pointer to where the x position should be stored.
 * @param	y	Pointer to where the y position should be stored.
 */
void getPosition(int8_t *x, int8_t *y) {
  Serial.println("Beginning wait for position");//DEBUG
  
  uint8_t len = 3;
  uint8_t buf[len];

  for (int i = 0; i < 3; i++) {
    vw_wait_rx();

    bool checksum = vw_get_message(buf, &len);
    int8_t type = (DATATYPE) buf[0];

    if (checksum && type == POSDATA) break;

    if (i == 2) {
      Serial.println("Failed to recieve position!");
      return;
    }
  }

  int8_t temp1 = (int8_t) buf[1];
  int8_t temp2 = (int8_t) buf[2];
  (*x) = temp1;
  (*y) = temp2;
  Serial.println("Position received");//DEBUG
}

/**
 * Listens for a specified char, returning only after that char is received.
 * Will hard loop with no chance of escape until the specified char is received.
 * @param	c	The char to listen for.
 */
void listenUntil(char c) {
  while (1) {
    uint8_t len = 1;
    uint8_t buf[len];

    vw_wait_rx();
    vw_get_message(buf, &len);
    char temp = (char) buf[0];
    if(c == temp) {
      Serial.print("Listened until and found: "); Serial.println(c, HEX);//DEBUG
      return;
    }
    else {//DEBUG
      Serial.print("Listening for char(hex): "); Serial.println(c, HEX);//DEBUG
      Serial.print("Read char(hex): "); Serial.println(c, HEX);//DEBUG
    }//DEBUG
  }
}

/**
 * Sends a response to the shot position that includes whether or not it was hit
 * as well as the type of shit that was hit in the even that it was sunk.
 * @param	hit	Boolean that is true if there was a hit and false otherwise.
 * @param	type	uint8 containing the type of ship hit if and only if the ship was sunk.
 */
void sendResponse(bool hit, uint8_t type) {
  for (int i = 0; i < 3; i++) {
    uint8_t len = 3;
    uint8_t buf[len];
    buf[0] = RSPDATA;
    buf[1] = hit;
    buf[2] = type;

    vw_send(buf, len);
    vw_wait_tx();

    delay(250);
  }
}

/**
 * Gets a response to a shot.
 * @param	hit	Pointer to a boolean that will be filled with the hit state.
 * @param	type	Pointer to a ship type, Ship::NONE if no ship sunk, something else if it was sunk.
 */
void getResponse(bool *hit, Ship::TYPES *type) {
  Serial.println("Beginning wait for position");//DEBUG
  
  uint8_t len = 3;
  uint8_t buf[len];

  for (int i = 0; i < 3; i++) {
    vw_wait_rx();

    bool checksum = vw_get_message(buf, &len);
    int8_t type = (DATATYPE) buf[0];

    if (checksum && type == POSDATA) break;

    if (i == 2) {
      Serial.println("Failed to recieve position!");
      return;
    }
  }

  (*hit) = (bool) buf[1];
  (*type) = (Ship::TYPES) buf[2];
  Serial.println("Response received");//DEBUG
}

void determinePlayer() {
  if (vw_have_message()) {
    uint8_t len = 1;
    uint8_t buf[len];

    vw_get_message(buf, &len);
    char temp = (char) buf[0];

    if (temp == ENQ) {
      player = PLAYER_2;
    } else {
      Serial.print("Connection error! No ENQ found. ");//DEBUG
      Serial.print("Found ");//DEBUG
      Serial.print(temp, HEX);//DEBUG
      Serial.println(" instead.");//DEBUG
      Serial.print(Serial1.available());//DEBUG
      Serial.println(" more bytes available.");//DEBUG
      return;
    }
  } else {
    player = PLAYER_1;

    uint8_t len = 1;
    uint8_t buf[len];
    buf[0] = ENQ;

    vw_send(buf, len);
    vw_wait_tx();
  }
}
