# define Transport2_arrows 23
# define Assembly_arrows 22

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Transport2_arrows, OUTPUT);
  pinMode(Assembly_arrows, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Transport2_arrows, HIGH);
  digitalWrite(Assembly_arrows, HIGH);
  delay(100);
}
