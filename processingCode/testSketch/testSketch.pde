import processing.video.*;
Movie myMovie;
float myMovie_md;

void setup() {
  size(1000, 1000);
  //String filePath = "/home/pi/Documents/FruitPunchAI_SupplyChain/processingCode/testSketch/Data/UI_Intro_video_co-op.mp4";
  String filePath = sketchPath()+"/Data/UI_Intro_video_co-op.mp4";
  println(filePath);
  myMovie = new Movie(this, filePath);
  frameRate(30);
}

void draw() {
  //tint(255, 20);
  //image(myMovie, mouseX, mouseY);
  
  
}

// Called every time a new frame is available to read
void movieEvent(Movie m) {
  m.read();
}
void mouseClicked(){
   myMovie.play();
   image(myMovie,0,0);
   myMovie_md = myMovie.duration();
   println("My movie time " +str(myMovie.time()));
   println("My movie dur " +str(myMovie_md));
   while(myMovie.time()<myMovie_md){
       println(myMovie.time());
       println(myMovie.available());
       if(myMovie.available()){
         myMovie.read();
         image(myMovie,0,0);
       }
      delay(50);
   }
}
