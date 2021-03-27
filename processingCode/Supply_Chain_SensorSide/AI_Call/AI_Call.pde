void setup() {
  size(1600, 800);
  String[] args = new String[2];
  args[0] = "py3";
  args[1] = new String(sketchPath()+"/../../AI/SCGamePredictor/SCGamePredictor.keras.py");
  args[2]= new String(sketchPath()+"/Data/Board_set_up_video_0.mp4");
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
