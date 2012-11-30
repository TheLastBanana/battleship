#include <Arduino.h>
#include <VirtualWire.h>
#include <TimerThree.h>
#include "network.h"
#include "globals.h"

// Outgoing packets
// First byte is the turn, rest is data
uint8_t outRespPacket[3];
uint8_t outPosPacket[3];

// Incoming packets
uint8_t inRespPacket[2];
uint8_t inPosPacket[2];

// Current packet sending info
DATATYPE currentSendType; //current datatype to be sent
uint8_t currentAckTurn = 0; //sent with ACK package
bool acknowledged = false; //determine whether to conintue spamming

// Whether incoming packets have been receieved
bool newInResp = false;
bool newInPos = false;

//private functions
void tick();

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
  outPosPacket[0] = turn;
  outPosPacket[1] = x;
  outPosPacket[2] = y;
}

/**
 * Reads an (x,y) position from the opposing arduino.
 * @param	x	Pointer to where the x position should be stored.
 * @param	y	Pointer to where the y position should be stored.
 */
void getPosition(int8_t *x, int8_t *y) {
  while (!newInPos) {}
  *x = inPosPacket[0];
  *y = inPosPacket[1];
}

/**
 * Sends a response to the shot position that includes whether or not it was hit
 * as well as the type of shit that was hit in the even that it was sunk.
 * @param	hit	Boolean that is true if there was a hit and false otherwise.
 * @param	type	uint8 containing the type of ship hit if and only if the ship was sunk.
 */
void sendResponse(bool hit, uint8_t type) {
  outRespPacket[0] = turn;
  outRespPacket[1] = hit;
  outRespPacket[2] = type;
}

/**
 * Gets a response to a shot.
 * @param	hit	Pointer to a boolean that will be filled with the hit state.
 * @param	type	Pointer to a ship type, Ship::NONE if no ship sunk, something else if it was sunk.
 */
void getResponse(bool *hit, Ship::TYPES *type) {
  while (!newInResp) {}
  *hit = (bool) inRespPacket[0];
  *type = (Ship::TYPES) inRespPacket[1];
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
  
  Timer3.initialize();
  Timer3.attachInterrupt(&tick, 50000);
}

void tick() {
  //--------------START RX---------------//
  if(vw_have_message()) {
    uint8_t inLen = 4;
    uint8_t inBuf[inLen];

    vw_get_message(inBuf, &inLen);

    if(inBuf[1] == ACK) {
      acknowledged = true;
    }
    else if(inBuf[1] == RSPDATA && !newInResp) {
      inRespPacket[0] = inBuf[0];
      inRespPacket[1] = inBuf[2];
      inRespPacket[2] = inBuf[3];
      newInResp = true;
    }
    else if(inBuf[1] == POSDATA && !newInPos) {
      inPosPacket[0] = inBuf[0];
      inPosPacket[1] = inBuf[2];
      inPosPacket[2] = inBuf[3];
      newInPos = true;
    }
  }
  //---------------END RX----------------//

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
