import processing.serial.*;

//Path to images, path is the data folder next to the processing sketch
PImage Dia1;PImage Dia2;PImage Dia3;PImage Dia4;PImage Dia5;PImage Dia6;PImage Dia7;PImage Dia8;PImage Dia9;PImage Dia10;
PImage Dia11;PImage Dia12;PImage Dia13;PImage Dia14;PImage Dia15;PImage Dia16;PImage Dia17;PImage Dia18;PImage Dia19;
PImage Dia20;PImage Dia21;PImage Dia22;PImage Dia23;PImage Dia24;PImage Dia25;PImage Dia26;PImage Dia27;PImage Dia28;PImage Dia29;
PImage Dia30;PImage Dia31;PImage Dia32;PImage Dia33;PImage Dia34;PImage Dia35;PImage Dia36;PImage Dia37;

//Set the pagestate at 0 to begin the program at Dia1
int pagestate = 1;
boolean newpage = false;

//set round number to 1 at the beignning of the program
int roundNo = 1;
int roundLim = 10; //default round limit

//set serial baudrate
int baudRate = 9600;

//boolean for Serial registration
Serial myArduinoPort_left;
Serial myArduinoPort_right;
Serial myRPiPort;

//score Strings
String myScore;
String oppScore;

//instruction packets will be sent to arduino as <ARROW_PHASE,SCORE_QUERY> for non sensor side, 1 is true, 0 is false for score query
//instruction packets will be sent between raspberry Pis as <OPPONENT_WAITING, SCORE>, 1 is true, 0 is false

//defining the instructions from RPi to Arduino
//arrow phase codes
String NONE = "0";
String ASSEMBLY = "1";
String LOGISTICS = "2";
String TRANSPORT1 = "3";
String TRANSPORT2 = "4";
String DEMAND = "5";

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
 
 String arduinoPort_left = Serial.list()[1];
 myArduinoPort_left = new Serial(this, arduinoPort_left, baudRate);
 
  String arduinoPort_right = Serial.list()[2];
 myArduinoPort_right = new Serial(this, arduinoPort_right, baudRate);

 String RPiPort = Serial.list()[3];
 myRPiPort = new Serial(this, RPiPort, baudRate);
}

void draw()
{
 background (0);
 if (newpage){
    pagestate_change(pagestate);
    newpage = false;
 }
}

char[] recvWithStartEndMarkers(Serial port) {
    int numChars = 128;
    char[] receivedChars = new char[numChars];
    
    boolean recvInProgress = false;
    int ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (port.available() > 0) {
        rc = port.readChar();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    return receivedChars;
}

String createArduinoPacket(String arrow_phase, String score_query, String stone_count_query){
  return "<"+arrow_phase+","+score_query+","+stone_count_query+">";
}
String createRPiPacket(String opponent_waiting, String score){
  return "<"+opponent_waiting+","+score_+">";
}
int[] parseStoneValues(){
    int numBoards = 17;
    int[] stoneCount = new int [numBoards];
    String[] arduinoLeft = split(new String(recvWithStartEndMarkers(myArduinoPort_left)),",");
    for(int i=0; i<=arduinoLeft.length;i++){
      stoneCount[i] = Integer.parseInt(arduinoLeft[i]);
    }
    String[] arduinoRight = split(new String(recvWithStartEndMarkers(myArduinoPort_right)),",");
    for(int i=0; i<=arduinoRight.length;i++){
      stoneCount[i+11] = Integer.parseInt(arduinoRight[i]);
    }
    return stoneCount;
}
