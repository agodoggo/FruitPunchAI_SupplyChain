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

//mouse Press variables
long startTimePressed;
boolean mousePress = false;
boolean displayMap = false;

//set serial baudrate
int baudRate = 9600;

//boolean for Serial registration
Serial myArduinoPort_left;
Serial myArduinoPort_right;
Serial myRPiPort;

//new data for arduino and raspberry pi
boolean ArduinoLeftNewData = false;
boolean ArduinoRightNewData = false;
boolean RaspberryPiNewData = false;

//score Strings
String myScore = "0";
String oppScore = "0";

//data variables
String name = "";
String demandMsg = "";

//stone count array
int numBoards_left = 11;
int numBoards_right = 6;
int numBoards = 17;
String[] stoneCount = new String[numBoards];

int[] mapX = new int[numBoards];
int[] mapY = new int[numBoards];

String rec[] = new String[8];
boolean textFlag = false;
boolean InstructionsSeen = false;
boolean SetupSeen = false;

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

//Stone count codes
String STONE_QUERY = "1";

//blinking cursor data
int blinkTime;
boolean blinkOn = true;

//float timeout
long fiveMinutes = 1000 * 60 * 5; //1000 ms * 60 s * 5 min
boolean inActive = false;
int lastPageState = 0;

void setup()
{
  //set screen size, this is the pixel ratio of the build in screens
  fullScreen();
  //size (800, 1280);

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
  Instructions8 = loadImage("Data/Instructions8.PNG");
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

  BoardMap = loadImage("Data/BoardMap.PNG");
  Electronics = loadImage("Data/Electronics.png");
  GameComputerCasing = loadImage("Data/GameComputerCasing.png");
  TabletCasing = loadImage("Data/TabletCasing.png");
  AssembledTablet = loadImage("Data/AssembledTablet.png");
  AssembledGameComputer = loadImage("Data/AssembledGameComputer.png");

  Determining_demand_boxes_video = sketchPath()+"/Data/Determining_demand_boxes_video.mp4";

  mapX[0] = 195;
  mapY[0] = 165;
  mapX[1] = 70;
  mapY[1] = 320;
  mapX[2] = 195;
  mapY[2] = 465;
  mapX[3] = 410;
  mapY[3] = 145;
  mapX[4] = 415;
  mapY[4] = 505;
  mapX[5] = 290;
  mapY[5] = 245;
  mapX[6] = 295;
  mapY[6] = 390; 
  mapX[7] = 615;
  mapY[7] = 245;
  mapX[8] = 200;
  mapY[8] = 310;
  mapX[9] = 305;
  mapY[9] = 295;
  mapX[10] = 300;
  mapY[10] = 150;
  mapX[11] = 305;
  mapY[11] = 445;
  mapX[12] = 420;
  mapY[12] = 230;
  mapX[13] = 420;
  mapY[13] = 380;
  mapX[14] = 630;
  mapY[14] = 120;
  mapX[15] = 530;
  mapY[15] = 225;
  mapX[16] = 530;
  mapY[16] = 370;
  //set aesthetics
  PFont erasDemi_font;
  erasDemi_font = createFont("Data/ERASDEMI.TTF", 20.0);
  textFont(erasDemi_font);

  for (int i =0; i< stoneCount.length; i++) {
    stoneCount[i] = "0";
  }

  ///JEROEN: This line gave a warning, which I fixed by joining the list into a string separated by commas
  println("Serial: " + String.join(",", Serial.list()));

  String arduinoPort_left = Serial.list()[3]; // figure this out
  myArduinoPort_left = new Serial(this, arduinoPort_left, baudRate);

  String arduinoPort_right = Serial.list()[2]; //figure this out
  myArduinoPort_right = new Serial(this, arduinoPort_right, baudRate);

  String RPiPort = Serial.list()[5]; //figure this out
  myRPiPort = new Serial(this, RPiPort, baudRate);

  myArduinoPort_left.bufferUntil('>');
  myArduinoPort_right.bufferUntil('>');
  myRPiPort.bufferUntil('>');

  myArduinoPort_left.write(createArduinoPacket_left(NONE, NONE));
  myArduinoPort_right.write(createArduinoPacket_right(NONE, NONE, NONE));
  pagestate_change(pagestate);
  initHighScore();
  displayHighScores();
  blinkTime = millis();
  blinkOn = true;
}

void draw()
{
  //loop waiting if waiting
  if (mousePress && millis() - startTimePressed > 1000) {
    image(BoardMap, 0, 0);
    String[] inv_places = new String[numBoards];
    inv_places = conv_stoneCount_invPlaces();
    fill(0, 0, 0);
    for (int i = 0; i < inv_places.length; i++) {
      text(inv_places[i], mapX[i], mapY[i]);
    }
  } else if (pagestate == 16 || pagestate == 20 || pagestate == 23 || pagestate == 28 || pagestate == 33) {
    checkWaiting();
  } else if (name.length() == 0 && (pagestate == 1 || pagestate == 2)) {
    pagestate_change(pagestate);
    fill(0, 0, 0);
    if (blinkOn) {
      line(400, 745, 400, 790);
    }
    if (millis() - 500 > blinkTime) {
      blinkTime = millis();
      blinkOn = !blinkOn;
    }
  } else if (textFlag == false && pagestate == 12 || pagestate == 16 || pagestate == 17 || pagestate == 20 || pagestate == 21 || pagestate ==23) {
    // put in refreshing circle here
  } else if (!inActive && millis() - startTimePressed > fiveMinutes) {
    inActive = true;
    lastPageState = pagestate;
    pagestate = 35;
    pagestate_change(pagestate);
  } else if (millis() - startTimePressed < fiveMinutes) {
    inActive = false;
  }
}

//}
//  int percent = (int)(100*(double)usedMem()/totalMem());
//  println(percent + "%");
//println("frameRate is " + frameRate);
//}
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
