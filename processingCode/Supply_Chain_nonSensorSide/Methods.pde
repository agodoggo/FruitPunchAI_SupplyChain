//Serial receiving from Raspberry Pi
public void checkWaiting(){
  print(oppWaiting);
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
    pagestate_change(pagestate);
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
  background (0);
  pagestate_change(pagestate);
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

String createArduinoPacket(String arrow_phase, String score_query){
  print("New message to Arduino: " + "<"+arrow_phase+","+score_query+">"+"\n");
  return "<"+arrow_phase+","+score_query+">";
}
String createRPiPacket(String opponent_waiting, String score_query){
  print("New message to Raspberry Pi: " + "<"+opponent_waiting+","+score_query+">"+"\n");
  return "<"+opponent_waiting+","+score_query+">";
}
