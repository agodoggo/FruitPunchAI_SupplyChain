//program for Arduino of sensor side, board 1 (non-raspberry pi side)
//last modified 18 January 2019

//define all of these
static int phaseNo = 2
int phasePins[phaseNo] = {22,23} // {Assembly, Transport2}
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
  for (int i = 0; i < phaseNo; i++){
    pinMode(phasePins[i],OUTPUT);
  }
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
    else if (msg_rec == ASSEMBLY){
      for (int i = 0; i < phaseNo; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[0],HIGH);
    }
    else if (msg_rec == TRANSPORT2){
      for (int i = 0; i < phaseNo; i++){
        digitalWrite(phasePins[i],LOW);
      }
      digitalWrite(phasePins[1],HIGH);
    }
    delay(1000);
  }
  else{
    Serial.println("No message");
    for (int i = 0; i < phaseNo; i++){
      digitalWrite(phasePins[i],LOW);
    }
  }
}

void establishContact(){
  while (Serial.available() <= 0){
    Serial.println(NOCONTACT);
    delay(300);
  }
}
