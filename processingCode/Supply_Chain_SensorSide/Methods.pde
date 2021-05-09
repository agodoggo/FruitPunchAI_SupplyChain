import java.nio.charset.StandardCharsets;
import java.util.concurrent.ThreadLocalRandom;
public void checkWaiting() {
  if (oppWaiting == 1) { //checks if RPi Instruction for opponent waiting is true
    if (pagestate==16) {
      send_waitForArduinoData("left", ASSEMBLY, NONE, STONE_QUERY);
      send_waitForArduinoData("right", NONE, NONE, STONE_QUERY);
      rec = getRecommendation();
    } else if (pagestate==20) {
      send_waitForArduinoData("left", NONE, NONE, STONE_QUERY);
      send_waitForArduinoData("right", LOGISTICS, NONE, STONE_QUERY);
      rec = getRecommendation();
    } else if (pagestate==23) {
      myArduinoPort_left.write(createArduinoPacket_left(NONE, NONE));
      myArduinoPort_right.write(createArduinoPacket_right(TRANSPORT1, NONE, NONE));
    } else if (pagestate==28) {
      myArduinoPort_left.write(createArduinoPacket_left(NONE, NONE));
      myArduinoPort_right.write(createArduinoPacket_right(DEMAND, NONE, NONE));
    } else if (pagestate == 33) {
      myArduinoPort_left.write(createArduinoPacket_left(NONE, NONE));
      myArduinoPort_right.write(createArduinoPacket_right(NONE, NONE, NONE));
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
      } else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
      }
    } else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
  return receivedChars;
}

public void send_waitForArduinoData(String direction, String arrow_phase, String score_query, String stone_query) {
  if (direction.equals("left")) {
    while (!ArduinoLeftNewData) {
      myArduinoPort_left.write(createArduinoPacket_left(arrow_phase, stone_query));
      delay(100);
    }
    ArduinoLeftNewData = false;
  } else {
    while (!ArduinoRightNewData) {
      myArduinoPort_right.write(createArduinoPacket_right(arrow_phase, score_query, stone_query));
      delay(100);
    }
    ArduinoRightNewData = false;
  }
}

void newGameSetup() {
  myArduinoPort_left.write(createArduinoPacket_left(NONE, NONE));
  myArduinoPort_right.write(createArduinoPacket_right(NONE, SCORE_ERASE, NONE));
}

void serialEvent(Serial thisPort) {
  //store received transmission in variable
  char[] tmp = new char[128];
  tmp = recvWithStartEndMarkers(thisPort);

  //store in appropriate globals
  if (thisPort == myArduinoPort_left) {
    String[] val = new String[numBoards_left];
    val = split(new String(tmp), ",");
    print("Decoded message from Arduino left: " + new String(tmp) +"\n");
    for (int i = 0; i < numBoards_left; i++) {
      //print("index " + str(i) + "reached\n");
      byte[] bytes = val[i].getBytes();
      //print("bytes found\n");
      //print("bytes length " + str(bytes.length) + "\n");
      byte[] bytes_kept = new byte[] {bytes[0]};
      //print("passed to byte array");
      stoneCount[i] = new String(bytes_kept, StandardCharsets.UTF_8);
      //print("stone count\n");
      //stoneCount[i] = val[i];
    }
    ArduinoLeftNewData = true;
  }
  if (thisPort == myArduinoPort_right) {
    String[] val = new String[numBoards_right+1];
    val = split(new String(tmp), ",");
    print("Decoded message from Arduino right: " + new String(tmp) +"\n");
    myScore = val[0];
    for (int i = 1; i < numBoards_right+1; i++) {
      byte[] bytes = val[i].getBytes();
      byte[] bytes_kept = new byte[] {bytes[0]};
      stoneCount[numBoards_left+i-1] = new String(bytes_kept, StandardCharsets.UTF_8);

      //stoneCount[numBoards_left+i-1] = val[i];
    }
    ArduinoRightNewData = true;
  }
  if (thisPort == myRPiPort) {
    String[] val = new String[2];
    val = split(new String(tmp), ",");
    print("Decoded message from Raspberry Pi: " + new String(tmp) +"\n");
    oppWaiting = Integer.parseInt(val[0]);
    print("oppWaiting: " + str(oppWaiting) +"\n");
    oppScore = val[1];
    print("oppScore: " + oppScore +"\n");
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

String quantDemand() {
  return str(ThreadLocalRandom.current().nextInt(1, 7)); //https://stackoverflow.com/questions/363681/how-do-i-generate-random-integers-within-a-specific-range-in-java
}

String createArduinoPacket_left(String arrow_phase, String stone_count_query) { //direction purely for print statement
  print("New message to left Arduino : " + "<"+arrow_phase+","+stone_count_query+">"+"\n");
  return "<"+arrow_phase+","+stone_count_query+">";
}
String createArduinoPacket_right(String arrow_phase, String score_query, String stone_count_query) { //direction purely for print statement
  print("New message to right Arduino " + " : " + "<"+arrow_phase+","+score_query+","+stone_count_query+">"+"\n");
  return "<"+arrow_phase+","+score_query+","+stone_count_query+">";
}
String createRPiPacket(String opponent_waiting, String score_query, String DemandMessageTmp) {
  print("New message to Raspberry Pi: " + "<"+opponent_waiting+","+score_query+","+DemandMessageTmp+">"+"\n");
  return "<"+opponent_waiting+","+score_query+","+DemandMessageTmp+">";
}
String[] getRecommendation() { //inv places should be 17 numbers, roundsLeft should be 10 in the first round
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
  for (int i =0; i<numBoards; i++) {
    inv_places[i] = "0";
  }
  inv_places = conv_stoneCount_invPlaces();
  for (int i = 5; i < numArgs; i++) {
    args[i] = inv_places[i-5];
  }
  try { 
    Process p = exec(args);  
    p.waitFor();
    String[] txtDat = loadStrings(filePath);
    print("entered try 4\n");
    String[] recRaw = txtDat[0].split(" ");
    textFlag = true;
    return parseRec(recRaw);
  }
  catch(Exception e) {
    e.printStackTrace();
    return null;
  }
}

void printBytes(String var)
{
  byte[] varBytes = var.getBytes();
  for (byte b : varBytes)
  {
    print(b);
  }
}

String[] conv_stoneCount_invPlaces() {
  String inv[] = new String[numBoards];
  //print("this is stonecount: \n");
  //for (int i = 0; i < stoneCount.length; i++) {
  //  print(str(i) + ": " + stoneCount[i] + "\n");
  //}
  if (inv.length == numBoards) {
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

String[] parseRec(String[] rec) {
  String[] ret = new String[rec.length];
  //ret[0] = "Order " + rec[1] + " tablet casings\n";
  //ret[1] = "Order " + rec[2] + " electronics\n";
  //ret[2] = "Order " + rec[3] + " game computer casings\n";
  //ret[3] = "Order " + rec[4] + " assembled tablets\n";
  //ret[4] = "Order " + rec[5] + " assembled computers\n";
  //ret[5] = "Assemble " + rec[6] + " tablets\n";
  //ret[6] = "Assemble " + rec[7] + " game computers\n";
  //ret[7] = "Move " + rec[8] + " tablets from the Netherlands to Germany";
  ret[0] = rec[1];
  ret[1] = rec[2];
  ret[2] = rec[3];
  ret[3] = rec[4];
  ret[4] = rec[5];
  ret[5] = rec[6];
  ret[6] = rec[7];
  ret[7] = rec[8];
  return ret;
}

void playMovie(String filePath) {
  String[] args = new String[2];
  args[0] = "omxplayer";
  args[1] = filePath;
  Process p = exec(args);
  try {
    int result = p.waitFor();
    println("the process returned " + result);
  }
  catch (InterruptedException e) {
  }
}
