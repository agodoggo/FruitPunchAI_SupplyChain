//program for Arduino of sensor side - board 2 (raspberry pi side)
//last modified 3 February 2021

#include <Adafruit_NeoPixel.h>

//stones
const int TEN_STONE_BOARD =  10;
const int SIX_STONE_BOARD =  6;
const int TWO_STONE_BOARD = 2;
const int BOARD_COUNT = 6;

int board_sums[BOARD_COUNT];
int board_vals[BOARD_COUNT][TEN_STONE_BOARD];
int board_pins[BOARD_COUNT][TEN_STONE_BOARD] = {
  {22,24,26,28,30,32,34,36,38,40},
  {4,5,6,7,8,9,10,11,12,13},
  {23,25,27,29,31,33,35,37,39,41},
  {42,44,46,48,50,52},
  {A0,A1,A2,A3,A4,A5},
  {A14,A15}
};

//slot counter variables
int State;             // the current reading from the input pin
int lastState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 2;    // the debounce time; increase if the output flickers

//slot counter LED
const int demand_LED = 2;
const int LED_COUNT = 16;
Adafruit_NeoPixel strip(LED_COUNT, demand_LED, NEO_GRB+NEO_KHZ800);
boolean demand_phase = false;

//define all of these
const int phaseNo = 2;
int phasePins[phaseNo] = {3,A8}; // {Logistics, Transport1}
int slotCount_dataPin = A9; //make sure to pullup to 20k
int phaseArrowStates[phaseNo] = {LOW,LOW};
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
int score = 0;

//boolean for Serial registration
boolean newData = false;

//instruction packet components <ARROW_PHASE,SCORE_QUERY>
//defining the arrow phase codings
const int NONE = 0;
const int ASSEMBLY = 1;
const int LOGISTICS = 2;
const int TRANSPORT1 = 3;
const int TRANSPORT2 = 4;
const int DEMAND = 5;

//information codes
const int SCORE_QUERY = 1;
const int SCORE_ERASE = 2;

//deciphered score
int arrow_phase = -1;
int score_query = -1;
int stone_query = -1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(slotCount_dataPin, INPUT_PULLUP);
  for (int i = 0; i < phaseNo; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  setupStoneBoards();
  setupLEDstrip();
}

void loop() {
  // put your main code here, to run repeatedly:
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
    score_query = atoi(strtokIndx);     // convert this part to an integer

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
    phaseArrowStates[0]=LOW;
    phaseArrowStates[1]=LOW;
    demand_phase=false;
    if (arrow_phase == LOGISTICS){
      phaseArrowStates[0]=HIGH;
    }
    else if (arrow_phase == TRANSPORT1){
       phaseArrowStates[1]=HIGH;
    }
    else if (arrow_phase == DEMAND){
       demand_phase = true;
    }
    
    //score query
    if (score_query == SCORE_QUERY){
      Serial.print(createPacket(String(score),board_sums));
    }
    else if (score_query == SCORE_ERASE){
      score=0;
    }

    //stone query
    if (stone_query == 1){
      StoneCount();
      Serial.print(createPacket(String(score),board_sums));
    }
  }
  writeArrowStates();
  slotCount();
}


void setupStoneBoards(){
  for(int i = 0; i < BOARD_COUNT-3; i++){
    for(int j = 0; j < TEN_STONE_BOARD; j++){
      pinMode(board_pins[i][j], INPUT);
    }
  }
  for(int i = 3; i < BOARD_COUNT-1; i++){
    for(int j = 0; j < SIX_STONE_BOARD; j++){
      pinMode(board_pins[i][j], INPUT);
    }
  }
  for(int j = 0; j < TWO_STONE_BOARD; j++){
    pinMode(board_pins[5][j], INPUT);
  }
}

void StoneCount(){
  for(int i = 0; i < BOARD_COUNT-3; i++){
    board_sums[i] = TEN_STONE_BOARD;
    for(int j = 0; j < TEN_STONE_BOARD; j++){
      board_vals[i][j] = digitalRead(board_pins[i][j]);
      board_sums[i] = board_sums[i] - board_vals[i][j];
    }
  }
  for(int i = 3; i < BOARD_COUNT-1; i++){
    board_sums[i] = SIX_STONE_BOARD;
    for(int j = 0; j < SIX_STONE_BOARD; j++){
      board_vals[i][j] = digitalRead(board_pins[i][j]);
      board_sums[i] = board_sums[i] - board_vals[i][j];
    }
  }
  board_sums[BOARD_COUNT-1] = TWO_STONE_BOARD;
  for(int j = 0; j < TWO_STONE_BOARD; j++){
      board_vals[BOARD_COUNT-1][j] = digitalRead(board_pins[BOARD_COUNT-1][j]);
      board_sums[BOARD_COUNT-1] = board_sums[BOARD_COUNT-1] - board_vals[BOARD_COUNT-1][j];
  }
}
String createPacket(String score, int stoneCountArr[]){
  String msg = "<"+score+",";
  for(int i = 0; i < BOARD_COUNT-1; i++){
    msg+=String(stoneCountArr[i]);
    msg+=",";
  }
  msg+=String(stoneCountArr[BOARD_COUNT-1]);
  msg+=">";
  return msg;
}
void slotCount(){
  int reading = digitalRead(slotCount_dataPin);
  if (reading != lastState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != State) {
      State = reading;
      if (State == LOW) {
        score=score+1;
        Serial.print(createPacket(String(score),board_sums));
      }
    } 
  }
  lastState = reading;
}

void writeArrowStates(){
  for (int i = 0; i < phaseNo; i++){
    digitalWrite(phasePins[i],phaseArrowStates[i]);
  }
  if(demand_phase==true){
    setLEDStripHIGH();
  }
  else{
    setLEDStripLOW();
  }
}

void setupLEDstrip(){
  strip.begin();
  strip.setBrightness(120);
  strip.show();
}

void setLEDStripHIGH(){
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i,255,255,255);
  }
  strip.show();
}

void setLEDStripLOW(){
  strip.clear();
  strip.show();
}
