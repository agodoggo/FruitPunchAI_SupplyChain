void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(49,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("49 digital");Serial.print(": ");Serial.print(digitalRead(49));Serial.print(", ");
//  Serial.print("49 analog");Serial.print(": ");Serial.print(analogRead(49)*5.0/1023.0);Serial.print(", ");
  Serial.println();
}
