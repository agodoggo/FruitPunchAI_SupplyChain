#define SIX_STONE_BOARD 6
#define TWO_STONE_BOARD 2
#define BOARD_COUNT 11
#define MAX_BOARD_NAME_LENGTH 

#include "Arduino.h"

int board1[SIX_STONE_BOARD] = {24,25,26,27,28,29};
int board2[SIX_STONE_BOARD] = {14,15,16,17,18,19};
int board3[SIX_STONE_BOARD] = {2,3,4,5,6,7};
int board4[SIX_STONE_BOARD] = {48,49,50,51,52,53};
int board5[SIX_STONE_BOARD] = {8,9,10,11,12,13};
int board6[SIX_STONE_BOARD] = {30,31,32,33,34,35};
int board7[SIX_STONE_BOARD] = {A2,A3,A4,A5,A6,A7};
int board8[SIX_STONE_BOARD] = {42,43,44,45,46,47};
int board9[SIX_STONE_BOARD] = {A10,A11,A12,A13,A14,A15};
int board10[SIX_STONE_BOARD] = {36,37,38,39,40,41};
int board11[TWO_STONE_BOARD] = {A8,A9};

int board1_val[SIX_STONE_BOARD];
int board2_val[SIX_STONE_BOARD];
int board3_val[SIX_STONE_BOARD];
int board4_val[SIX_STONE_BOARD];
int board5_val[SIX_STONE_BOARD];
int board6_val[SIX_STONE_BOARD];
int board7_val[SIX_STONE_BOARD];
int board8_val[SIX_STONE_BOARD];
int board9_val[SIX_STONE_BOARD];
int board10_val[SIX_STONE_BOARD];
int board11_val[TWO_STONE_BOARD];

void setup() 
{
  Serial.begin(9600);
  //setup Pins
  for(int i = 0; i < SIX_STONE_BOARD; i++){
    pinMode(board1[i], INPUT);
    pinMode(board2[i], INPUT);
    pinMode(board3[i], INPUT);
    pinMode(board4[i], INPUT);
    pinMode(board5[i], INPUT);
    pinMode(board6[i], INPUT);
    pinMode(board7[i], INPUT);
    pinMode(board8[i], INPUT);
    pinMode(board9[i], INPUT);
    pinMode(board10[i], INPUT);
  }
  pinMode(board11[0],INPUT);pinMode(board11[1],INPUT);
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  sensorRead();
}
void sensorRead(){
//  // Read Piezo ADC value in, and convert it to a voltage
  for(int i = 0; i < SIX_STONE_BOARD; i++){
    board1_val[i] = digitalRead(board1[i]);
    board2_val[i] = digitalRead(board2[i]);
    board3_val[i] = digitalRead(board3[i]);
    board4_val[i] = digitalRead(board4[i]);
    board5_val[i] = digitalRead(board5[i]);
    board6_val[i] = digitalRead(board6[i]);
    board7_val[i] = digitalRead(board7[i]);
    board8_val[i] = digitalRead(board8[i]);
    board9_val[i] = digitalRead(board9[i]);
    board10_val[i] = digitalRead(board10[i]);
  }
  board11_val[0] = digitalRead(board11[0]);
  board11_val[1] = digitalRead(board11[1]);

  for(int i = 0; i < SIX_STONE_BOARD; i++){
    Serial.print(board1[i]);Serial.print(":");Serial.print(board1_val[i]);Serial.print(", ");
    Serial.print(board2[i]);Serial.print(":");Serial.print(board2_val[i]);Serial.print(", ");
    Serial.print(board3[i]);Serial.print(":");Serial.print(board3_val[i]);Serial.print(", ");
    Serial.print(board4[i]);Serial.print(":");Serial.print(board4_val[i]);Serial.print(", ");
    Serial.print(board5[i]);Serial.print(":");Serial.print(board5_val[i]);Serial.print(", ");
    Serial.print(board6[i]);Serial.print(":");Serial.print(board6_val[i]);Serial.print(", ");
    Serial.print(board7[i]);Serial.print(":");Serial.print(board7_val[i]);Serial.print(", ");
    Serial.print(board8[i]);Serial.print(":");Serial.print(board8_val[i]);Serial.print(", ");
    Serial.print(board9[i]);Serial.print(":");Serial.print(board9_val[i]);Serial.print(", ");
    Serial.print(board10[i]);Serial.print(":");Serial.print(board10_val[i]);Serial.print(", ");
  }
  Serial.print(board11[0]);Serial.print(":");Serial.print(board11_val[0]);Serial.print(", ");
  Serial.print(board11[1]);Serial.print(":");Serial.print(board11_val[1]);Serial.print(", ");
  Serial.println(); 
}
