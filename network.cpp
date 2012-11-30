#include <Arduino.h>
#include <VirtualWire.h>
#include "network.h"
#include "globals.h"

// Outgoing packets
uint8_t outRespPacket[2];
uint8_t outPosPacket[2];

// Incoming packets
uint8_t inRespPacket[2];
uint8_t inPosPacket[2];

// Current packet sending info
DATATYPE currentSendType; //current datatype to be sent
uint8_t currentAckTurn = 0;


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

  while (1) {
    vw_wait_rx();

    bool checksum = vw_get_message(buf, &len);
    int8_t type = (DATATYPE) buf[0];

    if (checksum && type == POSDATA) break;
  }

  int8_t temp1 = (int8_t) buf[1];
  int8_t temp2 = (int8_t) buf[2];

  (*x) = temp1;
  (*y) = temp2;
  Serial.println("Position received:");//DEBUG
  Serial.print("x: ");//DEBUG
  Serial.println(*x);//DEBUG
  Serial.print("y: ");//DEBUG
  Serial.println(*y);//DEBUG
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

  while (1) {
    vw_wait_rx();

    bool checksum = vw_get_message(buf, &len);
    int8_t type = (DATATYPE) buf[0];

    if (checksum && type == RSPDATA) break;
  }

  (*hit) = (bool) buf[1];
  (*type) = (Ship::TYPES) buf[2];
  Serial.println("Response received:");//DEBUG
  Serial.print("Hit: ");//DEBUG
  Serial.println(*hit);//DEBUG
  Serial.print("Type: ");//DEBUG
  Serial.println(getTypeName(*type));//DEBUG
}

void determinePlayer() {
  //Check 3 times for a message.
  for (int i = 0; i < 3; i++) {
    if (vw_have_message()) {
      uint8_t len = 1;
      uint8_t buf[len];

      vw_get_message(buf, &len);
      char temp = (char) buf[0];

      //Data has already been sent; this must be player 2.
      if (temp == ENQ) {
	player = PLAYER_2;
	return;
      } else {
	Serial.print("Connection error! No ENQ found. ");//DEBUG
	Serial.print("Found ");//DEBUG
	Serial.print(temp, HEX);//DEBUG
	Serial.println(" instead.");//DEBUG
	Serial.print(Serial1.available());//DEBUG
	Serial.println(" more bytes available.");//DEBUG
      }
    }

    delay(250);
  }

  //No data was receieved, so we're player 1.
  player = PLAYER_1;

  uint8_t len = 1;
  uint8_t buf[len];
  buf[0] = ENQ;

  //Send the message three times.
  for (int i = 0; i < 3; i++) {
    vw_send(buf, len);
    vw_wait_tx();
    
    delay(250);
  }
}

void tick() {
  //--------------START TX---------------//
  uint8_t outLen;
  if(currentSendType == ACK) {
    outLen = 2;
  }
  else {
    outLen = 4;
  }

  uint8_t outBuf[outLen];
  outBuf[1] = currentSendType;

  if(currentSendType == ACK) {
    outBuf[0] = currentAckTurn;
  }
  else if(currentSendType == RSPDATA) {
    outBuf[0] = outRespPacket[0];
    outBuf[2] = outRespPacket[1];
    outBuf[3] = outRespPacket[2];
  }
  else if(currentSendType == POSDATA) {
    outBuf[0] = outPosPacket[0];
    outBuf[2] = outPosPacket[1];
    outBuf[3] = outPosPacket[2];
  }

  vw_send(outBuf, outLen);
  vw_wait_tx();
  //--------------END TX-----------------//
}
