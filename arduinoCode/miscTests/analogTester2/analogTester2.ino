void setup() {
  // put your setup code here, to run once:
  pinMode(A14,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A14));
}
