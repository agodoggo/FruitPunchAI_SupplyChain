// Wire Slave Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI slave device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

//constants won't change
const int num_stones = 10; // adjust this for the number of stones
const int stones[] = {2,3,4,5,6,7,8,9,10,11}; // adjust this for the pins in use

//Variables will change
int stonesState[num_stones]; 

byte stoneCount = 0;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  sensorRead();
  Wire.write(stoneCount); // respond with message of 1 bytes
  stoneCount=0;
}

void sensorRead(){
  for (int j = 0; j < num_stones; j = j + 1){
    int reading = digitalRead(stones[j]);
    stonesState[j] = reading;
  }
  for (int k = 0; k < num_stones; k = k + 1){
    stoneCount = stoneCount + stonesState[k];
  }
}
