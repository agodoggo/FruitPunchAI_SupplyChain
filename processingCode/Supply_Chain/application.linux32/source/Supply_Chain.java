import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Supply_Chain extends PApplet {



//Path to images, path is the data folder next to the processing sketch
PImage Dia1;PImage Dia2;PImage Dia3;PImage Dia4;PImage Dia5;PImage Dia6;PImage Dia7;PImage Dia8;PImage Dia9;PImage Dia10;
PImage Dia11;PImage Dia12;PImage Dia13;PImage Dia14;PImage Dia15;PImage Dia16;PImage Dia17;PImage Dia18;PImage Dia19;
PImage Dia20;PImage Dia21;PImage Dia22;PImage Dia23;PImage Dia24;PImage Dia25;PImage Dia26;PImage Dia27;PImage Dia28;PImage Dia29;
PImage Dia30;PImage Dia31;PImage Dia32;PImage Dia33;PImage Dia34;PImage Dia35;PImage Dia36;PImage Dia37;

//Set the pagestate at 0 to begin the program at Dia1
int pagestate=1;


public void setup()
{
 //set screen size, this is the pixel ratio of the build in screens
 
 
 //preload all the images in the program, this should make the entire programm fast by not loading a picture per pagestate
 Dia1 = loadImage("Data/Dia1.PNG");Dia2 = loadImage("Data/Dia2.PNG");Dia3 = loadImage("Data/Dia3.PNG");Dia4 = loadImage("Data/Dia4.PNG");
 Dia5 = loadImage("Data/Dia5.PNG");Dia6 = loadImage("Data/Dia6.PNG");Dia7 = loadImage("Data/Dia7.PNG");Dia8 = loadImage("Data/Dia8.PNG");
 Dia9 = loadImage("Data/Dia9.PNG");Dia10 = loadImage("Data/Dia10.PNG");Dia11 = loadImage("Data/Dia11.PNG");Dia12 = loadImage("Data/Dia12.PNG");
 Dia13 = loadImage("Data/Dia13.PNG");Dia14 = loadImage("Data/Dia14.PNG");Dia15 = loadImage("Data/Dia15.PNG");Dia16 = loadImage("Data/Dia16.PNG");
 Dia17 = loadImage("Data/Dia17.PNG");Dia18 = loadImage("Data/Dia18.PNG");Dia19 = loadImage("Data/Dia19.PNG");Dia20 = loadImage("Data/Dia20.PNG");
 Dia21 = loadImage("Data/Dia21.PNG");Dia22 = loadImage("Data/Dia22.PNG");Dia23 = loadImage("Data/Dia23.PNG");Dia24 = loadImage("Data/Dia24.PNG");
 Dia25 = loadImage("Data/Dia25.PNG");Dia26 = loadImage("Data/Dia26.PNG");Dia27 = loadImage("Data/Dia27.PNG");Dia28 = loadImage("Data/Dia28.PNG");
 Dia29 = loadImage("Data/Dia29.PNG");Dia30 = loadImage("Data/Dia30.PNG");Dia31 = loadImage("Data/Dia31.PNG");Dia32 = loadImage("Data/Dia32.PNG");
 Dia33 = loadImage("Data/Dia33.PNG");Dia34 = loadImage("Data/Dia34.PNG");Dia35 = loadImage("Data/Dia35.PNG");Dia36 = loadImage("Data/Dia36.PNG");
 Dia37 = loadImage("Data/Dia37.PNG");
}

public void draw()
{
 background (0);
 pagestate_change(pagestate);
 //x,y - width, lenght
 

}
public void mouseClicked(){
  //New game button
  if (pagestate == 1 && mouseX>70 && mouseX <730 && mouseY >310 && mouseY < 460) {
    pagestate = 2;
    pagestate_change(2);
  } 
  //next button Dia 2
  else if (pagestate == 2 && mouseX>55 && mouseX < 1180 && mouseY > 1110 && mouseY < 1244) {
    pagestate =4;
    pagestate_change(4);}
    
    
  //Minus button should decrease number in system
  else if (pagestate == 2 && mouseX>280 && mouseX < 340 && mouseY > 450 && mouseY < 510) {
  //plus button should increase number in system
  } 
  
  
  else if (pagestate == 2 && mouseX>460 && mouseX < 520 && mouseY > 450 && mouseY < 510) {
  }
  
  
  //next button Dia 4
  else if (pagestate == 4 && mouseX>280 && mouseX < 530 && mouseY > 1060 && mouseY < 1190) {
    pagestate =12;
    pagestate_change(12);}
    
    
  //Question mark
   else if (pagestate == 4 && mouseX>520 && mouseX < 620 && mouseY > 700 && mouseY < 800) {
    pagestate = 5;
    pagestate_change(5);   
  }
  
  
  //rewind logo for seeing the video again
   else if (pagestate == 4 && mouseX>160 && mouseX <350 && mouseY > 670 && mouseY < 840) {
  }
  
  
  //going through the pages after the question button
  else if (pagestate == 5 || pagestate == 6 ||pagestate == 7 ||pagestate == 8||pagestate == 9||pagestate == 10) {
    if (mouseX>50 && mouseX < 300 && mouseY > 1110 && mouseY < 1250) {
      pagestate += 1;
    }
  }
  
  
  
  //For the word back and exit the rules
  else if (pagestate == 5 || pagestate == 6 ||pagestate == 7 ||pagestate == 8||pagestate == 9||pagestate == 10||pagestate == 11) {
    if (mouseX>500 && mouseX < 750 && mouseY > 1110 && mouseY < 1250) {
     pagestate = 4;
     pagestate_change(4);}
    }
    
    
  //info button on dia 12
  else if (pagestate == 12 && mouseX>650 && mouseX < 750 && mouseY > 1130 && mouseY < 1230) {
    pagestate =13;
    pagestate_change(13);
    }
    
    
  //next button
    //else if (pagestate == 12 && mouseX>50 && mouseX < 300 && mouseY > 1120 && mouseY < 1240) {
    //pagestate = 17;
    //pagestate_change(17);
    //}
    //Ai advice cloud
    else if (pagestate == 12 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
    pagestate = 16;
    pagestate_change(16);
    }
    //to go back to dia 12 so to go back to before you clicked ai advice
    else if (pagestate == 16 && !( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
    pagestate =12;
    pagestate_change(12);}
    
    //question mark dia 12
    //else if (pagestate == 12 && mouseX > 650 && mouseX < 750 && mouseY > 1130 && mouseY < 1230) {
   // pagestate =13;
   // pagestate_change(13);}
    
    //info button on dia 12
    else if (pagestate == 12 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 13;
    pagestate_change(13);
    }
    
    //next dia 13
    else if (pagestate == 13 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 14;
    pagestate_change(14);
    }
    
    //next dia 14
    else if (pagestate == 14 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 15;
    pagestate_change(15);
    }
    
    //back button dia 15
    else if (pagestate == 13 || pagestate == 14 ||pagestate == 15) {
    if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
     pagestate = 12;
     pagestate_change(12);}
    }
    //skip waiting page for now
    else if (pagestate == 12 && mouseX>50 && mouseX < 300 && mouseY > 1120 && mouseY < 1240) {
    pagestate = 18;
    pagestate_change(18);
    }
    
    
    //Ai advice cloud purple dia 18
    else if (pagestate == 18 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
    pagestate = 19;
    pagestate_change(19);
    }
    
    //to go back to dia 18 so to go back to before you clicked ai advice
    else if (pagestate == 19 && !( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
    pagestate =18;
    pagestate_change(18);}
    
    //info button on dia 18
    else if (pagestate == 18 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 20;
    pagestate_change(20);
    }
    
    //next dia 
    else if (pagestate == 20 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 21;
    pagestate_change(21);
    }
    
    //back button dia 18
    else if (pagestate == 20 || pagestate == 21) {
    if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
     pagestate = 18;
     pagestate_change(18);}
    }
    
    ////next dia 22
    //else if (pagestate == 18 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    //pagestate = 22;
    //pagestate_change(22);
    //}
    
    //skip dia 22 for now
    else if (pagestate == 18 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 23;
    pagestate_change(23);
    }
    
    
    
    //Ai advice cloud blue dia23
    else if (pagestate == 23 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
    pagestate = 25;
    pagestate_change(25);
    }
    
    //to go back to dia 23 so to go back to before you clicked ai advice
    else if (pagestate == 25 && !( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
    pagestate =23;
    pagestate_change(23);}
    
    //info button on dia 23
    else if (pagestate == 23 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 24;
    pagestate_change(24);
    }
    
    
    //back button dia 24
    else if (pagestate == 24 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240){
    pagestate = 23;
    pagestate_change(23);
    }
    
    ////next dia 26
    //else if (pagestate == 23 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    //pagestate = 26;
    //pagestate_change(26);
    //}
    
    //skip dia 26 for now
    else if (pagestate == 23 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 27;
    pagestate_change(27);
    }
    
    //info button on dia 27
    else if (pagestate == 27 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 28;
    pagestate_change(28);
    }
    
    
    //back button dia 28
    else if (pagestate == 28 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240){
    pagestate = 27;
    pagestate_change(27);
    }
    
    ////next dia 28
    //else if (pagestate == 28 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    //pagestate = 31;
    //pagestate_change(31);
    //}
    
    //next button dia 27
    else if (pagestate == 27 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 29;
    pagestate_change(29);
    }
    
    //next button 29
    else if (pagestate == 29 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 32;
    pagestate_change(32);
    }
    
    //info button on dia 29
    else if (pagestate == 29 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 30;
    pagestate_change(30);
    }
    
    //back button dia 30
    else if (pagestate == 30 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240){
    pagestate = 29;
    pagestate_change(29);
    }
    
    
    
    
   
    //info button on dia 32
    else if (pagestate == 32 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 33;
    pagestate_change(33);
    }
    
    //next dia 33
    else if (pagestate == 33 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 34;
    pagestate_change(34);
    }
    
    //back button dia 34
    else if (pagestate == 33 || pagestate == 34) {
    if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
     pagestate = 32;
     pagestate_change(32);}
    }
    
    ////next dia 32
    //else if (pagestate == 32 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    //pagestate = 35;
    //pagestate_change(35);
    //}
    
    //skip dia 22 for now
    else if (pagestate == 32 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 36;
    pagestate_change(36);
    }
    
    //go back to beginning
    else if (pagestate == 36 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 12;
    pagestate_change(12);
    }
    
    
}
  
public void pagestate_change(int pagestate) {
  if(pagestate == 1){
  image(Dia1,0,0);
  }
  if(pagestate == 2){
  image(Dia2,0,0);
  }
  if(pagestate == 3){
  image(Dia3,0,0);
  }
  if(pagestate == 4){
  image(Dia4,0,0);
  }
  if(pagestate == 5){
  image(Dia5,0,0);
  }
  if(pagestate == 6){
  image(Dia6,0,0);
  }
  if(pagestate == 7){
  image(Dia7,0,0);
  }
  if(pagestate == 8){
  image(Dia8,0,0);
  }
  if(pagestate == 9){
  image(Dia9,0,0);
  }
  if(pagestate == 10){
  image(Dia10,0,0);
  }
  if(pagestate == 11){
  image(Dia11,0,0);
  }
  if(pagestate == 12){
  image(Dia12,0,0);
  }
  if(pagestate == 13){
  image(Dia13,0,0);
  }
  if(pagestate == 14){
  image(Dia14,0,0);
  }
  if(pagestate == 15){
  image(Dia15,0,0);
  }
  if(pagestate == 16){
  image(Dia16,0,0);
  }
  if(pagestate == 17){
  image(Dia17,0,0);
  }
  if(pagestate == 18){
  image(Dia18,0,0);
  }
  if(pagestate == 19){
  image(Dia19,0,0);
  }
  if(pagestate == 20){
  image(Dia20,0,0);
  }
  if(pagestate == 21){
  image(Dia21,0,0);
  }
  if(pagestate == 22){
  image(Dia22,0,0);
  }
  if(pagestate == 23){
  image(Dia23,0,0);
  }
  if(pagestate == 24){
  image(Dia24,0,0);
  }
  if(pagestate == 25){
  image(Dia25,0,0);
  }
  if(pagestate == 26){
  image(Dia26,0,0);
  }
  if(pagestate == 27){
  image(Dia27,0,0);
  }
  if(pagestate == 28){
  image(Dia28,0,0);
  }
  if(pagestate == 29){
  image(Dia29,0,0);
  }
  if(pagestate == 30){
  image(Dia30,0,0);
  }
  if(pagestate == 31){
  image(Dia31,0,0);
  }
  if(pagestate == 32){
  image(Dia32,0,0);
  }
  if(pagestate == 33){
  image(Dia33,0,0);
  }
  if(pagestate == 34){
  image(Dia34,0,0);
  }
  if(pagestate == 35){
  image(Dia35,0,0);
  }
  if(pagestate == 36){
  image(Dia36,0,0);
  }
  if(pagestate == 37){
  image(Dia37,0,0);
  }
}
  public void settings() {  size (800, 1280); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "--present", "--window-color=#666666", "--hide-stop", "Supply_Chain" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
