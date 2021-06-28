void mouseClicked(){
//Dia 0 buttons
  //New game button
  if (pagestate == 0 && mouseX>70 && mouseX <730 && mouseY >310 && mouseY < 460) {
    pagestate = 1;
    newGameSetup();
    InstructionsSeen = false;
    SetupSeen = false;
  }
  
  
  //Dia 1 and 2 buttons
  else if ((pagestate==1||pagestate==2)) {
    if (mouseX>55 && mouseX < 290 && mouseY > 1110 && mouseY < 1244){
      pagestate = 3;
    }
    key_pressed();
  }
  
  
  // Dia 3 buttons
  //next button
  else if (pagestate == 3 && mouseX>240 && mouseX < 560 && mouseY > 460 && mouseY < 550) {
    pagestate = -1;
  }
  
  else if(pagestate == 3 && mouseX>240 && mouseX < 560 && mouseY > 750 && mouseY < 840) {
    pagestate = -2;
  }
  
    //Instructions buttons
  
  else if ((pagestate == -1 || pagestate == -3 || pagestate == -5 || pagestate == -7 || pagestate == -9 || pagestate == -11 || pagestate == -13) && mouseX> 50 && mouseX < 290 && mouseY > 1145 && mouseY < 1215){
    pagestate = pagestate - 2;
  }
  else if ((pagestate == -1 || pagestate == -3 || pagestate == -5 || pagestate == -7 || pagestate == -9 || pagestate == -11 || pagestate == -13) && mouseX> 510 && mouseX < 745 && mouseY > 1145 && mouseY < 1215){
    println("if statement 1 entered");
    pagestate = 3;
    InstructionsSeen = true;
  }
  else if (pagestate == -15 && ((mouseX> 50 && mouseX < 290 && mouseY > 1145 && mouseY < 1215) || (mouseX> 510 && mouseX < 745 && mouseY > 1145 && mouseY < 1215))){
    println("if statement 2 entered");
    pagestate = 3;
    InstructionsSeen = true;
  }
  
    //Board Setup buttons
   else if ((pagestate == -2 || pagestate == -4 || pagestate == -6 || pagestate == -8 || pagestate == -10 || pagestate == -12 || pagestate == -14) && mouseX> 50 && mouseX < 290 && mouseY > 1145 && mouseY < 1215){
     pagestate = pagestate - 2;
  }
  else if ((pagestate == -2 || pagestate == -4 || pagestate == -6 || pagestate == -8 || pagestate == -10 || pagestate == -12 || pagestate == -14) && mouseX> 510 && mouseX < 745 && mouseY > 1145 && mouseY < 1215){
    pagestate = 3;
    println("if statement 3 entered");
    SetupSeen = true;
  }
  else if (pagestate == -16 && ((mouseX> 50 && mouseX < 290 && mouseY > 1145 && mouseY < 1215) || (mouseX> 510 && mouseX < 745 && mouseY > 1145 && mouseY < 1215))){
    pagestate = 3;
    SetupSeen = true;
    println("if statement 4 entered");
  }
  
  //dia 3
  else if (InstructionsSeen && SetupSeen && pagestate == 3 && mouseX>55 && mouseX < 290 && mouseY > 1145 && mouseY < 1215) {
    myArduinoPort.write(createArduinoPacket(NONE,NONE));
    pagestate = 12;
  }

  //SUPPLY PHASE
  //Dia 12
  //next button
  else if (pagestate == 12 && mouseX>50 && mouseX < 300 && mouseY > 1120 && mouseY < 1240) {
    pagestate = 16;
    myRPiPort.write(createRPiPacket(WAITING, NONE));
  }  
  //info button on dia 12
  else if (pagestate == 12 && mouseX>650 && mouseX < 750 && mouseY > 1130 && mouseY < 1230) {
    pagestate = 13;
  }

  //Dias 13-15
  //back button
  else if ((pagestate == 13 || pagestate == 14 ||pagestate == 15) && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
      pagestate = 12;
  }
  else if ((pagestate == 13 || pagestate == 14) && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
      pagestate = pagestate + 1;
  }

  //ASSEMBLY
  //Dia 17
  //next button to Dia 20
  else if (pagestate == 17 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240) {
    pagestate = 20;
    myRPiPort.write(createRPiPacket(WAITING, NONE));
  }
  //info button
  else if (pagestate == 17 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 18;
  }

  //Dia 18
  //next button
  else if (pagestate == 18 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240) {
    pagestate = 19;
  }

  //Dia 18-19
  //back button -> Dia 17
  else if (pagestate == 18 || pagestate == 19) {
    if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
      pagestate = 17;
    }
  }


  //LOGISTICS  
  //Dia 21
  //next button -> Dia 23
  else if (pagestate == 21 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240) {
    pagestate = 23;
    myRPiPort.write(createRPiPacket(WAITING, NONE));
  }

  //Dia 21
  //info button
  else if (pagestate == 21 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 22;
  }

  //Dia 22
  //back button
  else if (pagestate == 22 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
    pagestate = 21;
  }

  //TRANSPORT
  //Dia 24
  //next button
  else if (pagestate == 24 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 26;
  myArduinoPort.write(createArduinoPacket(TRANSPORT2,NONE));
  }
   //info button
  else if (pagestate == 24 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 25;
  }

  //Dia 25
  //back button
  else if (pagestate == 25 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
    pagestate = 24;
  }
  
  //Dia 26
  //next button -> 32 (change to 31 when waiting for opponent function written)
  else if (pagestate == 26 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
  pagestate = 28;
  sendWaitingStatus();
  }
  //info button
  else if (pagestate == 26 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 27;
  }

  //Dia 30
  //back button
  else if (pagestate == 27 && mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
    pagestate = 26;
  }

  //DEMAND
  //Dia 30
  else if (pagestate == 30 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    myArduinoPort.write(createArduinoPacket(DEMAND,"1")); //ask for score before proceeding to dia36
    waitForArduinoData();
    myRPiPort.write(createRPiPacket(WAITING, myScore)); //send score and waiting status together
    pagestate = 33;
  }
  //info button
  else if (pagestate == 30 && mouseX>650 && mouseX <750 && mouseY >1130 && mouseY < 1230) {
    pagestate = 31;
  }
  
  //Dia 31
  //next button
  else if (pagestate == 31 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    pagestate = 32;
  }
  
  //Dias 31-32
  //back button
  else if (pagestate == 31 || pagestate == 32) {
    if (mouseX>510 && mouseX < 750 && mouseY > 1120 && mouseY < 1240) {
      pagestate = 30;
    }
  }
  
  //Dia 34
  //go back to beginning
  else if (pagestate == 34 && mouseX>55 && mouseX <290 && mouseY >1120 && mouseY < 1240){
    if(roundNo <= roundLim){
      pagestate = 12;
      roundNo = roundNo + 1;
      myArduinoPort.write(createArduinoPacket(NONE,NONE));
    }
    else{
      //save score and return to homescreen
      pagestate = 36;
    }
  }
  
  else if (pagestate == 35){
    if(mouseX > 110 && mouseX < 360 && mouseY > 666 && mouseY < 815){
      pagestate = 0;
      myScore = "0";
      name = "";
    }
    else if(mouseX > 440 && mouseX < 690 && mouseY > 666 && mouseY < 815){
      pagestate = lastPageState;
    }
  }
  
  else if (pagestate == 36){
    if(mouseX > 55 && mouseX < 295 && mouseY > 1145 && mouseY < 1215){
      pagestate = 0;
      addHighScore(name, Integer.parseInt(myScore));
      saveHighScore();
      name="";
      myScore="0";
    }
  }
  
  else if ((pagestate >=12 && pagestate <= 34) || pagestate == 36){
    if (mouseX >= 750 && mouseX <= 800 && mouseY >=0 && mouseY <= 50){
      pagestate = 35;
    }
  }
  
  //change to correct pagestate
  pagestate_change(pagestate);
}
void mousePressed() {
  mousePress = true;
  startTimePressed = millis();
}
void mouseReleased() {
  mousePress = false;
}
