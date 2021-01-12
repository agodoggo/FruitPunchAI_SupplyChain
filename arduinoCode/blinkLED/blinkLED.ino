void setup() {
  // put your setup code here, to run once:
  pinMode(23,OUTPUT);
  pinMode(22,OUTPUT);
  //pinMode(A9,OUTPUT);
  //pinMode(7,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(23,HIGH);
  delay(2000);
  digitalWrite(23,LOW);
  delay(2000);
  digitalWrite(22,HIGH);
  delay(2000);
  digitalWrite(22,LOW);
  delay(2000);
}
