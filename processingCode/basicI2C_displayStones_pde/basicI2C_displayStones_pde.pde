import processing.io.*;
I2C i2c;

void setup(){
  i2c = new I2C(I2C.list()[0]);
}

void draw(){
  
}
void readStones(){
  //read from first mega board
  i2c.beginTransmission(0x8);
  byte[] in = i2c.read(1) // read 1 byte in, also read implicitly ends transmission
  i2c.endTransmission(0x8); //end transmission just in case
  //read from second mega board
  i2c.beginTransmission(0x9);
  byte[] in = i2c.read(1) // read 1 byte in, also read implicitly ends transmission
  i2c.endTransmission(0x8); //end transmission just in case
}
