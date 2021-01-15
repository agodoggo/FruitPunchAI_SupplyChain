#define TEN_STONE_BOARD 10
#define SIX_STONE_BOARD 6
#define TWO_STONE_BOARD 2
#define BOARD_COUNT 2
#define MAX_BOARD_NAME_LENGTH 

#include "Arduino.h"

int board1[TEN_STONE_BOARD] = {22,24,26,28,30,32,34,36,38,40};
int board2[TEN_STONE_BOARD] = {4,5,6,7,8,9,10,11,12,13};
int board3[TEN_STONE_BOARD] = {23,25,27,29,31,33,35,37,39,41};
int board4[SIX_STONE_BOARD] = {42,44,46,48,50,52};
int board5[SIX_STONE_BOARD] = {A0,A1,A2,A3,A4,A5};
int board6[TWO_STONE_BOARD] = {A6,A7};

int board1_val[TEN_STONE_BOARD];
int board2_val[TEN_STONE_BOARD];
int board3_val[TEN_STONE_BOARD];
int board4_val[SIX_STONE_BOARD];
int board5_val[SIX_STONE_BOARD];
int board6_val[TWO_STONE_BOARD];

void setup() 
{
  Serial.begin(9600);
  //setup Pins
  for(int i = 0; i < TEN_STONE_BOARD; i++){
    pinMode(board1[i], INPUT);
    pinMode(board2[i], INPUT);
    pinMode(board3[i], INPUT);
  }
  for(int i = 0; i < SIX_STONE_BOARD; i++){
    pinMode(board4[i], INPUT);
    pinMode(board5[i], INPUT);
  }
  pinMode(board6[0],INPUT);pinMode(board6[1],INPUT);
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  sensorRead();
}
void sensorRead(){
//  // Read Piezo ADC value in, and convert it to a voltage
  for(int i = 0; i < TEN_STONE_BOARD; i++){
    board1_val[i] = digitalRead(board1[i]);
    board2_val[i] = digitalRead(board2[i]);
    board3_val[i] = digitalRead(board3[i]);
  }
  for(int i = 0; i < SIX_STONE_BOARD; i++){
    board4_val[i] = digitalRead(board4[i]);
    board5_val[i] = digitalRead(board5[i]);
  }
  board6_val[0] = digitalRead(board6[0]);board6_val[1] = digitalRead(board6[1]);

  for(int i = 0; i < TEN_STONE_BOARD; i++){
    Serial.print(board1[i]);Serial.print(":");Serial.print(board1_val[i]);Serial.print(", ");
    Serial.print(board2[i]);Serial.print(":");Serial.print(board2_val[i]);Serial.print(", ");
    Serial.print(board3[i]);Serial.print(":");Serial.print(board3_val[i]);Serial.print(", ");
  }
  for(int i = 0; i < SIX_STONE_BOARD; i++){
    Serial.print(board4[i]);Serial.print(":");Serial.print(board4_val[i]);Serial.print(", ");
    Serial.print(board5[i]);Serial.print(":");Serial.print(board5_val[i]);Serial.print(", ");
  }
  Serial.print(board6[0]);Serial.print(":");Serial.print(board6_val[0]);Serial.print(", ");
  Serial.print(board6[1]);Serial.print(":");Serial.print(board6_val[1]);Serial.print(", ");
  Serial.println(); 
}
