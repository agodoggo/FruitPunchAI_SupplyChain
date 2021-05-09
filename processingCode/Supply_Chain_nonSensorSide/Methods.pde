//Serial receiving from Raspberry Pi
import java.nio.charset.StandardCharsets;
import java.util.concurrent.ThreadLocalRandom;
public void checkWaiting(){
  if (oppWaiting == 1){ //checks if RPi Instruction for opponent waiting is true
    if(pagestate==16){
      myArduinoPort.write(createArduinoPacket(ASSEMBLY,NONE));
    }
    else if(pagestate==20){
      myArduinoPort.write(createArduinoPacket(LOGISTICS,NONE));
    }
    else if(pagestate==23){
      myArduinoPort.write(createArduinoPacket(TRANSPORT1,NONE));
    }
    else if(pagestate==28){
      myArduinoPort.write(createArduinoPacket(DEMAND,NONE));
    }
    else if(pagestate == 33){
       myArduinoPort.write(createArduinoPacket(NONE,NONE));
    }
    pagestate = pagestate + 1;
    oppWaiting = 0;
    pagestate_change(pagestate);
    if (pagestate==29) {
      playMovie(Determining_demand_boxes_video);
      pagestate = 30;
      pagestate_change(pagestate);
    }
  }
}

//serial sending to Raspberry Pi
public void sendWaitingStatus(){
  myRPiPort.write(createRPiPacket(WAITING,NONE)); 
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

public void waitForArduinoData(){
  while(!ArduinoNewData){ 
    delay(100);
    myArduinoPort.write(createArduinoPacket(DEMAND,SCORE_QUERY));
  }
  ArduinoNewData = false;
}

void newGameSetup(){
  myArduinoPort.write(createArduinoPacket(NONE,SCORE_ERASE));
}

void serialEvent(Serial thisPort){
  //store received transmission in variable
  char[] tmp = new char[128];
  tmp = recvWithStartEndMarkers(thisPort);
  
  //store in appropriate globals
  if (thisPort == myArduinoPort){
    print("Decoded message from Arduino: " + new String(tmp) +"\n");
    myScore = new String(tmp);
    ArduinoNewData = true;
  }
  if(thisPort == myRPiPort){
    String[] val = new String[3];
    val = split(new String(tmp),",");
    print("Decoded message from Raspberry Pi: " + new String(tmp) +"\n");
    oppWaiting = Integer.parseInt(val[0]);
    print("oppWaiting: " + str(oppWaiting) +"\n");
    oppScore = val[1];
    print("oppScore: " + oppScore +"\n");
    byte[] bytes= val[2].getBytes();
    int lastInd = 34;
    for(int i = 0; i < bytes.length; i++){
      if (bytes[i] == 'y' || bytes[i] == 's'){
        lastInd = i+1;
      }
    }
    byte[] bytes_kept = Arrays.copyOfRange(bytes,0,lastInd);
    demandMsg = new String(bytes_kept, StandardCharsets.UTF_8);
    RaspberryPiNewData = true;
  }
}

String demandMsg() {
  int dice_roll = int(ThreadLocalRandom.current().nextInt(0, 3));
  if (dice_roll == 0) {
    return quantDemand() + " Game Computers\nThe Netherlands";
  } else if (dice_roll == 1) {
    return quantDemand() + " Tablets\nThe Netherlands";
  } else if (dice_roll == 2) {
    return quantDemand() + " Tablets\nGermany";
  }
  return "No country";
}

String quantDemand(){
  return str(ThreadLocalRandom.current().nextInt(1, 7)); //https://stackoverflow.com/questions/363681/how-do-i-generate-random-integers-within-a-specific-range-in-java
}

String createArduinoPacket(String arrow_phase, String score_query){
  print("New message to Arduino: " + "<"+arrow_phase+","+score_query+">"+"\n");
  return "<"+arrow_phase+","+score_query+">";
}
String createRPiPacket(String opponent_waiting, String score_query){
  print("New message to Raspberry Pi: " + "<"+opponent_waiting+","+score_query+">"+"\n");
  return "<"+opponent_waiting+","+score_query+">";
}
void playMovie(String filePath){
  String[] args = new String[3];
  args[0] = "omxplayer";
  args[1] = filePath;
  args[2] = "--orientation=90";
  Process p = exec(args);
  try {
   int result = p.waitFor();
   println("the process returned " + result);
   }
   catch (InterruptedException e) { }
}
