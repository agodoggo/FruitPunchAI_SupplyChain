//demand LED blink test
const int slotCount = 5;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(slotCount,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int slotCount = digitalRead(slotCount);
  Serial.println(slotCount);
}
