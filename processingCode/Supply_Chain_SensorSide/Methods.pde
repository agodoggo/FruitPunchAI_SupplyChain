import java.nio.charset.StandardCharsets;
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
      //myDemandMov.jump(0.0);
      //myDemandMov.play();
      //demandMov = true;
      //movPlaying = true;
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
    for(int i = 0; i < numBoards_left; i++){
      print("index " + str(i) + "reached\n");
      byte[] bytes = val[i].getBytes();
      print("bytes found\n");
      print("bytes length " + str(bytes.length) + "\n");
      byte[] bytes_kept = new byte[] {bytes[0]};
      print("passed to byte array");
      stoneCount[i] = new String(bytes_kept,StandardCharsets.UTF_8);
      print("stone count\n");
      //stoneCount[i] = val[i];
    }
    ArduinoLeftNewData = true;
  }
  if (thisPort == myArduinoPort_right){
    String[] val = new String[numBoards_right+1];
    val = split(new String(tmp),",");
    print("Decoded message from Arduino right: " + new String(tmp) +"\n");
    myScore = val[0];
    for(int i = 1; i < numBoards_right+1; i++){
      byte[] bytes = val[i].getBytes();
      byte[] bytes_kept = new byte[] {bytes[0]};
      stoneCount[numBoards_left+i-1] = new String(bytes_kept,StandardCharsets.UTF_8);
      
      //stoneCount[numBoards_left+i-1] = val[i];
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
  String roundsLeft = str(11-roundNo);
  int numArgs = 22;
  String[] args = new String[numArgs];
  String filePath = new String(sketchPath()+"/../../AI/model.4.0/log.txt");
  
  args[0] = "python3";
  args[1] = new String(sketchPath()+"/../../AI/model.4.0/SCGamePredictor.keras.py");
  args[2] = new String(sketchPath()+"/../../AI/model.4.0/model.4.0.keras");
  args[3] = filePath;
  args[4] = roundsLeft;
  String inv_places[] = new String[numBoards];
  for(int i =0;i<numBoards;i++){
    inv_places[i] = "0";
  }
  inv_places = conv_stoneCount_invPlaces();
  for(int i = 5; i < numArgs; i++){
    args[i] = inv_places[i-5];
  }
  try{ 
    for(int j = 0; j < args.length; j++){
      if(args[j] == null){
        print(str(j) + " is NULL\n");
      }
      else{
        print(str(j) + ": " + args[j] + "\n");
      }
    }
    print("entered try 1\n");
    print("command: " + String.join(",", args)+"\n");
    String[] newArgs = new String[numArgs];
    newArgs[0] = "python3";
    newArgs[1] = "/home/pi/Documents/FruitPunchAI_SupplyChain/processingCode/Supply_Chain_SensorSide/../../AI/model.4.0/SCGamePredictor.keras.py";
    newArgs[2] = "/home/pi/Documents/FruitPunchAI_SupplyChain/processingCode/Supply_Chain_SensorSide/../../AI/model.4.0/model.4.0.keras";
    newArgs[3] = "/home/pi/Documents/FruitPunchAI_SupplyChain/processingCode/Supply_Chain_SensorSide/../../AI/model.4.0/log.txt";
    newArgs[4] = "10";
    newArgs[5] = "0";
    newArgs[6] = "0";
    newArgs[7] = "0";
    newArgs[8] = "0";
    newArgs[9] = "0";
    newArgs[10] = "0";
    newArgs[11] = "0";
    newArgs[12] = "0";
    newArgs[13] = "0";
    newArgs[14] = "0";
    newArgs[15] = "0";
    newArgs[16] = "0";
    newArgs[17] = "0";
    newArgs[18] = "0";
    newArgs[19] = "1";
    newArgs[20] = "0";
    newArgs[21] = "0";
    
    for(int k = 0; k < newArgs.length; k++)
    {
      print(str(k) + "\n");
      print("#"+args[k]+"#\n");
      print("#"+newArgs[k]+"#\n");
      print("Same: " + str(args[k].equals(newArgs[k])) + "\n");
      print("Same int: " + str(args[k].equals(0)) + "\n");
      print("Null: " + str(args[k].equals(null)) + "\n");
      print("Bits: args - ");
      printBytes(args[k]);
      print("\n   newargs - ");
      printBytes(newArgs[k]);
      print("\n");
      print("type: " + args[k].getClass().getName() + "\n\n");
    }
    Process p = exec(args);  
    print("entered try 2\n");
    p.waitFor();
    print("entered try 3\n");
    String[] txtDat = loadStrings(filePath);
    print("entered try 4\n");
    String[] recRaw = txtDat[0].split(" ");
    return parseRec(recRaw);  
  }
  catch(Exception e){
    e.printStackTrace();
    return null;
  }
}

void printBytes(String var)
{
  byte[] varBytes = var.getBytes();
  for(byte b: varBytes)
  {
     print(b);
  }
}

String[] conv_stoneCount_invPlaces(){
  String inv[] = new String[numBoards];
  print("this is stonecount: \n");
  for(int i = 0; i < stoneCount.length; i++){
    print(str(i) + ": " + stoneCount[i] + "\n");
  }
  if(inv.length == numBoards){
    inv[0] = stoneCount[1];
    inv[1] = stoneCount[0];
    inv[2] = stoneCount[3];
    inv[3] = stoneCount[16];
    inv[4] = stoneCount[10];
    inv[5] = stoneCount[5];
    inv[6] = stoneCount[7];
    inv[7] = stoneCount[14];
    inv[8] = stoneCount[2];
    inv[9] = stoneCount[6];
    inv[10] = stoneCount[4];
    inv[11] = stoneCount[8];
    inv[12] = stoneCount[15];
    inv[13] = stoneCount[9];
    inv[14] = stoneCount[13];
    inv[15] = stoneCount[12];
    inv[16] = stoneCount[11];
  }
  return inv;
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
