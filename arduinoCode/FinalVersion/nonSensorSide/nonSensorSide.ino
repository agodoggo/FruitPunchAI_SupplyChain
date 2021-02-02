//program for Arduino of non-sensor side
//last modified 25 January 2019

//slot counter variables
int State;             // the current reading from the input pin
int lastState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 30;    // the debounce time; increase if the output flickers


//define all of these
const int phaseNo = 5;
int phasePins[phaseNo] = {4,7,3,2,6}; // {Assembly, Logistics, Transport1, Transport2, Demand}
int phaseArrowStates[5] = {LOW,LOW,LOW,LOW,LOW}; // {Assembly, Logistics, Transport1, Transport2, Demand}
int slotCount_dataPin = 5; //make sure to pullup to 20k
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];
boolean newData = false;
int score = 0;

//instruction packet components <ARROW_PHASE,SCORE_QUERY>
char messageFromPC[numChars] = {0};
int integerFromPC = 0;

//defining the arrow phase codings
int NONE = 0;
int ASSEMBLY = 1;
int LOGISTICS = 2;
int TRANSPORT1 = 3;
int TRANSPORT2 = 4;
int DEMAND = 5;

//defining the score query codings
int SCORE = 1;

//deciphered score
int arrow_phase = -1;
int score_query = -1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 5; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  pinMode(slotCount_dataPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  recvWithStartEndMarkers();
  changeHardwareState();
}

void writeArrowStates(){
  for (int i = 0; i < phaseNo; i++){
    digitalWrite(phasePins[i],phaseArrowStates[i]);
  }
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

void changeHardwareState(){
  if(newData == true){
    //get information from packet and change boolean
    strcpy(tempChars, receivedChars);
    parseData();
    newData = false;

    
    // arrow if-else statements
    if (arrow_phase > 0 && arrow_phase < 6){
      memset(phaseArrowStates,0,sizeof(phaseArrowStates));
      phaseArrowStates[arrow_phase-1] = HIGH;
    }
    
    //score query statements
    if (score_query == true){
      Serial.print(createPacket(String(score)));
    }
  }
  
  else{ // no data to read
    
  }

  slotCount();
  writeArrowStates();
  delay(10);
}

void parseData(){
    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    arrow_phase = atoi(strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    score_query = atoi(strtokIndx);     // convert this part to an integer

}

String createPacket(String val){
  return "<"+val+">";
}

void slotCount(){
  int reading = digitalRead(slotCount_dataPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != State) {
      State = reading;

      // only toggle the LED if the new button state is HIGH
      if (State == LOW) {
        score++;
        Serial.println(score); //comment out for final version
      }
    } 
  }
  lastState = reading;
}
