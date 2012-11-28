#include <Arduino.h>
#include "network.h"
#include "globals.h"

/**
 * Sends an (x,y) position to the opposing arduino over Serial1.
 * @param	x	The x position.
 * @param	y	The y position.
 */
void sendPosition(int8_t x, int8_t y) {
  Serial1.write(x);
  Serial1.write(y);
  Serial1.write(EOT);
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
  while(Serial1.available() < 3) {} //wait for three bytes to fill the buffer

  int8_t temp1 = (int8_t) Serial1.read();
  int8_t temp2 = (int8_t) Serial1.read();
  uint8_t eot = (uint8_t) Serial1.read();

  if(eot != EOT) {
    Serial.print("Error: getPosition() received third byte that was not EOT."); Serial.println(eot, HEX); //DEBUG
    Serial.println("--Returning offscreen position--"); //DEBUG
    (*x) = -1;
    (*y) = -1;
  }
  else { 
    (*x) = temp1;
    (*y) = temp2;
    Serial.println("Position received");//DEBUG
  }
}

/**
 * Listens to Serial1 for a specified char, returning only after that char is received.
 * Will hard loop with no chance of escape until the specified char is received.
 * @param	c	The char to listen for.
 */
void listenUntil(char c) {
  while( 1 ) {
    if(Serial1.available()) {
      char temp = (char) Serial1.read();
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
}

/**
 * Sends a response to the shot position that includes whether or not it was hit
 * as well as the type of shit that was hit in the even that it was sunk.
 * @param	hit	Boolean that is true if there was a hit and false otherwise.
 * @param	type	uint8 containing the type of ship hit if and only if the ship was sunk.
 */
void sendResponse(bool hit, uint8_t type) {
  Serial1.write(hit);
  Serial1.write(type);
  Serial1.write(EOT);
}

/**
 * Gets a response to a shot.
 * @param	hit	Pointer to a boolean that will be filled with the hit state.
 * @param	type	Pointer to a ship type, Ship::NONE if no ship sunk, something else if it was sunk.
 */
void getResponse(bool *hit, Ship::TYPES *type) {
  Serial.println("Beginning wait for response");//DEBUG
  while(Serial1.available() < 3) {}

  bool temp1 = (bool) Serial1.read();
  Ship::TYPES temp2 = (Ship::TYPES) Serial1.read();
  uint8_t eot = (uint8_t) Serial1.read();

  if(eot != EOT) {
    Serial.print("Error: getResponse() received third byte that was not EOT, "); Serial.println(eot, HEX); //DEBUG
    Serial.println("--Returning miss and no ship type--"); //DEBUG
    (*hit) = false;
    (*type) = Ship::NONE;
  }
  else {
    (*hit) = temp1;
    (*type) = temp2;
    Serial.println("Response received");//DEBUG
  } 
}

void determinePlayer() {
  if (Serial1.available()) {
    char temp = Serial1.read();//NEED TO READ CHAR OUT OF BUFFER, DON'T DELETE
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
    Serial1.write(ENQ);
  }
}
