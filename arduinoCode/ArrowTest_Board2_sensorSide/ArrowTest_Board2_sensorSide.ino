# define Transport1_arrow A8
# define Logistics_arrow 3

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Transport1_arrow, OUTPUT);
  pinMode(Logistics_arrow, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(Transport1_arrow, HIGH);
  digitalWrite(Logistics_arrow, HIGH);
  delay(100);
}
