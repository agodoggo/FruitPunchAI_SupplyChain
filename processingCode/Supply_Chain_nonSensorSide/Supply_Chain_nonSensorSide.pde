import processing.serial.*;
//import processing.video.*;

//Path to images, path is the data folder next to the processing sketch
PImage Dia0; 
PImage Dia1;
PImage Dia2;
PImage Dia3_Clickable;
PImage Dia3_Hidden;
PImage Dia12;
PImage Dia13;
PImage Dia14;
PImage Dia15;
PImage Dia16;
PImage Dia17;
PImage Dia18;
PImage Dia19;
PImage Dia20;
PImage Dia21;
PImage Dia22;
PImage Dia23;
PImage Dia24;
PImage Dia25;
PImage Dia26;
PImage Dia27;
PImage Dia28;
PImage Dia29;
PImage Dia30;
PImage Dia31;
PImage Dia32;
PImage Dia33;
PImage Dia34;
PImage Dia35;
PImage Dia36;
PImage Instructions8;
PImage Instructions1;
PImage Instructions2;
PImage Instructions3;
PImage Instructions4;
PImage Instructions5;
PImage Instructions6;
PImage Instructions7;
PImage Setup1;
PImage Setup2;
PImage Setup3;
PImage Setup4;
PImage Setup5;
PImage Setup6;
PImage Setup7;
PImage Setup8;



PImage BoardMap;
PImage Electronics;
PImage GameComputerCasing;
PImage TabletCasing;
PImage AssembledTablet;
PImage AssembledGameComputer;


String Determining_demand_boxes_video;

//colors
Color SUPPLY_col = new Color(170, 111, 35);
Color ASSEMBLY_col = new Color(87, 40, 88);
Color LOGISTICS_col = new Color(17, 125, 163);
Color TRANSPORT_col = new Color(125, 29, 29);
Color DEMAND_col = new Color(80, 110, 35);

//Set the pagestate at 0 to begin the program at Dia1
int pagestate = 0;

//set round number to 1 at the beignning of the program
int roundNo = 1;
int roundLim = 10; //default round limit

//global int
int oppWaiting = 0;

//set serial baudrate
int baudRate = 9600;

//boolean for Serial registration
Serial myArduinoPort;
Serial myRPiPort;

//new data for arduino and raspberry pi
boolean ArduinoNewData = false;
boolean RaspberryPiNewData = false;

//score Strings
String myScore = "0";
String oppScore = "0";

//data variables
String name = "";
String demandMsg = "";

//instruction packets will be sent to arduino as <ARROW_PHASE,SCORE_QUERY> for non sensor side, 1 is true, 0 is false for score query
//instruction packets will be sent between raspberry Pis as <OPPONENT_WAITING, SCORE>, 1 is true, 0 is false

//defining the instructions from RPi to Arduino
String NONE = "0";

//arrow phase codes
String ASSEMBLY = "1";
String LOGISTICS = "2";
String TRANSPORT1 = "3";
String TRANSPORT2 = "4";
String DEMAND = "5";

//information codes
String SCORE_QUERY = "1";
String SCORE_ERASE = "2";

//Waiting codes
String WAITING = "1";

//blinking cursor data
int blinkTime = millis();
boolean blinkOn = true;

//float timeout
long fiveMinutes = 1000 * 60 * 5; //1000 ms * 60 s * 5 min
boolean inActive = false;
int lastPageState = 0;
boolean InstructionsSeen = false;
boolean SetupSeen = false;

//mouse Press variables
long startTimePressed;
boolean mousePress = false;

void setup()
{ 
  //set screen size, this is the pixel ratio of the build in screens
  size (800, 1280);

  //preload all the images in the program, this should make the entire programm fast by not loading a picture per pagestate
   Dia0 = loadImage("Data/Dia0.PNG");
  Dia1 = loadImage("Data/Dia1.PNG");
  Dia2 = loadImage("Data/Dia2.PNG");
  Dia3_Clickable = loadImage("Data/Dia3_Clickable.PNG");
  Dia3_Hidden = loadImage("Data/Dia3_Hidden.PNG");
  Dia12 = loadImage("Data/Dia12.PNG");
  Dia13 = loadImage("Data/Dia13.PNG");
  Dia14 = loadImage("Data/Dia14.PNG");
  Dia15 = loadImage("Data/Dia15.PNG");
  Dia16 = loadImage("Data/Dia16.PNG");
  Dia17 = loadImage("Data/Dia17.PNG");
  Dia18 = loadImage("Data/Dia18.PNG");
  Dia19 = loadImage("Data/Dia19.PNG");
  Dia20 = loadImage("Data/Dia20.PNG");
  Dia21 = loadImage("Data/Dia21.PNG");
  Dia22 = loadImage("Data/Dia22.PNG");
  Dia23 = loadImage("Data/Dia23.PNG");
  Dia24 = loadImage("Data/Dia24.PNG");
  Dia25 = loadImage("Data/Dia25.PNG");
  Dia26 = loadImage("Data/Dia26.PNG");
  Dia27 = loadImage("Data/Dia27.PNG");
  Dia28 = loadImage("Data/Dia28.PNG");
  Dia29 = loadImage("Data/Dia29.PNG");
  Dia30 = loadImage("Data/Dia30.PNG");
  Dia31 = loadImage("Data/Dia31.PNG");
  Dia32 = loadImage("Data/Dia32.PNG");
  Dia33 = loadImage("Data/Dia33.PNG");
  Dia34 = loadImage("Data/Dia34.PNG");
  Dia35 = loadImage("Data/Dia35.PNG");
  Dia36 = loadImage("Data/Dia36.PNG");
  Instructions0 = loadImage("Data/Instructions0.PNG");
  Instructions1 = loadImage("Data/Instructions1_coop.PNG");
  Instructions2 = loadImage("Data/Instructions2.PNG");
  Instructions3 = loadImage("Data/Instructions3.PNG");
  Instructions4 = loadImage("Data/Instructions4.PNG");
  Instructions5 = loadImage("Data/Instructions5.PNG");
  Instructions6 = loadImage("Data/Instructions6.PNG");
  Instructions7 = loadImage("Data/Instructions7.PNG");
  Setup1 = loadImage("Data/Setup1.PNG");
  Setup2 = loadImage("Data/Setup2.PNG");
  Setup3 = loadImage("Data/Setup3.PNG");
  Setup4 = loadImage("Data/Setup4.PNG");
  Setup5 = loadImage("Data/Setup5.PNG");
  Setup6 = loadImage("Data/Setup6.PNG");
  Setup7 = loadImage("Data/Setup7.PNG");
  Setup8 = loadImage("Data/Setup8.PNG");
  
  BoardMap = loadImage("Data/BoardMap.png");
  Electronics = loadImage("Data/Electronics.png");
  GameComputerCasing = loadImage("Data/GameComputerCasing");
  TabletCasing = loadImage("Data/TabletCasing");
  AssembledTablet = loadImage("Data/AssembledTablet");
  AssembledGameComputer = loadImage("Data/AssembledGameComputer");
  
  Determining_demand_boxes_video = sketchPath()+"/Data/Determining_demand_boxes_video.mp4";

  //set aesthetics
  PFont erasDemi_font;
  erasDemi_font = createFont("Data/ERASDEMI.TTF", 20.0);
  textFont(erasDemi_font);

  println(Serial.list());

  //setup serial ports
  String arduinoPort = Serial.list()[2];
  myArduinoPort = new Serial(this, arduinoPort, baudRate);

  String RPiPort = Serial.list()[4];
  myRPiPort = new Serial(this, RPiPort, baudRate);

  myArduinoPort.bufferUntil('>');
  myRPiPort.bufferUntil('>');

  myArduinoPort.write(createArduinoPacket(NONE, NONE));
  pagestate_change(pagestate);
  initHighScore();
  displayHighScores();
}

void draw()
{
  //loop waiting if waiting
  if (pagestate == 17 || pagestate == 22 || pagestate == 26 || pagestate == 31 || pagestate == 35) {
    checkWaiting();
  }
  else if (name.length() == 0 && pagestate == 1 || pagestate ==2 ){
    if (blinkOn){
      line(700, 380, 700, 420);
    }
    if (millis() - 500 > blinkTime) {
      blinkTime = millis();
      blinkOn = !blinkOn;
    }
  }
  else if(!inActive && millis() - startTimePressed > fiveMinutes){
      inActive = true;
      lastPageState = pagestate;
      pagestate = 35;
      pagestate_change(pagestate);
    }
    else if(millis() - startTimePressed < fiveMinutes){
      inActive = false;
    }
  int percent = (int)(100*(double)usedMem()/totalMem());
  println(percent + "%");
}
public long totalMem() {
  return Runtime.getRuntime().totalMemory();
}

public long usedMem() {
  return Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
}
class Color {

  int r;        // the red color vaulue (0-255)
  int g;       // the yellow color vaulue (0-255
  int b;       // the blue color vaulue (0-255)

  public Color(int r, int g, int b) {
    this.r = r;
    this.g = g;
    this.b = b;
  }
}
