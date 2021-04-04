  String[] args = new String[2];
  
void setup() {
  size(1600, 800);
  args[0] = "omxplayer";
  args[1]= new String(sketchPath()+"/Data/Board_set_up_video_0.mp4");
  //args[2] = "--orientation 90";
  //args[3] = "--aspect-mode stretch";
  println(args);
  noLoop();
  doExec();
}

void draw() {
  //exec(args);
}

void doExec(){
  Process p = exec(args);
try {
   int result = p.waitFor();
   println("the process returned " + result);
 } catch (InterruptedException e) { }
}


// Called every time a new frame is available to read
//void movieEvent(Movie m) {
//  m.read();
//}
