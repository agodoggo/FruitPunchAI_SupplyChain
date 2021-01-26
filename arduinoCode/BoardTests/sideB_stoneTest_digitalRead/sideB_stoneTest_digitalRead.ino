#define SIX_STONE_BOARD 6
#define TWO_STONE_BOARD 2
#define BOARD_COUNT 11
#define MAX_BOARD_NAME_LENGTH 

#include "Arduino.h"

int board1[10] = {22,24,26,28,30,32,34,36,38,40};

int board1_val[10];

void setup() 
{
  Serial.begin(9600);
  //setup Pins
  for(int i = 0; i < 10; i++){
    pinMode(board1[i], INPUT);
  }
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  sensorRead();
}
void sensorRead(){
//  // Read Piezo ADC value in, and convert it to a voltage
  for(int i = 0; i < 10; i++){
    board1_val[i] = digitalRead(board1[i]);
  }

  for(int i = 0; i < 10; i++){
    Serial.print(board1[i]);Serial.print(":");Serial.print(board1_val[i]);Serial.print(", ");
  }
  Serial.println(); 
}
