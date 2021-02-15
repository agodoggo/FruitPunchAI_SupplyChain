import processing.video.*;
Movie myMovie;

void setup() {
  size(1600, 800);
  String filePath = sketchPath("/Data/UI_Intro_video_co-op.mp4");
  myMovie = new Movie(this, filePath);
  myMovie.play();
}

void draw() {
  //tint(255, 20);
  //image(myMovie, mouseX, mouseY);
  image(myMovie,0,0);
  
}

// Called every time a new frame is available to read
void movieEvent(Movie m) {
  m.read();
}
