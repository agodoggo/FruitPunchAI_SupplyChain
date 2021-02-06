//data variables
String name = "";

//y pixel ranges
int y_1row_bot = 815;
int y_1row_top = 875;
int y_2row_bot = 885;
int y_2row_top = 945;
int y_3row_bot = 955;
int y_3row_top = 1015;
int y_4row_bot = 1025;
int y_4row_top = 1085;

public void key_pressed(){
  
  //letter keys
  //key a
  println("key press conditions: (" + mouseX +", " + mouseY);
  if(mouseX>5 && mouseX <65 && mouseY >y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key b
  else if(mouseX>360 && mouseX < 420 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key c
  else if(mouseX>220 && mouseX <280 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="C";
    }
    else if(pagestate == 3){
      name+="c";
    }
  }
  
  //key d
  else if(mouseX>180 && mouseX < 240 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="D";
    }
    else if(pagestate == 3){
      name+="d";
    }
  }
  //key e
  if(mouseX>150 && mouseX < 210 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="E";
    }
    else if(pagestate == 3){
      name+="e";
    }
  }
  
  //key f
  else if(mouseX>250 && mouseX < 310 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="F";
    }
    else if(pagestate == 3){
      name+="f";
    }
  }
  //key g
  else if(mouseX>325 && mouseX <385 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="G";
    }
    else if(pagestate == 3){
      name+="g";
    }
  }
  
  //key h
  if(mouseX>395 && mouseX < 455 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="H";
    }
    else if(pagestate == 3){
      name+="h";
    }
  }
  //key i
  else if(mouseX>510 && mouseX < 570 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="I";
    }
    else if(pagestate == 3){
      name+="i";
    }
  }
  
  //key j
  if(mouseX>467 && mouseX < 527 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="J";
    }
    else if(pagestate == 3){
      name+="j";
    }
  }
  //key k
  else if(mouseX>540 && mouseX <600 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="K";
    }
    else if(pagestate == 3){
      name+="k";
    }
  }
  
  //key l
  else if(mouseX>610 && mouseX < 670 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="L";
    }
    else if(pagestate == 3){
      name+="l";
    }
  }
  //key m
  else if(mouseX>500 && mouseX <560 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="M";
    }
    else if(pagestate == 3){
      name+="m";
    }
  }
  
  //key n
  else if(mouseX>430 && mouseX < 490 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="N";
    }
    else if(pagestate == 3){
      name+="n";
    }
  }
  //key o
  else if(mouseX>585 && mouseX <645 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="O";
    }
    else if(pagestate == 3){
      name+="o";
    }
  }
  
  //key p
  else if(mouseX>657 && mouseX < 717 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="P";
    }
    else if(pagestate == 3){
      name+="p";
    }
  }
  //key q
  else if(mouseX>5 && mouseX <65 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="Q";
    }
    else if(pagestate == 3){
      name+="q";
    }
  }
  
  //key r
  else if(mouseX<222 && mouseX < 282 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="R";
    }
    else if(pagestate == 3){
      name+="r";
    }
  }
  //key s
  else if(mouseX>108 && mouseX <168 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="S";
    }
    else if(pagestate == 3){
      name+="s";
    }
  }
  
  //key t
  else if(mouseX>295 && mouseX < 365 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="T";
    }
    else if(pagestate == 3){
      name+="t";
    }
  }
  //key u
  else if(mouseX>440 && mouseX <500 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="U";
    }
    else if(pagestate == 3){
      name+="u";
    }
  }
  
  //key v
  else if(mouseX>290 && mouseX < 350 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="V";
    }
    else if(pagestate == 3){
      name+="v";
    }
  }
  //key w
  else if(mouseX>77 && mouseX <137 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="W";
    }
    else if(pagestate == 3){
      name+="w";
    }
  }
  
  //key x
  else if(mouseX>147 && mouseX < 207 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="X";
    }
    else if(pagestate == 3){
      name+="x";
    }
  }
  //key y
  else if(mouseX>367 && mouseX <427 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="Y";
    }
    else if(pagestate == 3){
      name+="y";
    }
  }
  
  //key z
  else if(mouseX>77 && mouseX < 137 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="Z";
    }
    else if(pagestate == 3){
      name+="z";
    }
  }
  
  //misc keys
  //shift left
  else if(mouseX>5 && mouseX < 65 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      pagestate=3;
    }
    else if(pagestate == 3){
      pagestate=2;
    }
  }
  //shift right
  else if(mouseX>645 && mouseX < 705 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      pagestate=3;
    }
    else if(pagestate == 3){
      pagestate=2;
    }
  }
  //hyphen
  else if(mouseX>573 && mouseX < 631 && mouseY > y_3row_bot && mouseY < y_3row_top){
    name+="-";
  }
  //return
  else if(mouseX>683 && mouseX < 793 && mouseY > y_2row_bot && mouseY < y_2row_top){
    name+="\n";
  }
  //backspace
  else if(mouseX>730 && mouseX < 793 && mouseY > y_1row_bot && mouseY < y_1row_top){
      name = name.substring(0,name.length()-1);
  }
  //space
  else if(mouseX>218 && mouseX < 631 && mouseY > y_4row_bot && mouseY < y_4row_top){
    name+=" ";
  }
}
