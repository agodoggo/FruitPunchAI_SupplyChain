import processing.serial.*;

//Path to images, path is the data folder next to the processing sketch
PImage Dia1;PImage Dia2;PImage Dia3;PImage Dia4;PImage Dia5;PImage Dia6;PImage Dia7;PImage Dia8;PImage Dia9;PImage Dia10;
PImage Dia11;PImage Dia12;PImage Dia13;PImage Dia14;PImage Dia15;PImage Dia16;PImage Dia17;PImage Dia18;PImage Dia19;
PImage Dia20;PImage Dia21;PImage Dia22;PImage Dia23;PImage Dia24;PImage Dia25;PImage Dia26;PImage Dia27;PImage Dia28;PImage Dia29;
PImage Dia30;PImage Dia31;PImage Dia32;PImage Dia33;PImage Dia34;PImage Dia35;PImage Dia36;PImage Dia37;

//Set the pagestate at 0 to begin the program at Dia1
int pagestate = 1;

//set round number to 1 at the beignning of the program
int roundNo = 1;
int roundLim = 10; //default round limit

//set serial baudrate
int baudRate = 9600;

//boolean for Serial registration
Serial[] myPorts = new Serial[2];
int[] dataIn = new int[2];
boolean[] dataIn_changed = {false,false};

//defining the phase codings
int msg_rec;
int OTHER = 0;
int ASSEMBLY = 1;
int LOGISTICS = 2;
int TRANSPORT1 = 3;
int TRANSPORT2 = 4;
int DEMAND = 5;
int SCORE = 6;
int WAITING = 7;

void setup()
{
 //set screen size, this is the pixel ratio of the build in screens
 size (800, 1280);
 
 //preload all the images in the program, this should make the entire programm fast by not loading a picture per pagestate
 Dia1 = loadImage("Data/Dia1.PNG");Dia2 = loadImage("Data/Dia2.PNG");Dia3 = loadImage("Data/Dia3.PNG");Dia4 = loadImage("Data/Dia4.PNG");
 Dia5 = loadImage("Data/Dia5.PNG");Dia6 = loadImage("Data/Dia6.PNG");Dia7 = loadImage("Data/Dia7.PNG");Dia8 = loadImage("Data/Dia8.PNG");
 Dia9 = loadImage("Data/Dia9.PNG");Dia10 = loadImage("Data/Dia10.PNG");Dia11 = loadImage("Data/Dia11.PNG");Dia12 = loadImage("Data/Dia12.PNG");
 Dia13 = loadImage("Data/Dia13.PNG");Dia14 = loadImage("Data/Dia14.PNG");Dia15 = loadImage("Data/Dia15.PNG");Dia16 = loadImage("Data/Dia16.PNG");
 Dia17 = loadImage("Data/Dia17.PNG");Dia18 = loadImage("Data/Dia18.PNG");Dia19 = loadImage("Data/Dia19.PNG");Dia20 = loadImage("Data/Dia20.PNG");
 Dia21 = loadImage("Data/Dia21.PNG");Dia22 = loadImage("Data/Dia22.PNG");Dia23 = loadImage("Data/Dia23.PNG");Dia24 = loadImage("Data/Dia24.PNG");
 Dia25 = loadImage("Data/Dia25.PNG");Dia26 = loadImage("Data/Dia26.PNG");Dia27 = loadImage("Data/Dia27.PNG");Dia28 = loadImage("Data/Dia28.PNG");
 Dia29 = loadImage("Data/Dia29.PNG");Dia30 = loadImage("Data/Dia30.PNG");Dia31 = loadImage("Data/Dia31.PNG");Dia32 = loadImage("Data/Dia32.PNG");
 Dia33 = loadImage("Data/Dia33.PNG");Dia34 = loadImage("Data/Dia34.PNG");Dia35 = loadImage("Data/Dia35.PNG");Dia36 = loadImage("Data/Dia36.PNG");
 Dia37 = loadImage("Data/Dia37.PNG");
 
 String arduinoPort = Serial.list()[1];
 myPorts[0] = new Serial(this, arduinoPort, baudRate);

 String RPiPort = Serial.list()[2];
 myPorts[1] = new Serial(this, RPiPort, baudRate);
 
 
}

void draw()
{
 background (0);
 pagestate_change(pagestate);
}

void serialEvent(Serial myPort){ //read in Data from Serial Port
  int portNumber = -1;
  
  for (int p = 0; p < myPorts.length; p++){
    if (myPort == myPorts[p]){
      portNumber = p;
    }
  }
  
  int inByte = myPort.read();
  dataIn_changed[portNumber] = inByte != dataIn[portNumber]; //used to see if new value is read
  dataIn[portNumber] = inByte;
  
  println("Got " + inByte + " from serial port " + portNumber);
}
