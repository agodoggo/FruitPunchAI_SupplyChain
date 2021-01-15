int Transport2_arrows = 23;
int Assembly_arrows = 22;
char val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Transport2_arrows, OUTPUT);
  pinMode(Assembly_arrows, OUTPUT);
  establishContact();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    val = Serial.read();
  
    if (val == '1')
    {
      digitalWrite(Transport2_arrows,HIGH);
      digitalWrite(Assembly_arrows,HIGH);
      delay(1000);
    }
  }
  else{
    Serial.println("Hello world 1");
    digitalWrite(Transport2_arrows, LOW);
    digitalWrite(Assembly_arrows, LOW);
    delay(50);
  }
}

void establishContact(){
  while (Serial.available() <= 0) {
    Serial.println("A");
    delay(300);
  }
}
