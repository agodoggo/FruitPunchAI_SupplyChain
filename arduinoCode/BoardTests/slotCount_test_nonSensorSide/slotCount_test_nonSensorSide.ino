//demand LED blink test
const int slotCount = A5;
int toggle = HIGH;
int count = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(slotCount,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensValue = digitalRead(slotCount);
  if (sensValue == LOW){
    count++;
    delay(100); //to give time for stone to pass through, no multiple readings
    Serial.println(count);
  }
  
//  if (sensValue != toggle){
//    toggle = sensValue;
//    Serial.println(millis());
//  }
//  float slotCount = sensValue * (5.0/1023.0);
//  Serial.println(sensValue);
}
