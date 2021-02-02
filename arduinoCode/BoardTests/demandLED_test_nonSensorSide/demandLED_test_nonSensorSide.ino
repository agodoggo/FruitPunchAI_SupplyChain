#include <Adafruit_NeoPixel.h>

//demand LED blink test
const int demand_LED = 6;
const int LED_COUNT = 16;

Adafruit_NeoPixel strip(LED_COUNT, demand_LED, NEO_GRB+NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(120);
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < LED_COUNT; i++){
    strip.setPixelColor(i,255,255,255);
  }
  strip.show();
  delay(1000);
  strip.clear();
  strip.show();
  delay(1000);
}
