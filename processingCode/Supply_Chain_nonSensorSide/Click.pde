void mouseClicked(){
  
  //Dia 1 buttons
  //New game button
  if (pagestate == 1 && mouseX>70 && mouseX <730 && mouseY >310 && mouseY < 460) {
    pagestate = 2;
  }
  
  
  //Dia 2 buttons
  //next button
  else if (pagestate == 2 && mouseX>55 && mouseX < 1180 && mouseY > 1110 && mouseY < 1244) {
    pagestate = 4;
  }
  //Minus button should decrease number in system
  else if (pagestate == 2 && mouseX>280 && mouseX < 340 && mouseY > 450 && mouseY < 510) {
  
  } 
  //plus button should increase number in system
  else if (pagestate == 2 && mouseX>460 && mouseX < 520 && mouseY > 450 && mouseY < 510) {
  }
  
  
  // Dia 4 buttons
  //next button
  else if (pagestate == 4 && mouseX>280 && mouseX < 530 && mouseY > 1060 && mouseY < 1190) {
    pagestate =12;
  }  
  //Question mark
   else if (pagestate == 4 && mouseX>520 && mouseX < 620 && mouseY > 700 && mouseY < 800) {
    pagestate = 5;  
  }
  //rewind logo for seeing the video again
   else if (pagestate == 4 && mouseX>160 && mouseX <350 && mouseY > 670 && mouseY < 840) {
  }
  
  //Dias 5-11
  //next buttons - going through the pages after the question button
  else if (pagestate == 5 || pagestate == 6 ||pagestate == 7 ||pagestate == 8||pagestate == 9||pagestate == 10) {
    if (mouseX>50 && mouseX < 300 && mouseY > 1110 && mouseY < 1250) {
      pagestate += 1;
    }
  }
  //back buttons and exit the rules
  else if (pagestate == 5 || pagestate == 6 ||pagestate == 7 ||pagestate == 8||pagestate == 9||pagestate == 10||pagestate == 11) {
    if (mouseX>500 && mouseX < 750 && mouseY > 1110 && mouseY < 1250) {
     pagestate = 4;
     }
  }

  //SUPPLY PHASE
  //Dia 12
  //skip waiting page for now (Dia 17), go to next phase (Dia 18)
  else if (pagestate == 12 && mouseX>50 && mouseX < 300 && mouseY > 1120 && mouseY < 1240) {
  pagestate = 18;
  sendWaitingStatus();
  }  
  //info button on dia 12
  else if (pagestate == 12 && mouseX>650 && mouseX < 750 && mouseY > 1130 && mouseY < 1230) {
    pagestate = 13;
  }
  //Ai advice cloud
  else if (pagestate == 12 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
  pagestate = 16;
  }
  
  //Dia 16
  //go back to Dia 12 by clicking outside of bubble
  else if (pagestate == 16 && !( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
  pagestate =12;
  }
    
  //Dia 13
  //next button
  else if (pagestate == 13 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 14;
  }
    
  //Dia 14
  //next button
  else if (pagestate == 14 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 15;
  }
    
  //Dias 13-15
  //back button
  else if (pagestate == 13 || pagestate == 14 ||pagestate == 15) {
  if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
   pagestate = 12;
    }
  }
  
  //Dia 17
  //next button to Dia 18
  else if (pagestate == 17 && mouseX>50 && mouseX < 300 && mouseY > 1120 && mouseY < 1240) {
  checkWaiting();
  }
  
    
  //ASSEMBLY
  //Dia 18
  //skip dia 22 for now -> Dia 23
  else if (pagestate == 18 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 23; //change to 22 for final version
  sendWaitingStatus();
  }
  //info button
  else if (pagestate == 18 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
  pagestate = 20;
  }
  //Ai advice cloud
  else if (pagestate == 18 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
  pagestate = 19;
  }
  
  //Dia 19
  //go back to Dia 18 by clicking outside of bubble
  else if (pagestate == 19 && !( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
  pagestate =18;
  }
  
  //Dia 20
  //next button
  else if (pagestate == 20 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 21;
  }
  
  //Dia 20-21
  //back button -> Dia 18
  else if (pagestate == 20 || pagestate == 21) {
  if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
   pagestate = 18;
    }
  }
  
  //Dia 22
  ////next button -> Dia 23
  else if (pagestate == 22 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  checkWaiting();
  } 
    
  //LOGISTICS  
  //Dia 23
  //next button -> Dia 27
  else if (pagestate == 23 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 27;
    sendWaitingStatus();
  }
  //Ai advice cloud
  else if (pagestate == 23 && mouseX>40 && mouseX < 300 && mouseY > 950 && mouseY < 1090) {
  pagestate = 25;
  }
  
  //Dia 25
  //go back to Dia 23 by clicking outside of bubble
  else if (pagestate == 25 && !( mouseX>40 && mouseX < 650 && mouseY > 800 && mouseY < 1080)) {
  pagestate =23;
  }
  
  //Dia 23
  //info button
  else if (pagestate == 23 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
  pagestate = 24;
  }
  
  //Dia 24
  //back button
  else if (pagestate == 24 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240){
  pagestate = 23;
  }
  
  //Dia 23
  ////next dia 26
  else if (pagestate == 26 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    checkWaiting();
  }

  //TRANSPORT
  //Dia 27
  //next button
  else if (pagestate == 27 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 29;
  }
  //info button
  else if (pagestate == 27 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
  pagestate = 28;
  }
  
  //Dia 28
  //back button
  else if (pagestate == 28 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240){
  pagestate = 27;
  }
  
  //Dia 29
  //next button -> 32 (change to 31 when waiting for opponent function written)
  else if (pagestate == 29 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 32;
  sendWaitingStatus()
  }
  //info button
  else if (pagestate == 29 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
  pagestate = 30;
  }
  
  //Dia 30
  //back button
  else if (pagestate == 30 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240){
  pagestate = 29;
  }
  
  //Dia 31
  else if (pagestate == 31 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    checkWaiting();
  }

  //DEMAND
  //Dia 32
  //next button skip dia 35 for now -> 36
  else if (pagestate == 32 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 36;
    sendWaitingStatus();
  }
  //info button
  else if (pagestate == 32 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 33;
  }
  
  //Dia 33
  //next button
  else if (pagestate == 33 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 34;
  }
  
  //Dias 33-34
  //back button
  else if (pagestate == 33 || pagestate == 34) {
    if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
      pagestate = 32;
    }
  }
  
  //Dia 35
  ////next button
  else if (pagestate == 35 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    sendScore();
    checkWaiting();
  }
  
  //Dia 36
  //go back to beginning
  else if (pagestate == 36 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    if(roundNo <= roundLim){
      pagestate = 12;
      roundNo = roundNo + 1;
    }
    else{
      //display game over and return to homescreen
      pagestate = 1;
    }
  }
  
  //Serial messages to arduino for arrow displays
  if (pagestate >=1 && pagestate <= 17){
    myArduinoPort.write(createArduinoPacket(NONE,NONE));
  }
  else if (pagestate >=18 && pagestate <= 22){
    myArduinoPort.write(createArduinoPacket(ASSEMBLY,NONE));
  }
  else if (pagestate >=23 && pagestate <= 26){
    myArduinoPort.write(createArduinoPacket(LOGISTICS,NONE));
  }
  else if (pagestate >=27 && pagestate <= 28){
    myArduinoPort.write(createArduinoPacket(TRANSPORT1,NONE));
  }
  else if (pagestate >=29 && pagestate <= 31){
    myArduinoPort.write(createArduinoPacket(TRANSPORT2,NONE));
  }
  else if (pagestate >=32 && pagestate <= 35){
    myArduinoPort.write(createArduinoPacket(DEMAND,NONE));
  }
  else if (pagestate == 36){//score query
    myArduinoPort.write(createArduinoPacket(NONE,"1"));
  }
}
  
