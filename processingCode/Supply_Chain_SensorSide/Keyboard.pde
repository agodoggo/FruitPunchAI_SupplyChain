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
  println("key pressed");
  if(pagestate == 1 || pagestate ==2 && mouseX>35 && mouseX <95 && mouseY >y_2row_bot && mouseY < y_2row_top){
    println("if statement entered");
    if(pagestate == 1){
      name+="A";
    }
    else if(pagestate == 2){
      name+="a";
    }
  }
  
  //key b
  else if(pagestate == 1 || pagestate ==2 && mouseX>360 && mouseX < 420 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="B";
    }
    else if(pagestate == 2){
      name+="b";
    }
  }
  //key c
  else if(pagestate == 1 || pagestate ==2 && mouseX>220 && mouseX <280 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="C";
    }
    else if(pagestate == 2){
      name+="c";
    }
  }
  
  //key d
  else if(pagestate == 1 || pagestate ==2 && mouseX>180 && mouseX < 240 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="D";
    }
    else if(pagestate == 2){
      name+="d";
    }
  }
  //key e
  if(pagestate == 1 || pagestate ==2 && mouseX>150 && mouseX < 210 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="E";
    }
    else if(pagestate == 2){
      name+="e";
    }
  }
  
  //key f
  else if(pagestate == 1 || pagestate ==2 && mouseX>250 && mouseX < 310 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="F";
    }
    else if(pagestate == 2){
      name+="f";
    }
  }
  //key g
  else if(pagestate == 1 || pagestate ==2 && mouseX>325 && mouseX <385 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="G";
    }
    else if(pagestate == 2){
      name+="g";
    }
  }
  
  //key h
  if(pagestate == 1 || pagestate ==2 && mouseX>395 && mouseX < 455 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="H";
    }
    else if(pagestate == 2){
      name+="h";
    }
  }
  //key i
  else if(pagestate == 1 || pagestate ==2 && mouseX>510 && mouseX < 570 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="I";
    }
    else if(pagestate == 2){
      name+="i";
    }
  }
  
  //key j
  if(pagestate == 1 || pagestate ==2 && mouseX>467 && mouseX < 527 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="J";
    }
    else if(pagestate == 2){
      name+="j";
    }
  }
  //key k
  else if(pagestate == 1 || pagestate ==2 && mouseX>540 && mouseX <600 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="K";
    }
    else if(pagestate == 2){
      name+="k";
    }
  }
  
  //key l
  else if(pagestate == 1 || pagestate ==2 && mouseX>610 && mouseX < 670 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="L";
    }
    else if(pagestate == 2){
      name+="l";
    }
  }
  //key m
  else if(pagestate == 1 || pagestate ==2 && mouseX>500 && mouseX <560 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="M";
    }
    else if(pagestate == 2){
      name+="m";
    }
  }
  
  //key n
  else if(pagestate == 1 || pagestate ==2 && mouseX>430 && mouseX < 490 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="N";
    }
    else if(pagestate == 2){
      name+="n";
    }
  }
  //key o
  else if(pagestate == 1 || pagestate ==2 && mouseX>585 && mouseX <645 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="O";
    }
    else if(pagestate == 2){
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
  else if(pagestate == 1 || pagestate ==2 && mouseX>5 && mouseX <65 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="Q";
    }
    else if(pagestate == 2){
      name+="q";
    }
  }
  
  //key r
  else if(pagestate == 1 || pagestate ==2 && mouseX>222 && mouseX < 282 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="R";
    }
    else if(pagestate == 2){
      name+="r";
    }
  }
  //key s
  else if(pagestate == 1 || pagestate ==2 && mouseX>108 && mouseX <168 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 1){
      name+="S";
    }
    else if(pagestate == 2){
      name+="s";
    }
  }
  
  //key t
  else if(pagestate == 1 || pagestate ==2 && mouseX>295 && mouseX < 365 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="T";
    }
    else if(pagestate == 2){
      name+="t";
    }
  }
  //key u
  else if(pagestate == 1 || pagestate ==2 && mouseX>440 && mouseX <500 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="U";
    }
    else if(pagestate == 2){
      name+="u";
    }
  }
  
  //key v
  else if(pagestate == 1 || pagestate ==2 && mouseX>290 && mouseX < 350 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="V";
    }
    else if(pagestate == 2){
      name+="v";
    }
  }
  //key w
  else if(pagestate == 1 || pagestate ==2 && mouseX>77 && mouseX <137 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="W";
    }
    else if(pagestate == 2){
      name+="w";
    }
  }
  
  //key x
  else if(pagestate == 1 || pagestate ==2 && mouseX>147 && mouseX < 207 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="X";
    }
    else if(pagestate == 2){
      name+="x";
    }
  }
  //key y
  else if(pagestate == 1 || pagestate ==2 && mouseX>367 && mouseX <427 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 1){
      name+="Y";
    }
    else if(pagestate == 2){
      name+="y";
    }
  }
  
  //key z
  else if(pagestate == 1 || pagestate ==2 && mouseX>77 && mouseX < 137 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      name+="Z";
    }
    else if(pagestate == 2){
      name+="z";
    }
  }
  
  //misc keys
  //shift left
  else if(pagestate == 1 || pagestate ==2 && mouseX>5 && mouseX < 65 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      pagestate=2;
    }
    else if(pagestate == 2){
      pagestate=1;
    }
  }
  //shift right
  else if(pagestate == 1 || pagestate ==2 && mouseX>645 && mouseX < 705 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 1){
      pagestate=2;
    }
    else if(pagestate == 2){
      pagestate=1;
    }
  }
  //hyphen
  else if(pagestate == 1 || pagestate ==2 && mouseX>573 && mouseX < 631 && mouseY > y_3row_bot && mouseY < y_3row_top){
    name+="-";
  }
  //return
  else if(pagestate == 1 || pagestate ==2 && mouseX>683 && mouseX < 793 && mouseY > y_2row_bot && mouseY < y_2row_top){
    name+="\n";
  }
  //backspace
  else if(pagestate == 1 || pagestate ==2 && mouseX>730 && mouseX < 793 && mouseY > y_1row_bot && mouseY < y_1row_top){
      name = name.substring(0,name.length()-1);
  }
  //space
  else if(pagestate == 1 || pagestate ==2 && mouseX>218 && mouseX < 631 && mouseY > y_4row_bot && mouseY < y_4row_top){
    name+=" ";
  }
}
