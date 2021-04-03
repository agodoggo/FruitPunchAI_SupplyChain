void setup() {
  size(1600, 800);
  int numArgs = 22;
  String[] args = new String[numArgs];
  String state[] = new String[18];
  String filePath = new String(sketchPath()+"/../../../AI/model.4.0/log.txt");
  state = def_state(state);
 
  args[0] = "python3";
  args[1] = new String(sketchPath()+"/../../../AI/model.4.0/SCGamePredictor.keras.py");
  args[2] = new String(sketchPath()+"/../../../AI/model.4.0/model.4.0.keras");
  args[3] = filePath;
  for(int i = 4; i < numArgs; i++){
    args[i] = state[i-4];
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
    print("entered try 1");
    Process p = exec(args);  
    print("entered try 2");
    p.waitFor();
    print("entered try 3");
    String[] txtDat = loadStrings(filePath);
    print("entered try 4");
    String[] recRaw = txtDat[0].split(" ");
    print(recRaw);
    //return parseRec(recRaw);  
  }
  catch(Exception e){
    e.printStackTrace();
    //return null;
  }
  //probably need to change this, sloppy! Need this because program takes about that long to call commmand.
  delay(10000);
  String[] txtDat = loadStrings(filePath);
  String[] rec = txtDat[0].split(" ");
  String parsedRec[] = parseRec(rec);
  
}

void draw() {
  //tint(255, 20);
  //image(myMovie, mouseX, mouseY);
  //image(myMovie,0,0);
}

// Called every time a new frame is available to read
//void movieEvent(Movie m) {
//  m.read();
//}

String[] def_state(String[] writeArr){
  if(writeArr.length == 18){
   for(int i = 0; i < writeArr.length; i++){
      writeArr[i] = "0";
    }
    writeArr[0] = "10";
    writeArr[8] = "0";
    writeArr[9] = "0";
    writeArr[10] = "0";
    writeArr[11] = "0";
    writeArr[12] = "0";
    writeArr[13] = "0";
    writeArr[15] = "1";
 }
 return writeArr;
}

String returnList(String[] arr){
  String ret = "";
  for (int i = 0; i < arr.length - 1; i++){
    ret += arr[i] + " ";
  }
  ret += arr[arr.length-1];
  return(ret);
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

// python3 SCGamePredictor.keras.py  /home/pi/Documents/SupplyChain/FruitPunchAI_SupplyChain/AI/model.4.0/model.4.0.keras 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
