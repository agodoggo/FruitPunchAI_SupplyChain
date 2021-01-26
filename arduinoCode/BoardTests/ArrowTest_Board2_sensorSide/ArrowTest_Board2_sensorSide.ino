int Transport1_arrow = A8;
int Logistics_arrow = 3;
char val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Transport1_arrow, OUTPUT);
  pinMode(Logistics_arrow, OUTPUT);
  establishContact();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    val = Serial.read();
  
    if (val == '1')
    {
      digitalWrite(Transport1_arrow,HIGH);
      digitalWrite(Logistics_arrow,HIGH);
      delay(1000);
    }
  }
  else{
    Serial.println("Hello world 2")
    digitalWrite(Transport1_arrow, LOW);
    digitalWrite(Logistics_arrow, LOW);
    delay(50);
  }
}

void establishContact(){
  while (Serial.available() <= 0) {
    Serial.println("A");
    delay(300);
  }
}
