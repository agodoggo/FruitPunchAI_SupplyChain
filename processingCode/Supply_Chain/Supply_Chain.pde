//Path to images, path is the data folder next to the processing sketch
PImage Dia1;PImage Dia2;PImage Dia3;PImage Dia4;PImage Dia5;PImage Dia6;PImage Dia7;PImage Dia8;PImage Dia9;PImage Dia10;
PImage Dia11;PImage Dia12;PImage Dia13;PImage Dia14;PImage Dia15;PImage Dia16;PImage Dia17;PImage Dia18;PImage Dia19;
PImage Dia20;PImage Dia21;PImage Dia22;PImage Dia23;PImage Dia24;PImage Dia25;PImage Dia26;PImage Dia27;PImage Dia28;PImage Dia29;
PImage Dia30;PImage Dia31;PImage Dia32;PImage Dia33;PImage Dia34;PImage Dia35;PImage Dia36;PImage Dia37;

//Set the pagestate at 0 to begin the program at Dia1
int pagestate=1;


void setup()
{
 //set screen size, this is the pixel ratio of the build in screens
 size (800, 1280);
 
 //preload all the images in the program, this should make the entire programm fast by not loading a picture per pagestate
 Dia1 = loadImage("Dia1.PNG");Dia2 = loadImage("Dia2.PNG");Dia3 = loadImage("Dia3.PNG");Dia4 = loadImage("Dia4.PNG");
 Dia5 = loadImage("Dia5.PNG");Dia6 = loadImage("Dia6.PNG");Dia7 = loadImage("Dia7.PNG");Dia8 = loadImage("Dia8.PNG");
 Dia9 = loadImage("Dia9.PNG");Dia10 = loadImage("Dia10.PNG");Dia11 = loadImage("Dia11.PNG");Dia12 = loadImage("Dia12.PNG");
 Dia13 = loadImage("Dia13.PNG");Dia14 = loadImage("Dia14.PNG");Dia15 = loadImage("Dia15.PNG");Dia16 = loadImage("Dia16.PNG");
 Dia17 = loadImage("Dia17.PNG");Dia18 = loadImage("Dia18.PNG");Dia19 = loadImage("Dia19.PNG");Dia20 = loadImage("Dia20.PNG");
 Dia21 = loadImage("Dia21.PNG");Dia22 = loadImage("Dia22.PNG");Dia23 = loadImage("Dia23.PNG");Dia24 = loadImage("Dia24.PNG");
 Dia25 = loadImage("Dia25.PNG");Dia26 = loadImage("Dia26.PNG");Dia27 = loadImage("Dia27.PNG");Dia28 = loadImage("Dia28.PNG");
 Dia29 = loadImage("Dia29.PNG");Dia30 = loadImage("Dia30.PNG");Dia31 = loadImage("Dia31.PNG");Dia32 = loadImage("Dia32.PNG");
 Dia33 = loadImage("Dia33.PNG");Dia34 = loadImage("Dia34.PNG");Dia35 = loadImage("Dia35.PNG");Dia36 = loadImage("Dia36.PNG");
 Dia37 = loadImage("Dia37.PNG");
}

void draw()
{
 background (0);
 pagestate_change(pagestate);
 //x,y - width, lenght
 

}
