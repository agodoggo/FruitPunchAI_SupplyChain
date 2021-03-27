void setup() {
  size(1600, 800);
  String[] args = new String[3];
  String state[] = new String[18];
  state = def_state(state);
 
  args[0] = "python3";
  args[1] = new String(sketchPath()+"/../../../AI/SCGamePredictor/SCGamePredictor.keras.py");
  args[2] = returnList(state);
  print(args);
  launch(args);
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
    writeArr[8] = "2";
    writeArr[9] = "2";
    writeArr[10] = "2";
    writeArr[11] = "2";
    writeArr[12] = "3";
    writeArr[13] = "3";
    writeArr[15] = "3";
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

// python3 SCGamePredictor.keras.py  /home/pi/Documents/SupplyChain/FruitPunchAI_SupplyChain/AI/model.4.0/model.4.0.keras 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
