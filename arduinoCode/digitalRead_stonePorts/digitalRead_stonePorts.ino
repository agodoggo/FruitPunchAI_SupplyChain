/******************************************************************************
Piezo_Vibration_Sensor.ino
Example sketch for SparkFun's Piezo Vibration Sensor
  (https://www.sparkfun.com/products/9197)
Jim Lindblom @ SparkFun Electronics
April 29, 2016

- Connect a 1Mohm resistor across the Piezo sensor's pins.
- Connect one leg of the Piezo to GND
- Connect the other leg of the piezo to A0

Vibrations on the Piezo sensor create voltags, which are sensed by the Arduino's
A0 pin. Check the serial monitor to view the voltage generated.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int PIEZO_PIN_0 = 22; // Piezo output
const int PIEZO_PIN_1 = 24; // Piezo output
const int PIEZO_PIN_2 = 26; // Piezo output
const int PIEZO_PIN_3 = 28; // Piezo output
const int PIEZO_PIN_4 = 30; // Piezo output
const int PIEZO_PIN_5 = 32; // Piezo output
const int PIEZO_PIN_6 = 34; // Piezo output
const int PIEZO_PIN_7 = 36; // Piezo output
const int PIEZO_PIN_8 = 38; // Piezo output
const int PIEZO_PIN_9 = 40l; // Piezo output

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  // Read Piezo ADC value in, and convert it to a voltage
  sensorRead();
}
void sensorRead(){
  // Read Piezo ADC value in, and convert it to a voltage
  int piezoV_0 = digitalRead(PIEZO_PIN_0);
  int piezoV_1 = digitalRead(PIEZO_PIN_1);
  int piezoV_2 = digitalRead(PIEZO_PIN_2);
  int piezoV_3 = digitalRead(PIEZO_PIN_3);
  int piezoV_4 = digitalRead(PIEZO_PIN_4);
  int piezoV_5 = digitalRead(PIEZO_PIN_5);
  int piezoV_6 = digitalRead(PIEZO_PIN_6);
  int piezoV_7 = digitalRead(PIEZO_PIN_7);
  int piezoV_8 = digitalRead(PIEZO_PIN_8);
  int piezoV_9 = digitalRead(PIEZO_PIN_9);
  
//  float piezoV_0 = piezoADC_0 / 1023.0 * 5.0;
//  float piezoV_1 = piezoADC_1 / 1023.0 * 5.0;
//  float piezoV_2 = piezoADC_2 / 1023.0 * 5.0;
//  float piezoV_3 = piezoADC_3 / 1023.0 * 5.0;
//  float piezoV_4 = piezoADC_4 / 1023.0 * 5.0;
//  float piezoV_5 = piezoADC_5 / 1023.0 * 5.0;
//  float piezoV_6 = piezoADC_6 / 1023.0 * 5.0;
//  float piezoV_7 = piezoADC_7 / 1023.0 * 5.0;
//  float piezoV_8 = piezoADC_8 / 1023.0 * 5.0;
//  float piezoV_9 = piezoADC_9 / 1023.0 * 5.0;
 
  Serial.print("piezoV_0:");Serial.print(piezoV_0);Serial.print(", ");
  Serial.print("piezoV_1:");Serial.print(piezoV_1);Serial.print(", ");
  Serial.print("piezoV_2:");Serial.print(piezoV_2);Serial.print(", ");
  Serial.print("piezoV_3:");Serial.print(piezoV_3);Serial.print(", ");
  Serial.print("piezoV_4:");Serial.print(piezoV_4);Serial.print(", ");
  Serial.print("piezoV_5:");Serial.print(piezoV_5);Serial.print(", ");
  Serial.print("piezoV_6:");Serial.print(piezoV_6);Serial.print(", ");
  Serial.print("piezoV_7:");Serial.print(piezoV_7);Serial.print(", ");
  Serial.print("piezoV_8:");Serial.print(piezoV_8);Serial.print(", ");
  Serial.print("piezoV_9:");Serial.print(piezoV_9);Serial.print(", ");
  Serial.println();  
}
