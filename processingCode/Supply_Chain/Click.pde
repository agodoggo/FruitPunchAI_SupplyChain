void mouseClicked(){
  //New game button
  if (pagestate == 1 && mouseX>70 && mouseX <730 && mouseY >310 && mouseY < 460) {
    pagestate = 2;
    pagestate_change(2);
  } 
  //next button Dia 2
  if (pagestate == 2 && mouseX>55 && mouseX < 1180 && mouseY > 1110 && mouseY < 1244) {
    pagestate =4;
    pagestate_change(4);}
  //Minus button should decrease number in system
  if (pagestate == 2 && mouseX>280 && mouseX < 340 && mouseY > 450 && mouseY < 510) {
  //plus button should increase number in system
  } 
  if (pagestate == 2 && mouseX>460 && mouseX < 520 && mouseY > 450 && mouseY < 510) {
  }
  //next button Dia 4
  if (pagestate == 4 && mouseX>280 && mouseX < 530 && mouseY > 1060 && mouseY < 1190) {
    pagestate =12;
    pagestate_change(12);}
  //Questionmark
   if (pagestate == 4 && mouseX>520 && mouseX < 620 && mouseY > 700 && mouseY < 800) {
    pagestate = 5;
    pagestate_change(5);   
  }
  //rewind logo for seeing the video again
   if (pagestate == 4 && mouseX>160 && mouseX <350 && mouseY > 670 && mouseY < 840) {
  }
  //going through the pages after the question button
  if (pagestate == 5 || pagestate == 6 ||pagestate == 7 ||pagestate == 8||pagestate == 9||pagestate == 10) {
    if (mouseX>50 && mouseX < 300 && mouseY > 1110 && mouseY < 1250) {
      pagestate += 1;
    }
  }
  //For the word back and exit the rules
  if (pagestate == 5 || pagestate == 6 ||pagestate == 7 ||pagestate == 8||pagestate == 9||pagestate == 10||pagestate == 11) {
    if (mouseX>500 && mouseX < 750 && mouseY > 1110 && mouseY < 1250) {
     pagestate = 4;
     pagestate_change(4);}
    }
  //info button on dia 12
  if (pagestate == 12 && mouseX>650 && mouseX < 750 && mouseY > 1130 && mouseY < 1230) {
    pagestate =13;
    pagestate_change(13);}
  //next button
    if (pagestate == 12 && mouseX>50 && mouseX < 300 && mouseY > 1120 && mouseY < 1240) {
    pagestate = 17;
    pagestate_change(17);
    }
    //Ai advice cloud
    if (pagestate == 12 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
    pagestate = 16;
    pagestate_change(16);
    }
    //to go back to dia 12 so to go back to before you clicked ai advice
    if (pagestate == 16 != ( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
    pagestate =12;
    pagestate_change(12);}
    
    //question mark dia 12
    //if (pagestate == 12 && mouseX > 650 && mouseX < 750 && mouseY > 1130 && mouseY < 1230) {
   // pagestate =13;
   // pagestate_change(13);}
    
    
    if (pagestate == 12 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 13;
    pagestate_change(13);
    }
    
  
   
}
  
