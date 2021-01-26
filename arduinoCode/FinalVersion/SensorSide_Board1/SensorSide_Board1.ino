//program for Arduino of sensor side, board 1 (non-raspberry pi side)
//last modified 25 January 2019

#include "Arduino.h"

//stone constants

#define SIX_STONE_BOARD 6
#define TWO_STONE_BOARD 2
#define BOARD_COUNT 11

int [] board_sums = new int[]BOARD_COUNT];
int[][] board_vals = new int[BOARD_COUNT][SIX_STONEBOARD];
int[][] board_pins = new int[BOARD_COUNT][SIX_STONEBOARD];
board_pins[0] = {24,25,26,27,28,29};
board_pins[1] = {14,15,16,17,18,19};
board_pins[2] = {2,3,4,5,6,7};
board_pins[3] = {48,49,50,51,52,53};
board_pins[4] = {8,9,10,11,12,13};
board_pins[5] = {30,31,32,33,34,35};
board_pins[6] = {A2,A3,A4,A5,A6,A7};
board_pins[7] = {42,43,44,45,46,47};
board_pins[8] = {A10,A11,A12,A13,A14,A15};
board_pins[9] = {36,37,38,39,40,41};
board_pins[10] = {A8,A9};

//define all of these
static int phaseNo = 2
int phasePins[phaseNo] = {22,23} // {Assembly, Transport2}
int phaseArrowStates[5] = {LOW,LOW};
char msg_rec;

//boolean for Serial registration
boolean newData = false;

//defining the phase codings
int OTHER = 0;
int ASSEMBLY = 1;
int LOGISTICS = 2;
int TRANSPORT1 = 3;
int TRANSPORT2 = 4;
int DEMAND = 5;
int SCORE = 6;
int STONE_COUNT = 8;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < phaseNo; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  for(int i = 0; i < BOARD_COUNT-1; i++){
    for(int j = 0; j < SIX_STONE_BOARD; j++){
      pinMode(board_pins[i][j], INPUT);
    }
  pinMode(board[10][0],INPUT);
  pinMode(board[10][1],INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    msg_rec = Serial.read();
    newData = true;
  }
  else{};
  if(newData = true){
    // arrow if-else statements
    if (msg_rec == OTHER){
      for (int i = 0; i < phaseNo; i++){
        phaseArrowStates = {LOW,LOW};
      }
    }
    else if (msg_rec == ASSEMBLY){
      phaseArrowStates = {HIGH,LOW};
    }
    else if (msg_rec == TRANSPORT2){
       phaseArrowStates = {LOW,HIGH};
    }

    //score query statements
    if (msg_rec == SCORE){
      Serial.write(score);
    }

    //score query
    if (msg_rec == STONE_COUNT){
      StoneCount();
      for(int i = 0; i < BOARD_COUNT; i++){
        Serial.print(board_sums[i]);
        Serial.print(",");
      }
    }
    else(){
    }
    newData=false;
  }
  else{
  }
  writeArrowStates();
  delay(10);
}

void writeArrowStates(){
  for (int i = 0; i < phaseNo; i++){
    digitalWrite(phasePins[i],phaseArrowStates[i]);
  }
}

void StoneCount(){
  for(int i = 0; i < BOARD_COUNT-1; i++){
    board_sums[i] = 0;
    for(int j = 0; j < SIX_STONE_BOARD; j++){
      board_vals[i][j] = digitalRead(board_pins[i][j]);
      board_sums[i] = board_sums[i] + board_vals[i][j]; 
    }
  }
  
  board_sums[10] = 0;
  board_vals[10][0] = digitalRead(board_pins[10][0]);
  board_vals[10][1] = digitalRead(board_pins[10][1]);
  board_sums[10] = board_vals[10][0] + board_vals[10][1];
  }
}
}
