//program for Arduino of sensor side - board 2 (raspberry pi side)
//last modified 18 January 2019

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
  {A6,A7}
};

//define all of these
const int phaseNo = 3;
int phasePins[phaseNo] = {3,A8,2}; // {Logistics, Transport1, Demand}
int slotCount_dataPin = A9; //make sure to pullup to 20k
int phaseArrowStates[phaseNo] = {LOW,LOW,LOW};
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
int score = 0;

//boolean for Serial registration
boolean newData = false;

//defining the phase codings
int NONE = 0;
int ASSEMBLY = 1;
int LOGISTICS = 2;
int TRANSPORT1 = 3;
int TRANSPORT2 = 4;
int DEMAND = 5;
int SCORE = 6;
int STONE_COUNT = 8;

//deciphered score
int arrow_phase = -1;
int score_query = -1;
int stone_query = -1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(slotCount_dataPin, INPUT_PULLUP);
  for (int i = 0; i < phaseNo; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  setupStoneBoards(); 
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
    if (arrow_phase == NONE){
      phaseArrowStates[0]=LOW;phaseArrowStates[1]=LOW;phaseArrowStates[2]=LOW;
    }
    else if (arrow_phase == LOGISTICS){
      phaseArrowStates[0]=HIGH;phaseArrowStates[1]=LOW;phaseArrowStates[2]=LOW;
    }
    else if (arrow_phase == TRANSPORT1){
       phaseArrowStates[0]=LOW;phaseArrowStates[1]=HIGH;phaseArrowStates[2]=LOW;
    }
    else if (arrow_phase == DEMAND){
       phaseArrowStates[0]=LOW;phaseArrowStates[1]=LOW;phaseArrowStates[2]=HIGH;
    }
    
    //score query
    if (score_query == 1){
      Serial.print(createPacket(String(score)));
    }

    //stone query
    if (stone_query == 1){
      StoneCount();
      sendStoneCount();
    }
  }
  writeArrowStates();
  delay(10);
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
    for(int j = 0; j < TEN_STONE_BOARD; j++){
      board_vals[i][j] = digitalRead((board_pins[i][j], INPUT));
      board_sums[i] = board_sums[i] + board_vals[i][j];
    }
  }
  for(int i = 3; i < BOARD_COUNT-1; i++){
    for(int j = 0; j < SIX_STONE_BOARD; j++){
      board_vals[i][j] = digitalRead((board_pins[i][j], INPUT));
      board_sums[i] = board_sums[i] + board_vals[i][j];pinMode(board_pins[i][j], INPUT);
    }
  }
  for(int j = 0; j < TWO_STONE_BOARD; j++){
      board_vals[5][j] = digitalRead((board_pins[5][j], INPUT));
      board_sums[5] = board_sums[5] + board_vals[5][j];
  }
}
void sendStoneCount(){
  Serial.print("<");
  for(int i = 0; i < BOARD_COUNT-1; i++){
    Serial.print(board_sums[i]);
    Serial.print(",");
  }
  Serial.print(board_sums[BOARD_COUNT-1]);
  Serial.print(">");
}
String createPacket(String val){
  return "<"+val+">";
}
void writeArrowStates(){
  for (int i = 0; i < phaseNo; i++){
    digitalWrite(phasePins[i],phaseArrowStates[i]);
  }
}
