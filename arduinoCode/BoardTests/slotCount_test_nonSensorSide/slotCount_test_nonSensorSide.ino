//demand LED blink test
const int slotCount = A5;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(slotCount,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int slotCount = analogRead(slotCount);
  Serial.println(slotCount);
}
