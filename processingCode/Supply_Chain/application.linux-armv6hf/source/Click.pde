void mouseClicked(){
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
  
