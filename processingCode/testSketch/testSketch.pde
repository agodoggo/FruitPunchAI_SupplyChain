import processing.video.*;
Movie myMovie;
boolean ended;

void setup() {
  size(1000, 1000);
  //String filePath = "/home/pi/Documents/FruitPunchAI_SupplyChain/processingCode/testSketch/Data/UI_Intro_video_co-op.mp4";
  String filePath = sketchPath()+"/Data/UI_Intro_video_co-op.mp4";
  println(filePath);
    frameRate(30);
  myMovie = new Movie(this, filePath){
    @ Override public void eosEvent() {
      super.eosEvent();
      myEoS();
    }
  };
}

void draw() {
  //tint(255, 20);
  //image(myMovie, mouseX, mouseY);
  if(!ended) background(myMovie);
  
}

// Called every time a new frame is available to read
void movieEvent(final Movie m) {
  m.read();
}
void mouseClicked(){
   myMovie.play();
   image(myMovie,0,0);
   while(myMovie.time()<myMovie.duration()){
       println(myMovie.time());
       println(myMovie.available());
       if(myMovie.available()){
         myMovie.read();
         image(myMovie,0,0);
       }
      delay(50);
   }
}

void myEoS() {
  ended = true;
  frameRate(1);
}
