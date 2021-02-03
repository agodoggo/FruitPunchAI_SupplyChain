//Serial receiving from Raspberry Pi
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
    pagestate = pagestate + 1;
    oppWaiting = 0;
  }
}

//serial sending to Raspberry Pi
public void sendWaitingStatus(){
  myRPiPort.write(createRPiPacket("1",NONE)); 
}

public void waitForArduinoData(){
  while(ArduinoNewData){
     delay(10);
  }
  ArduinoNewData = false;
}

char[] recvWithStartEndMarkers(Serial port) {
    int numChars = 32;
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
