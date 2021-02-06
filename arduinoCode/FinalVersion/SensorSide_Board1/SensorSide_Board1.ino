//program for Arduino of sensor side, board 1 (non-raspberry pi side)
//last modified 3 February 2021

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

//stone constants
const int SIX_STONE_BOARD =  6;
const int TWO_STONE_BOARD = 2;
const int BOARD_COUNT = 11;

int board_sums[BOARD_COUNT];
int board_vals[BOARD_COUNT][SIX_STONE_BOARD];
int board_pins[BOARD_COUNT][SIX_STONE_BOARD] = {
  {24,25,26,27,28,29},
  {14,15,16,17,18,19},
  {2,3,4,5,6,7},
  {48,49,50,51,52,53},
  {8,9,10,11,12,13},
  {30,31,32,33,34,35},
  {A2,A3,A4,A5,A6,A7},
  {42,43,44,45,46,47},
  {A10,A11,A12,A13,A14,A15},
  {36,37,38,39,40,41},
  {A8,A9}
};

//define all of these
const int phaseNo = 2;
int phasePins[phaseNo] = {22,23}; // {Assembly, Transport2}
int phaseArrowStates[phaseNo] = {LOW,LOW};
const byte numChars = 128;
char receivedChars[numChars];
char tempChars[numChars];

//boolean for Serial registration
boolean newData = false;

//defining the phase codings
int NONE = 0;
int ASSEMBLY = 1;
int LOGISTICS = 2;
int TRANSPORT1 = 3;
int TRANSPORT2 = 4;
int DEMAND = 5;

//deciphered score
int arrow_phase = -1;
int stone_query = -1;

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
  }
  pinMode(board_pins[10][0],INPUT);
  pinMode(board_pins[10][1],INPUT);
}

void loop() {
  // put your main code here, to run repeatedly
    // arrow if-else statements
  recvWithStartEndMarkers();
  changeHardwareState();
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}
void parseData(){
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    arrow_phase = atoi(strtokIndx); // copy it to messageFromPC

    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    stone_query = atoi(strtokIndx);     // convert this part to an integer
}

void changeHardwareState(){
  if(newData == true){
    //get information from packet and change boolean
    strcpy(tempChars, receivedChars);
    parseData();
    newData = false;

    //arrow if-else statements
    phaseArrowStates[0] = LOW;
    phaseArrowStates[1] = LOW;
    if (arrow_phase == ASSEMBLY){
      phaseArrowStates[0] = HIGH;
    }
    else if (arrow_phase == TRANSPORT2){
      phaseArrowStates[1] = HIGH;
    }

    //stone query
    if (stone_query == 1){
      StoneCount();
      Serial.print(createPacket(board_sums));
    }
  }
  writeArrowStates();
}

void writeArrowStates(){
  for (int i = 0; i < phaseNo; i++){
    digitalWrite(phasePins[i],phaseArrowStates[i]);
  }
}

void StoneCount(){
  for(int i = 0; i < BOARD_COUNT-1; i++){
    board_sums[i] = SIX_STONE_BOARD;
    for(int j = 0; j < SIX_STONE_BOARD; j++){
      board_vals[i][j] = digitalRead(board_pins[i][j]);
      board_sums[i] = board_sums[i] - board_vals[i][j]; 
    }
  }
  
  board_sums[10] = TWO_STONE_BOARD;
  board_vals[10][0] = digitalRead(board_pins[10][0]);
  board_vals[10][1] = digitalRead(board_pins[10][1]);
  board_sums[10] = board_sums[10] - board_vals[10][0] - board_vals[10][1];
}

String createPacket(int stoneCountArr[]){
  String msg = "<";
  for(int i = 0; i < BOARD_COUNT-1; i++){
    msg+=String(stoneCountArr[i]);
    msg+=",";
  }
  msg+=String(stoneCountArr[BOARD_COUNT-1]);
  msg+=">";
  return msg;
}
