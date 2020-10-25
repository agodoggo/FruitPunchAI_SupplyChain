import processing.io.*;
I2C i2c;

byte[] in_1, in_2, in_total;
int stoneCount_byteSize = 5;
int num_MCU = 2;
boolean nextFlag = false;
int phase = 0;
String[] phaseLabels = {"Plan","Evolve","Demand"};

int last_mousePosX = 500;
int last_mousePosY = 500;

int NEXT_BUTTON_XPOS = 0;
int NEXT_BUTTON_YPOS = 0;
int NEXT_BUTTON_WIDTH = 100;
int NEXT_BUTTON_HEIGHT = 100;

int PHASE_DISPLAY_XPOS = 250;
int PHASE_DISPLAY_YPOS = 0;
int PHASE_DISPLAY_WIDTH = 100;
int PHASE_DISPLAY_HEIGHT = 100;

int STONE_DISPLAY_XPOS = 0;
int STONE_DISPLAY_YPOS = 100;
int STONE_DISPLAY_WIDTH = 500;
int STONE_DISPLAY_HEIGHT = 100;

void setup(){
  size(500,500);
  i2c = new I2C(I2C.list()[0]);
  in_total = 
}

void draw(){
  background(255,255,255);

// draw next button
  noFill();
  rect(NEXT_BUTTON_XPOS,NEXT_BUTTON_YPOS,NEXT_BUTTON_WIDTH,NEXT_BUTTON_HEIGHT);
  fill(0,102,153);
  text("NEXT",NEXT_BUTTON_XPOS+NEXT_BUTTON_WIDTH/2,NEXT_BUTTON_YPOS+NEXT_BUTTON_HEIGHT/2);
  
// draw phase display 
  noFill();
  rect(PHASE_DISPLAY_XPOS,PHASE_DISPLAY_YPOS,PHASE_DISPLAY_WIDTH,PHASE_DISPLAY_HEIGHT);
  fill(0,102,153);
  text(phaseLabels[phase],PHASE_DISPLAY_XPOS+PHASE_DISPLAY_WIDTH/2,PHASE_DISPLAY_YPOS+PHASE_DISPLAY_HEIGHT/2);

// draw stone display 
  noFill();
  rect(STONE_DISPLAY_XPOS,STONE_DISPLAY_YPOS,STONE_DISPLAY_WIDTH,STONE_DISPLAY_HEIGHT);
  fill(0,102,153);
  for(int i = 0; i < stoneCount_byteSize*num_MCU; i = i+1){
    text(in_total[i],STONE_DISPLAY_XPOS+STONE_DISPLAY_WIDTH/2,STONE_DISPLAY_YPOS+STONE_DISPLAY_HEIGHT/2);
  }
    
  if(last_mousePosX < (NEXT_BUTTON_XPOS+NEXT_BUTTON_WIDTH) && last_mousePosY < (NEXT_BUTTON_YPOS+NEXT_BUTTON_HEIGHT)){
    phase = (phase + 1)%3;
    resetMousePos();
    readStones();
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
  for(int i = 0; i < stoneCount_byteSize*num_MCU; i = i+1){
    if (i<5){
      in_total[i] = in_1[i];
    }
    else{
      in_total[i] = in_2[i-5];
    } 
  } 
}
void resetMousePos(){
  last_mousePosX = 500;
  last_mousePosY = 500;
}
void mousePressed(){
  last_mousePosX = mouseX;
  last_mousePosY = mouseY;
}
