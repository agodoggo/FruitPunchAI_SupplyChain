//program for Arduino of non-sensor side
//last modified 18 January 2019

//define all of these
int phasePins[5] = {4,7,3,2,6}; // {Assembly, Logistics, Transport1, Transport2, Demand}
int phaseArrowStates[5] = {LOW,LOW,LOW,LOW,LOW}; // {Assembly, Logistics, Transport1, Transport2, Demand}
int slotCount_dataPin = 5; //make sure to pullup to 20k
int msg_rec;
int score = 0;

//defining the phase codings
int OTHER = '0';
int ASSEMBLY = '1';
int LOGISTICS = '2';
int TRANSPORT1 = '3';
int TRANSPORT2 = '4';
int DEMAND = '5';
int SCORE = '6';


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 5; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  pinMode(slotCount_dataPin, INPUT_PULLUP);
  establishContact();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    msg_rec = Serial.read();
    
    // arrow if-else statements
    if (msg_rec == OTHER){
      phaseArrowStates = {LOW,LOW,LOW,LOW,LOW};
    }
    else if (msg_rec == ASSEMBLY){
      phaseArrowStates = {HIGH,LOW,LOW,LOW,LOW};
    }
    else if (msg_rec == LOGISTICS){
       phaseArrowStates = {LOW,HIGH,LOW,LOW,LOW};
    }
    else if (msg_rec == TRANSPORT1){
       phaseArrowStates = {LOW,LOW,HIGH,LOW,LOW};
    }
    else if (msg_rec == TRANSPORT2){
       phaseArrowStates = {LOW,LOW,LOW,HIGH,LOW};
    }
    else if (msg_rec == DEMAND){
       phaseArrowStates = {LOW,LOW,LOW,LOW,HIGH};
    }
    
    //score query statements
    if (msg_rec == SCORE){
      Serial.print(score);
    }
  }
  
  else{ // no data to read
    
  }

  score += digitalRead(slotCount_dataPin); //need to check if this is sufficient, otherwise need to implement a debounce
  writeArrowStates();
  delay(10);
}

void writeArrowStates(){
  for (int i = 0; i < 5; i++){
    digitalWrite(phasePins[i],phaseArrowStates[i]);
  }
}
