import processing.io.*;
I2C i2c;

byte[] in_1, in_2;
int stoneCount_byteSize = 5;
boolean roundEnd = false;
int last_mousePosX,last_mousePosY;

void setup(){
  size(500,500);
  i2c = new I2C(I2C.list()[0]);
}

void draw(){
  background(0,100,0);
  rect(0,0,100,200);
  if(last_mousePosX < 100 && last_mousePosY < 200){
    roundEnd = true;
    text("word",10,30);
  }
  if(roundEnd == true){
    text("word",10,30);
    //readStones();
    roundEnd = false;
  }
}
void readStones(){
  //read from first mega board
  i2c.beginTransmission(0x8);
  in_1 = i2c.read(stoneCount_byteSize); // read stoneCount_byteSize bytes in, also read implicitly ends transmission
  i2c.endTransmission(); //end transmission just in case
  //read from second mega board
  i2c.beginTransmission(0x9);
  in_2 = i2c.read(stoneCount_byteSize); // read stoneCount_byteSize bytes in, also read implicitly ends transmission
  i2c.endTransmission(); //end transmission just in case
}
void mousePressed(){
  last_mousePosX = mouseX;
  last_mousePosY = mouseY;
}
