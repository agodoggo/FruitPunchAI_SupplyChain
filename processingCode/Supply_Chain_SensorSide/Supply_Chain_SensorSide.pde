import processing.serial.*;
//import processing.video.*;

//Path to images, path is the data folder next to the processing sketch
PImage Dia0; 
PImage Dia1;
PImage Dia2;
PImage Dia3;
PImage Dia4;
PImage Dia5;
PImage Dia6;
PImage Dia7;
PImage Dia8;
PImage Dia9;
PImage Dia10;
PImage Dia11;
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
PImage Dia37;
PImage Dia_0; 
PImage Dia_1; 
PImage Dia_2; 
PImage Dia_3;
PImage Dia_4;
PImage Dia_5;
PImage Dia_6;
PImage Dia_7;
PImage BoardMap;
PImage Electronics;
PImage GameComputerCasing;
PImage TabletCasing;
PImage AssembledTablet;
PImage AssembledGameComputer;
//videos
String Board_setup_video_0;
String Board_setup_video_1;
String Board_setup_video_2;
String Board_setup_video_3;
String Board_setup_video_4;
String Board_setup_video_5;
String Board_setup_video_6;
String Board_setup_video_7;
String Determining_demand_boxes_video;
String Intro_video_AI_Co_op;

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
int blinkTime = millis();
boolean blinkOn = true;

void setup()
{
  //set screen size, this is the pixel ratio of the build in screens
  size (800, 1280);

  //preload all the images in the program, this should make the entire programm fast by not loading a picture per pagestate
  Dia0 = loadImage("Data/Dia0.PNG");
  Dia1 = loadImage("Data/Dia1.PNG");
  Dia2 = loadImage("Data/Dia2.PNG");
  Dia3 = loadImage("Data/Dia3.PNG");
  Dia4 = loadImage("Data/Dia4.PNG");
  Dia5 = loadImage("Data/Dia5.PNG");
  Dia6 = loadImage("Data/Dia6.PNG");
  Dia7 = loadImage("Data/Dia7.PNG");
  Dia8 = loadImage("Data/Dia8.PNG");
  Dia9 = loadImage("Data/Dia9.PNG");
  Dia10 = loadImage("Data/Dia10.PNG");
  Dia11 = loadImage("Data/Dia11.PNG");
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
  Dia37 = loadImage("Data/Dia37.PNG");
  Dia_0 = loadImage("Data/Board_set_up_video_0_LF.png");
  Dia_1 = loadImage("Data/Board_set_up_video_1_LF.png");
  Dia_2 = loadImage("Data/Board_set_up_video_2_LF.png");
  Dia_3 = loadImage("Data/Board_set_up_video_3_LF.png");
  Dia_4 = loadImage("Data/Board_set_up_video_4_LF.png");
  Dia_5 = loadImage("Data/Board_set_up_video_5_LF.png");
  Dia_6 = loadImage("Data/Board_set_up_video_6_LF.png");
  Dia_7 = loadImage("Data/Board_set_up_video_7_LF.png");
  BoardMap = loadImage("Data/BoardMap.png");
  Electronics = loadImage("Data/Electronics.png");
  GameComputerCasing = loadImage("Data/GameComputerCasing");
  TabletCasing = loadImage("Data/TabletCasing");
  AssembledTablet = loadImage("Data/AssembledTablet");
  AssembledGameComputer = loadImage("Data/AssembledGameComputer");

  //preload all movies
  Intro_video_AI_Co_op = sketchPath()+"/Data/Intro_video_AI_Co-op.mp4"; 
  Determining_demand_boxes_video = sketchPath()+"/Data/Determining_demand_boxes_video.mp4";
  Board_setup_video_0 = sketchPath()+"/Data/Board_set_up_video_0.mp4";
  Board_setup_video_1 = sketchPath()+"/Data/Board_set_up_video_1.mp4";
  Board_setup_video_2 = sketchPath()+"/Data/Board_set_up_video_2.mp4";
  Board_setup_video_3 = sketchPath()+"/Data/Board_set_up_video_3.mp4";
  Board_setup_video_4 = sketchPath()+"/Data/Board_set_up_video_4.mp4";
  Board_setup_video_5 = sketchPath()+"/Data/Board_set_up_video_5.mp4";
  Board_setup_video_6 = sketchPath()+"/Data/Board_set_up_video_6.mp4";
  Board_setup_video_7 = sketchPath()+"/Data/Board_set_up_video_7.mp4";

  mapX[0] = 280;
  mapY[0] = 200;
  mapX[1] = 122;
  mapY[1] = 395;
  mapX[2] = 290;
  mapY[2] = 595;
  mapX[3] = 584;
  mapY[3] = 153;
  mapX[4] = 584;
  mapY[4] = 646;
  mapX[5] = 420;
  mapY[5] = 300;
  mapX[6] = 424;
  mapY[6] = 495; 
  mapX[7] = 860;
  mapY[7] = 304;
  mapX[8] = 290;
  mapY[8] = 395;
  mapX[9] = 420;
  mapY[9] = 380;
  mapX[10] = 420;
  mapY[10] = 174;
  mapX[11] = 420;
  mapY[11] = 578;
  mapX[12] = 580;
  mapY[12] = 300;
  mapX[13] = 580;
  mapY[13] = 500;
  mapX[14] = 855;
  mapY[14] = 142;
  mapX[15] = 719;
  mapY[15] = 285;
  mapX[16] = 720;
  mapY[16] = 480;
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
  } else if (pagestate == 17 || pagestate == 22 || pagestate == 26 || pagestate == 31 || pagestate == 35) {
    checkWaiting();
  } else if (name.length() == 0 && pagestate == 1 || pagestate ==2 ) {
    if (blinkOn) {
      line(700, 380, 700, 420);
    }
    if (millis() - 500 > blinkTime) {
      blinkTime = millis();
      blinkOn = !blinkOn;
    }
  }
  else if (textFlag == false && pagestate == 12 || pagestate == 16 || pagestate == 17 || pagestate == 20 || pagestate == 21 || pagestate ==23){
    // put in refreshing circle here
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
// Called every time a new frame is available to read
//void movieEvent(Movie m) {
//  m.read();
//}
