//slot counter counting test - based on Debounce Arduino example

//debouncing variables
int State;             // the current reading from the input pin
int lastState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 30;    // the debounce time; increase if the output flickers

const int slotCount = A5;
int toggle = HIGH;
int count = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(slotCount,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = digitalRead(slotCount);

  // If the switch changed, due to noise or pressing:
  if (reading != lastState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != State) {
      State = reading;

      // only toggle the LED if the new button state is HIGH
      if (State == LOW) {
        count++;
        Serial.println(count);
      }
    } 
  }
  lastState = reading;
}

void basicStoneRead(){
  int sensValue = digitalRead(slotCount);
  if (sensValue == LOW){
    count++;
    delay(100); //to give time for stone to pass through, no multiple readings
    Serial.println(count);
  }
}

void misc(){
  //  if (sensValue != toggle){
//    toggle = sensValue;
//    Serial.println(millis());
//  }
//  float slotCount = sensValue * (5.0/1023.0);
//  Serial.println(sensValue);
}
