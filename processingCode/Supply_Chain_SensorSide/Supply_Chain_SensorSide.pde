import processing.serial.*;
import processing.video.*;

//Path to images, path is the data folder next to the processing sketch
PImage Dia0; PImage Dia1;PImage Dia2;PImage Dia3;PImage Dia4;PImage Dia5;PImage Dia6;PImage Dia7;PImage Dia8;PImage Dia9;PImage Dia10;
PImage Dia11;PImage Dia12;PImage Dia13;PImage Dia14;PImage Dia15;PImage Dia16;PImage Dia17;PImage Dia18;PImage Dia19;
PImage Dia20;PImage Dia21;PImage Dia22;PImage Dia23;PImage Dia24;PImage Dia25;PImage Dia26;PImage Dia27;PImage Dia28;PImage Dia29;
PImage Dia30;PImage Dia31;PImage Dia32;PImage Dia33;PImage Dia34;PImage Dia35;PImage Dia36;PImage Dia37;

//videos
Movie myIntroMov; Movie myDemandMov; Movie mySetupMov0;Movie mySetupMov1;Movie mySetupMov2;Movie mySetupMov3;Movie mySetupMov4;Movie mySetupMov5;
Movie mySetupMov6;Movie mySetupMov7;

//colors
Color SUPPLY_col = new Color(170,111,35);
Color ASSEMBLY_col = new Color(87,40,88);
Color LOGISTICS_col = new Color(17,125,163);
Color TRANSPORT_col = new Color(125,29,29);
Color DEMAND_col = new Color(80,110,35);

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
Serial myArduinoPort_left;
Serial myArduinoPort_right;
Serial myRPiPort;

//new data for arduino and raspberry pi
boolean ArduinoLeftNewData = false;
boolean ArduinoRightNewData = false;
boolean RaspberryPiNewData = false;

//movie variables
boolean setupMovs = false;
boolean[] setupMovArr = {true,true,true,true,true,true,true,true};
boolean[] setupMovArr_play = {true,true,true,true,true,true,true,true};
boolean introMov = false;
boolean demandMov = false;
boolean movPlaying = false;

//score Strings
String myScore = "0";
String oppScore = "0";

//data variables
String name = "";

//stone count array
int numBoards_left = 11;
int numBoards_right = 6;
int numBoards = 17;
String[] stoneCount = new String[numBoards]; 

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

void setup()
{
 //set screen size, this is the pixel ratio of the build in screens
 size (800, 1280);
 
 //preload all the images in the program, this should make the entire programm fast by not loading a picture per pagestate
 Dia0 = loadImage("Data/Dia0.PNG");Dia1 = loadImage("Data/Dia1.PNG");Dia2 = loadImage("Data/Dia2.PNG");Dia3 = loadImage("Data/Dia3.PNG");Dia4 = loadImage("Data/Dia4.PNG");
 Dia5 = loadImage("Data/Dia5.PNG");Dia6 = loadImage("Data/Dia6.PNG");Dia7 = loadImage("Data/Dia7.PNG");Dia8 = loadImage("Data/Dia8.PNG");
 Dia9 = loadImage("Data/Dia9.PNG");Dia10 = loadImage("Data/Dia10.PNG");Dia11 = loadImage("Data/Dia11.PNG");Dia12 = loadImage("Data/Dia12.PNG");
 Dia13 = loadImage("Data/Dia13.PNG");Dia14 = loadImage("Data/Dia14.PNG");Dia15 = loadImage("Data/Dia15.PNG");Dia16 = loadImage("Data/Dia16.PNG");
 Dia17 = loadImage("Data/Dia17.PNG");Dia18 = loadImage("Data/Dia18.PNG");Dia19 = loadImage("Data/Dia19.PNG");Dia20 = loadImage("Data/Dia20.PNG");
 Dia21 = loadImage("Data/Dia21.PNG");Dia22 = loadImage("Data/Dia22.PNG");Dia23 = loadImage("Data/Dia23.PNG");Dia24 = loadImage("Data/Dia24.PNG");
 Dia25 = loadImage("Data/Dia25.PNG");Dia26 = loadImage("Data/Dia26.PNG");Dia27 = loadImage("Data/Dia27.PNG");Dia28 = loadImage("Data/Dia28.PNG");
 Dia29 = loadImage("Data/Dia29.PNG");Dia30 = loadImage("Data/Dia30.PNG");Dia31 = loadImage("Data/Dia31.PNG");Dia32 = loadImage("Data/Dia32.PNG");
 Dia33 = loadImage("Data/Dia33.PNG");Dia34 = loadImage("Data/Dia34.PNG");Dia35 = loadImage("Data/Dia35.PNG");Dia36 = loadImage("Data/Dia36.PNG");
 Dia37 = loadImage("Data/Dia37.PNG");
 
  //preload all movies
 myIntroMov = new Movie(this, sketchPath()+"/Data/Intro_video_AI_Co-op.mp4"); 
 myDemandMov = new Movie(this, sketchPath()+"/Data/Determining_demand_boxes_video.mp4");
 mySetupMov0 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_0.mp4");
 mySetupMov1 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_1.mp4");
 mySetupMov2 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_2.mp4");
 mySetupMov3 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_3.mp4");
 mySetupMov4 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_4.mp4");
 mySetupMov5 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_5.mp4");
 mySetupMov6 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_6.mp4");
mySetupMov7 = new Movie(this,sketchPath()+"/Data/Board_set_up_video_7.mp4");
 
 //int frameRate = 2;
 //myIntroMov.frameRate(frameRate);
 //myDemandMov.frameRate(frameRate);
 //mySetupMov0.frameRate(frameRate);
 //mySetupMov1.frameRate(frameRate);
 //mySetupMov2.frameRate(frameRate);
 //mySetupMov3.frameRate(frameRate);
 //mySetupMov4.frameRate(frameRate);
 //mySetupMov5.frameRate(frameRate);
 //mySetupMov6.frameRate(frameRate);
 
 //set aesthetics
 PFont erasDemi_font;
 erasDemi_font = createFont("Data/ERASDEMI.TTF",20.0);
 textFont(erasDemi_font);
 
 println(Serial.list());
 
 String arduinoPort_left = Serial.list()[2]; // figure this out
 myArduinoPort_left = new Serial(this, arduinoPort_left, baudRate);
 
  String arduinoPort_right = Serial.list()[3]; //figure this out
 myArduinoPort_right = new Serial(this, arduinoPort_right, baudRate);

 String RPiPort = Serial.list()[5]; //figure this out
 myRPiPort = new Serial(this, RPiPort, baudRate);
 
 myArduinoPort_left.bufferUntil('>');
 myArduinoPort_right.bufferUntil('>');
 myRPiPort.bufferUntil('>');
 
 myArduinoPort_left.write(createArduinoPacket_left(NONE,NONE));
 myArduinoPort_right.write(createArduinoPacket_right(NONE,NONE,NONE));
 pagestate_change(pagestate);
 initHighScore();
 displayHighScores();
}

void draw()
{
   //loop waiting if waiting
  if (pagestate == 17 || pagestate == 22 || pagestate == 26 || pagestate == 31 || pagestate == 35){
    checkWaiting();
  }
}
  //else if(setupMovs && movPlaying){
  //  if(setupMovArr[0]){
  //    println("ind 0 " +setupMovArr[0]);
  //    if(setupMovArr_play[0]){
  //      mySetupMov0.jump(0.0);
  //      mySetupMov0.play();
  //      setupMovArr_play[0] = false;
  //    }
  //     setupMovArr[0] = playMovie(mySetupMov0);
  //  }
  //  else if(setupMovArr[1]){
  //    println("ind 1 " + setupMovArr[1]);
  //    if(setupMovArr_play[1]){
  //      mySetupMov1.jump(0.0);
  //      println("post jump");
  //      mySetupMov1.play();
  //      println("post play");
  //      setupMovArr_play[1] = false;
  //    }
  //      //println("frame of setup 1 loaded");
  //     setupMovArr[1] = playMovie(mySetupMov1);
  //     println("post playMovie");
  //  }
  //  else if(setupMovArr[2]){
  //    println("ind 2 " + setupMovArr[2]);
  //    if(setupMovArr_play[2]){
  //      mySetupMov2.jump(0.0);
  //      mySetupMov2.play();
  //      setupMovArr_play[2] = false;
  //    }
  //     setupMovArr[2] = playMovie(mySetupMov2);
  //  }
  //  else if(setupMovArr[3]){
  //    println("ind 3 " + setupMovArr[3]);
  //    if(setupMovArr_play[3]){
  //      mySetupMov3.jump(0.0);
  //      mySetupMov3.play();
  //      setupMovArr_play[3] = false;
  //    }
  //     setupMovArr[3] = playMovie(mySetupMov3);
  //  }
  //  else if(setupMovArr[4]){
  //    println("ind 4 " + setupMovArr[4]);
  //    if(setupMovArr_play[4]){
  //      mySetupMov4.jump(0.0);
  //      mySetupMov4.play();
  //      setupMovArr_play[4] = false;
  //    }
  //     setupMovArr[4] = playMovie(mySetupMov4);
  //  }
  //  else if(setupMovArr[5]){
  //    println("ind 5 " + setupMovArr[5]);
  //    if(setupMovArr_play[5]){
  //      mySetupMov5.jump(0.0);
  //      mySetupMov5.play();
  //      setupMovArr_play[5] = false;
  //    }
  //     setupMovArr[5] = playMovie(mySetupMov5);
  //  }
  //  else if(setupMovArr[6]){
  //    println("ind 6 " + setupMovArr[6]);
  //    if(setupMovArr_play[6]){
  //      mySetupMov6.jump(0.0);
  //      mySetupMov6.play();
  //      setupMovArr_play[6] = false;
  //    }
  //      setupMovArr[6] = playMovie(mySetupMov6);
  //  }
  //  else if(setupMovArr[7]){
  //    println("ind 7 " + setupMovArr[7]);
  //    if(setupMovArr_play[7]){
  //      mySetupMov7.jump(0.0);
  //      mySetupMov7.play();
  //      setupMovArr_play[7] = false;
  //    }
  //      pagestate =12;
  //      setupMovArr[7] = playMovie(mySetupMov7);
  //  }
  //  else if(setupMovArr[7] == false){
  //    setupMovs = false;
  //    movPlaying = false;
  //    for(int i = 0; i < setupMovArr.length; i++){
  //      setupMovArr[i] = true;
  //      setupMovArr_play[i] = true;
  //    }
  //    pagestate =12;
  //    pagestate_change(pagestate);
  //  }
  //}
  //else if(introMov){
  //  //println(introMov);
  //  if(movPlaying){
  //    pagestate = 3;
  //    introMov = playMovie(myIntroMov);
  //  }
  //  else{
  //    introMov = false;
  //    pagestate = 3;
  //    pagestate_change(pagestate);
  //  }
  //}
  //else if(demandMov){
  //  //println(demandMov);
  //  if(movPlaying){
  //    pagestate =32;
  //    demandMov = playMovie(myDemandMov);
  //  }
  //  else{
  //    demandMov = false;
  //    pagestate=32;
  //    pagestate_change(pagestate);
  //  }
    
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
class Color{

    int r;        // the red color vaulue (0-255)
    int g;       // the yellow color vaulue (0-255
    int b;       // the blue color vaulue (0-255)

    public Color(int r, int g, int b){
        this.r = r;
        this.g = g;
        this.b = b;
    }
}
// Called every time a new frame is available to read
void movieEvent(Movie m) {
  m.read();
}
