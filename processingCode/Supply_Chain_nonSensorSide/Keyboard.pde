//data variables
String name;

//y pixel ranges
int y_1row_top = 815;
int y_1row_bot = 875;
int y_2row_top = 885;
int y_2row_bot = 945;
int y_3row_top = 955;
int y_3row_bot = 1015;
int y_4row_top = 1025;
int y_4row_bot = 1085;

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
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key d
  else if(mouseX>180 && mouseX < 240 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key e
  if(mouseX>150 && mouseX < 210 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key f
  else if(mouseX>250 && mouseX < 310 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key g
  else if(mouseX>325 && mouseX <385 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key h
  if(mouseX>395 && mouseX < 455 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key i
  else if(mouseX>510 && mouseX < 570 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key j
  if(mouseX>467 && mouseX < 527 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key k
  else if(mouseX>540 && mouseX <600 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key l
  else if(mouseX>610 && mouseX < 670 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key m
  else if(mouseX>500 && mouseX <560 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key n
  else if(mouseX>430 && mouseX < 490 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key o
  else if(mouseX>585 && mouseX <645 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key p
  else if(mouseX>657 && mouseX < 717 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key q
  else if(mouseX>5 && mouseX <65 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key r
  else if(mouseX<222 && mouseX < 282 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key s
  else if(mouseX>108 && mouseX <168 && mouseY > y_2row_bot && mouseY < y_2row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key t
  else if(mouseX>295 && mouseX < 365 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key u
  else if(mouseX>440 && mouseX <500 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key v
  else if(mouseX>290 && mouseX < 350 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key w
  else if(mouseX>77 && mouseX <137 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key x
  else if(mouseX>147 && mouseX < 207 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
    }
  }
  //key y
  else if(mouseX>367 && mouseX <427 && mouseY > y_1row_bot && mouseY < y_1row_top){
    if(pagestate == 2){
      name+="A";
    }
    else if(pagestate == 3){
      name+="a";
    }
  }
  
  //key z
  else if(mouseX>77 && mouseX < 137 && mouseY > y_3row_bot && mouseY < y_3row_top){
    if(pagestate == 2){
      name+="B";
    }
    else if(pagestate == 3){
      name+="b";
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
