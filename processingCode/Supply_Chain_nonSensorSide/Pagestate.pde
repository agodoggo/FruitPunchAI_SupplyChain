//git test
public void pagestate_change(int pagestate) {
  //clears out old frame
  background(0);
  //change background picture
  if(pagestate == 1){
  image(Dia1,0,0);
  }
  else if(pagestate == 2){
  image(Dia2,0,0);
  }
  if(pagestate == 3){
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
  image(Dia32,0,0);
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
    text(roundNo, 100, 75);
  }
  
  //display score
  if(pagestate == 36){
    fill(0);
    text("Your score ",350,600);
    text(myScore,500,600);
    text("Opponent's score ",350,750);
    text(oppScore,550,750);
  }
  //display name
  if (pagestate == 2 || pagestate == 3){
    if(name!=null){
      print("Name: " + name);
      textAlign(CENTER);
      fill(0);
      textSize(26);
      text(name,400,770);
    }
  }
}
