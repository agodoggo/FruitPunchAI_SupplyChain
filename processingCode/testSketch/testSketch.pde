import processing.video.*;
Movie myMovie;

void setup() {
  size(1600, 800);
  myMovie = new Movie(this, "Data/UI_Intro_video_co-op.mp4");
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
