//Serial receiving from Raspberry Pi
public void checkWaiting(){
  if (oppWaiting == 1){ //checks if RPi Instruction for opponent waiting is true
    pagestate = pagestate + 1;
    oppWaiting = 0;
  }
}

//serial sending to Raspberry Pi
public void sendWaitingStatus(){
  myRPiPort.write(createRPiPacket("1",NONE)); 
}

public void sendScore(){
 myRPiPort.write(createRPiPacket(NONE,myScore));
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
