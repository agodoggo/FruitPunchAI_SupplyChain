//git test
public void pagestate_change(int pagestate) {
  //clears out old frame
  background(0);
  //change background picture
  if(pagestate == 0){
  image(Dia0,0,0);
  displayHighScores();
  }
  else if(pagestate == 1){
  image(Dia1,0,0);
  }
  if(pagestate == 2){
  image(Dia2,0,0);
  }
  else if(pagestate == 3){
    myIntroMov.play();
    image(Dia3,0,0);
  }
  else if(pagestate == 4){
  image(Dia4,0,0);
  }
  else if(pagestate == 5){
  image(Dia5,0,0);
  }
  else if(pagestate == 6){
  image(Dia6,0,0);
  }
  else if(pagestate == 7){
  image(Dia7,0,0);
  }
  else if(pagestate == 8){
  image(Dia8,0,0);
  }
  else if(pagestate == 9){
  image(Dia9,0,0);
  }
  else if(pagestate == 10){
  image(Dia10,0,0);
  }
  else if(pagestate == 11){
  image(Dia11,0,0);
  }
  else if(pagestate == 12){
    mySetupMov0.play();
    mySetupMov1.play();
    mySetupMov2.play();
    mySetupMov3.play();
    mySetupMov4.play();
    mySetupMov5.play();
    mySetupMov6.play();
    image(Dia12,0,0);
  }
  else if(pagestate == 13){
  image(Dia13,0,0);
  }
  else if(pagestate == 14){
  image(Dia14,0,0);
  }
  else if(pagestate == 15){
  image(Dia15,0,0);
  }
  else if(pagestate == 16){
  image(Dia16,0,0);
  }
  else if(pagestate == 17){
  image(Dia17,0,0);
  }
  else if(pagestate == 18){
  image(Dia18,0,0);
  }
  else if(pagestate == 19){
  image(Dia19,0,0);
  }
  else if(pagestate == 20){
  image(Dia20,0,0);
  }
  else if(pagestate == 21){
  image(Dia21,0,0);
  }
  else if(pagestate == 22){
  image(Dia22,0,0);
  }
  else if(pagestate == 23){
  image(Dia23,0,0);
  }
  else if(pagestate == 24){
  image(Dia24,0,0);
  }
  else if(pagestate == 25){
  image(Dia25,0,0);
  }
  else if(pagestate == 26){
  image(Dia26,0,0);
  }
  else if(pagestate == 27){
  image(Dia27,0,0);
  }
  else if(pagestate == 28){
  image(Dia28,0,0);
  }
  else if(pagestate == 29){
  image(Dia29,0,0);
  }
  else if(pagestate == 30){
  image(Dia30,0,0);
  }
  else if(pagestate == 31){
  image(Dia31,0,0);
  }
  else if(pagestate == 32){
    String demandMsg = demandMsg();
    myDemandMov.play();
    textAlign(CENTER);
    fill(0);
    textSize(30);
    text(demandMsg,400,400); //display name
    delay(5000);
    image(Dia32,0,0);
     textAlign(CENTER);
    fill(0);
    textSize(30);
    text(demandMsg,375,375); //display demand
  }
  else if(pagestate == 33){
  image(Dia33,0,0);
  }
  else if(pagestate == 34){
  image(Dia34,0,0);
  }
  else if(pagestate == 35){
  image(Dia35,0,0);
  }
  else if(pagestate == 36){
  image(Dia36,0,0);
  }
  else if(pagestate == 37){
  image(Dia37,0,0);
  }
  
  
  //display round number
  if (pagestate >= 12 && pagestate <= 35){
    textSize(45);
    fill(255,255,255);
    textAlign(CENTER);
    text(roundNo+"/10", 105, 95);
  }
  
  //display score on score page
  if(pagestate == 36){
    textSize(50);
    fill(0);
    text("Your score ",350,600);
    text(trim(myScore),700,600);
    text("Opponent's score ",350,750);
    text(trim(oppScore),700,750);
  }
  //display name on name enter page
  if (pagestate == 2 || pagestate == 3){
    if(name!=null){
      print("Name: " + name);
      textAlign(CENTER);
      fill(0);
      textSize(30);
      text(name,400,770);
    }
  }
  //display name and score for each phase in phase color
  if(pagestate>=12 && pagestate <= 36){
    //supply phase color
    if(pagestate>= 12 && pagestate <= 17){    
        fill(SUPPLY_col.r,SUPPLY_col.g,SUPPLY_col.b);
    }
    //assembly phase color
    else if(pagestate>= 18 && pagestate <= 22){
        fill(ASSEMBLY_col.r,ASSEMBLY_col.g,ASSEMBLY_col.b);
    }
    //logistics phase color
    else if(pagestate>= 23 && pagestate <= 26){
      fill(LOGISTICS_col.r,LOGISTICS_col.g,LOGISTICS_col.b);
    }
    //transport phase color
    else if(pagestate>= 27 && pagestate <= 31){
      fill(TRANSPORT_col.r,TRANSPORT_col.g,TRANSPORT_col.b);
    }
    //demand phase color
    else if(pagestate>= 32 && pagestate <= 37){
        fill(DEMAND_col.r,DEMAND_col.g,DEMAND_col.b);
    }
    textAlign(CENTER);
    textSize(30);
    text(name,100,35); //display name
    textAlign(LEFT);
    String score = "Score: " + trim(myScore);
    text(score,618,35); //display score
  }
}
