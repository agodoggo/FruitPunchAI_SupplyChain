public void checkWaiting(){
  if (oppWaiting == 1){ //checks if RPi Instruction for opponent waiting is true
    if(pagestate==17){
      myArduinoPort_left.write(createArduinoPacket_left(ASSEMBLY,NONE));
      myArduinoPort_right.write(createArduinoPacket_right(NONE,NONE,NONE));
    }
    else if(pagestate==22){
      myArduinoPort_left.write(createArduinoPacket_left(NONE,NONE));
      myArduinoPort_right.write(createArduinoPacket_right(LOGISTICS,NONE,NONE));
    }
    else if(pagestate==26){
      myArduinoPort_left.write(createArduinoPacket_left(NONE,NONE));
      myArduinoPort_right.write(createArduinoPacket_right(TRANSPORT1,NONE,NONE));
    }
    else if(pagestate==31){
      myArduinoPort_left.write(createArduinoPacket_left(NONE,NONE));
      myArduinoPort_right.write(createArduinoPacket_right(DEMAND,NONE,NONE));
    }
    else if(pagestate == 35){
      myArduinoPort_left.write(createArduinoPacket_left(NONE,NONE));
      myArduinoPort_right.write(createArduinoPacket_right(NONE,NONE,NONE));
    }
    pagestate = pagestate + 1;
    if(pagestate==32){
      myDemandMov.jump(0.0);
      myDemandMov.play();
      demandMov = true;
      movPlaying = true;
    }
    oppWaiting = 0;
    pagestate_change(pagestate);
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

public void send_waitForArduinoData(String direction, String arrow_phase, String score_query, String stone_query){
  if(direction.equals("left")){
    while(!ArduinoLeftNewData){
      myArduinoPort_left.write(createArduinoPacket_left(arrow_phase,stone_query));
      delay(100);
    }
    ArduinoLeftNewData = false;
  }
  else{
    while(!ArduinoRightNewData){
      myArduinoPort_right.write(createArduinoPacket_right(arrow_phase,score_query,stone_query));
      delay(100);
    }
    ArduinoRightNewData = false;
  }
}

void newGameSetup(){
  myArduinoPort_left.write(createArduinoPacket_left(NONE,NONE));
  myArduinoPort_right.write(createArduinoPacket_right(NONE,SCORE_ERASE,NONE));
}

void serialEvent(Serial thisPort){
  //store received transmission in variable
  char[] tmp = new char[128];
  tmp = recvWithStartEndMarkers(thisPort);
  
  //store in appropriate globals
  if (thisPort == myArduinoPort_left){
    String[] val = new String[numBoards_left];
    val = split(new String(tmp),",");
    print("Decoded message from Arduino left: " + new String(tmp) +"\n");
    for(int i = 0; i < val.length; i++){
      stoneCount[i] = val[i];
    }
    ArduinoLeftNewData = true;
  }
  if (thisPort == myArduinoPort_right){
    String[] val = new String[numBoards_right+1];
    val = split(new String(tmp),",");
    print("Decoded message from Arduino right: " + new String(tmp) +"\n");
    myScore = val[0];
    for(int i = 1; i < val.length; i++){
      stoneCount[numBoards_left+i-1] = val[i];
    }
    ArduinoRightNewData = true;
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

String demandMsg(){
  int dice_roll = int(random(-0.5,2.5));
  if(dice_roll == 0){
    return "Dutch Retail demands" + quantDemand() + "Game Computers";
  }
  else if(dice_roll == 1){
    return "Dutch Retail demands" + quantDemand() + "Tablets";
  }
  else if(dice_roll == 2){
    return "German Retail demands" + quantDemand() + "Game Computers";
  }
  return "No country";
}

String quantDemand(){
  return str(int(random(0.5,6.5)));
}

String createArduinoPacket_left(String arrow_phase, String stone_count_query){ //direction purely for print statement
  print("New message to left Arduino : " + "<"+arrow_phase+","+stone_count_query+">"+"\n");
  return "<"+arrow_phase+","+stone_count_query+">";
}
String createArduinoPacket_right(String arrow_phase, String score_query, String stone_count_query){ //direction purely for print statement
  print("New message to right Arduino " + " : " + "<"+arrow_phase+","+score_query+","+stone_count_query+">"+"\n");
  return "<"+arrow_phase+","+score_query+","+stone_count_query+">";
}
String createRPiPacket(String opponent_waiting, String score_query){
  print("New message to Raspberry Pi: " + "<"+opponent_waiting+","+score_query+">"+"\n");
  return "<"+opponent_waiting+","+score_query+">";
}
String[] getRecommendation(){ //inv places should be 17 numbers, roundsLeft should be 10 in the first round
  //call program
  String roundsLeft = str(10-roundNo+1);
  int numArgs = 22;
  String[] args = new String[numArgs];
  String filePath = new String(sketchPath()+"/../../../AI/model.4.0/log.txt");
  
  args[0] = "python3";
  args[1] = new String(sketchPath()+"/../../../AI/model.4.0/SCGamePredictor.keras.py");
  args[2] = new String(sketchPath()+"/../../../AI/model.4.0/model.4.0.keras");
  args[3] = filePath;
  args[4] = roundsLeft;
  String inv_places[] = new String[numBoards];
  for(int i = 5; i < numArgs; i++){
    args[i] = inv_places[i-5];
  }
  try{
    Process p = exec(args);  
    p.waitFor();
    String[] txtDat = loadStrings(filePath);
    String[] recRaw = txtDat[0].split(" ");
    return parseRec(recRaw);  
  }
  catch(Exception e){
    e.printStackTrace();
    return null;
  }
}

String [] conv_stoneCount_invPlaces(){
  String inv_places[] = new String[numBoards];
  inv_places[0] = stoneCount[1];
  inv_places[1] = stoneCount[0];
  inv_places[2] = stoneCount[3];
  inv_places[3] = stoneCount[16];
  inv_places[4] = stoneCount[10];
  inv_places[5] = stoneCount[5];
  inv_places[6] = stoneCount[7];
  inv_places[7] = stoneCount[14];
  inv_places[8] = stoneCount[2];
  inv_places[9] = stoneCount[6];
  inv_places[10] = stoneCount[4];
  inv_places[11] = stoneCount[8];
  inv_places[12] = stoneCount[15];
  inv_places[13] = stoneCount[9];
  inv_places[14] = stoneCount[13];
  inv_places[15] = stoneCount[12];
  inv_places[16] = stoneCount[11];
  return inv_places;
}

String[] parseRec(String[] rec){
  String[] ret = new String[rec.length];
  ret[0] = "Order " + rec[1] + " phone components\n";
  ret[1] = "Order " + rec[2] + " electronics components\n";
  ret[2] = "Order " + rec[3] + " computer components\n";
  ret[3] = "Order " + rec[4] + " finished phones\n";
  ret[4] = "Order " + rec[5] + " finished computers\n";
  ret[5] = "Produce " + rec[6] + " phones\n";
  ret[6] = "Produce " + rec[7] + " computers\n";
  ret[7] = "Move " + rec[8] + " phones from the Netherlands to Germany";
  return ret;
}

//boolean playMovie(Movie mov){
//  println("movie time " + mov.time());
//  println("movie duration " + mov.duration());
//  //println("difference of " + (mov.duration()-mov.time()) + " is " + str((mov.duration()-mov.time()) < 0.1)); 
//  if((mov.duration()-mov.time()) > 0.1){
//      //println("Movie image refreshed");
//      image(mov,0,0);
//      return true;
//   }
//   else{
//     pagestate_change(pagestate);
//     return false;
//   }
//}
