  //Serial messages to other Raspberry Pi
  
  
  if(pagestate == 17 || pagestate == 22 || pagestate == 26 || pagestate == 31 || pagestate == 35){ //waiting instances
    myRPiPort.write(createRPiPacket("1",NONE));  
  }
  if(pagestate==36){
    myRPiPort.write(createRPiPacket(NONE,myScore));
  }
  
  //Serial receiving from Arduino - stone count
  if (pagestate == 12){
    int[] stoneVals = parseStoneValues();
  }
  
  else if (pagestate == 18){
    int[] stoneVals = parseStoneValues();
  }
  
  else if (pagestate == 23){
    int[] stoneVals = parseStoneValues();
  }
  else if (pagestate == 36){
    myScore = new String(recvWithStartEndMarkers(myArduinoPort));
    oppScore = split(new String(recvWithStartEndMarkers(myRPiPort)),',')[1];
  }
  
  public void checkWaiting(){
  if (oppWaiting == 1){ //checks if RPi Instruction for opponent waiting is true
    if(pagestate==17){
      myArduinoPort.write(createArduinoPacket(ASSEMBLY,NONE));
    }
    else if(pagestate==22){
      myArduinoPort.write(createArduinoPacket(LOGISTICS,NONE));
    }
    else if(pagestate==26){
      myArduinoPort.write(createArduinoPacket(TRANSPORT1,NONE));
    }
    else if(pagestate==31){
      myArduinoPort.write(createArduinoPacket(DEMAND,NONE));
    }
    else if(pagestate == 35){
       myArduinoPort.write(createArduinoPacket(NONE,NONE));
    }
    pagestate = pagestate + 1;
    oppWaiting = 0;
  }
}
  
  //Serial receiving from Raspberry Pi
  
  if(pagestate == 17 || pagestate == 22 || pagestate == 26 || pagestate == 31 || pagestate == 35){
    int oppWaiting = Integer.parseInt(split(new String(recvWithStartEndMarkers(myRPiPort)),",")[1]);
    if (oppWaiting == 1){ //checks if RPi Instruction for opponent waiting is true
      pagestate = pagestate + 1;
      pagestate_change(pagestate);
    }
  }
  
  //serial sending to Raspberry Pi
public void sendWaitingStatus(){
  myRPiPort.write(createRPiPacket("1",NONE)); 
}

public void waitForArduinoData(){
  while(!ArduinoNewData){ 
    delay(100);
    myArduinoPort.write(createArduinoPacket(DEMAND,SCORE_QUERY));
  }
  ArduinoNewData = false;
}

void newGameSetup(){
  myArduinoPort_left.write(createArduinoPacket(NONE,SCORE_ERASE));
  background (0);
  pagestate_change(pagestate);
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
  return "<"+opponent_waiting+","+score+">";
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

void serialEvent(Serial thisPort){
  //store received transmission in variable
  char[] tmp = new char[32];
  tmp = recvWithStartEndMarkers(thisPort);
  
  //store in appropriate globals
  if (thisPort == myArduinoPort){
    print("Decoded message from Arduino: " + new String(tmp) +"\n");
    myScore = new String(tmp);
    ArduinoNewData = true;
  }
  if(thisPort == myRPiPort){
    String[] val = new String[2];
    val = split(new String(tmp),",");
    print("Decoded message from Raspberry Pi: " + new String(tmp) +"\n");
    oppWaiting = Integer.parseInt(val[0]);
    print("oppWaiting: " + str(oppWaiting) +"\n");
    oppScore = val[1];
    print("oppScore: " + oppScore +"\n");
    RaspberryPiNewData = true;
  }
}
