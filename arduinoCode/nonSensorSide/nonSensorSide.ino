//program for Arduino of non-sensor side
//last modified 18 January 2019

//define all of these
int phasePins[5] = {4,7,3,2,6}; // {Assembly, Logistics, Transport1, Transport2, Demand}
int slotCount_dataPin = 5; //make sure to pullup to 20k
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
  for (int i = 0; i < 5; i++){
    pinMode(phasePins[i],OUTPUT);
  }
  pinMode(slotCount_dataPin, INPUT_PULLUP);
  establishContact();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    msg_rec = Serial.read();

    // arrow if-else statements
    if (msg_rec == OTHER){
      for (int i = 0; i < 5; i++){
        digitalWrite(phasePins[i],LOW);
      }
    }
    else if (msg_rec == ASSEMBLY){
      for (int i = 0; i < 5; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[0],HIGH);
    }
    else if (msg_rec == LOGISTICS){
      for (int i = 0; i < 5; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[1],HIGH);
    }
    else if (msg_rec == TRANSPORT1){
      for (int i = 0; i < 5; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[2],HIGH);
    }
    else if (msg_rec == TRANSPORT2){
      for (int i = 0; i < 5; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[3],HIGH);
    }
    else if (msg_rec == DEMAND){
      for (int i = 0; i < 5; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[4],HIGH);
    }
    delay(1000);
  }
  else{
    Serial.println("No message");
    for (int i = 0; i < 5; i++){
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
