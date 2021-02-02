//demand LED blink test
const int demand_LED = 6;
void setup() {
  // put your setup code here, to run once:
  pinMode(demand_LED,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(demand_LED,HIGH);
  delay(1000);
  digitalWrite(demand_LED,LOW);
  delay(1000);
}
