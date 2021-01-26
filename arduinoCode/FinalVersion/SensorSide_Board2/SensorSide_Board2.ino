//program for Arduino of sensor side - board 2 (raspberry pi side)
//last modified 18 January 2019

//stones
#define TEN_STONE_BOARD 10
#define SIX_STONE_BOARD 6
#define TWO_STONE_BOARD 2
#define BOARD_COUNT 2
#define MAX_BOARD_NAME_LENGTH 

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


//define all of these
static int phaseNo = 5
int phasePins[phaseNo] = {3,A8,2} // {Logistics, Transport1, Demand}
int slotCount_dataPin = A9 //make sure to pullup to 20k
char msg_rec;
char msg_sent;

//boolean for Serial registration
char NOCONTACT = 'N';
char CONTACT = 'C';

//defining the phase codings
char OTHER = '0';
char ASSEMBLY = '1';
char LOGISTICS = '2';
char TRANSPORT1 = '3';
char TRANSPORT2 = '4';
char DEMAND = '5';


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupStoneBoards();
  for (int i = 0; i < phaseNo; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  pinMode(slotCount_dataPin, INPUT_PULLUP);
  establishContact()
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    msg_rec = Serial.read();

    // arrow if-else statements
    if (msg_rec == OTHER){
      for (int i = 0; i < phaseNo; i++){
        digitalWrite(phasePins[i],LOW);
      }
    }
    else if (msg_rec == LOGISTICS){
      for (int i = 0; i < phaseNo; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[0],HIGH);
    }
    else if (msg_rec == TRANSPORT1){
      for (int i = 0; i < phaseNo; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[1],HIGH);
    }
    else if (msg_rec == DEMAND){
      for (int i = 0; i < phaseNo; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[2],HIGH);
    }
  }
  else{
    Serial.println("No message");
    for (int i = 0; i < phaseNo; i++){
      digitalWrite(phasePins[i],LOW);
    }

    //figure out the data in for the slot counter -- look up the model and how the data transfer works
  }
}

void establishContact(){
  while (Serial.available() <= 0){
    Serial.println(NOCONTACT);
    delay(300);
  }
}

void setupStoneBoards(){
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
}

void sendStoneCount(){
  int board1Sum = 0;
  int board2Sum = 0;
  int board3Sum = 0;
  int board4Sum = 0;
  int board5Sum = 0;
  int board6Sum = 0;
  
  for(int i = 0; i < TEN_STONE_BOARD; i++){
    board1Sum+= board1[i];
    board2Sum+= board2[i];
    board3Sum+= board3[i];
  }
  for(int i = 0; i < SIX_STONE_BOARD; i++){
    int board4Sum+=board4[i];
    int board5Sum+=board5[i];
  }
  board6Sum = board6[0] + board6[1];
  Serial.println(); 
}
