//git test
public void pagestate_change(int pagestate) {
  //clears out old frame
  background(0);
  //change background picture
  println("pagestate refreshed to " + pagestate);
  //instructions and setup
  if (pagestate == -1) {
    image(Instructions1, 0, 0);
  } else if (pagestate == -2) {
    image(Setup1, 0, 0);
  } else if (pagestate == -3) {
    image(Instructions2, 0, 0);
  } else if (pagestate == -4) {
    image(Setup2, 0, 0);
  } else if (pagestate == -5) {
    image(Instructions3, 0, 0);
  } else if (pagestate == -6) {
    image(Setup3, 0, 0);
  } else if (pagestate == -7) {
    image(Instructions4, 0, 0);
  } else if (pagestate == -8) {
    image(Setup4, 0, 0);
  } else if (pagestate == -9) {
    image(Instructions5, 0, 0);
  } else if (pagestate == -10) {
    image(Setup5, 0, 0);
  } else if (pagestate == -11) {
    image(Instructions6, 0, 0);
  } else if (pagestate == -12) {
    image(Setup6, 0, 0);
  } else if (pagestate == -13) {
    image(Instructions7, 0, 0);
  } else if (pagestate == -14) {
    image(Setup7, 0, 0);
  } else if (pagestate == -15) {
    image(Instructions8, 0, 0);
  } else if (pagestate == -16) {
    image(Setup8, 0, 0);
  } else if (pagestate == 0) {
    image(Dia0, 0, 0);
    displayHighScores();
  } else if (pagestate == 1) {
    image(Dia1, 0, 0);
  } else if (pagestate == 2) {
    image(Dia2, 0, 0);
  } else if (pagestate == 3) {
    if (InstructionsSeen && SetupSeen) {
      image(Dia3_Clickable, 0, 0);
    } else {
      image(Dia3_Hidden, 0, 0);
    }
  } else if (pagestate == 12) {
    image(Dia12, 0, 0);
  } else if (pagestate == 13) {
    image(Dia13, 0, 0);
  } else if (pagestate == 14) {
    image(Dia14, 0, 0);
  } else if (pagestate == 15) {
    image(Dia15, 0, 0);
  } else if (pagestate == 16) {
    image(Dia16, 0, 0);
  } else if (pagestate == 17) {
    image(Dia17, 0, 0);
  } else if (pagestate == 18) {
    image(Dia18, 0, 0);
  } else if (pagestate == 19) {
    image(Dia19, 0, 0);
  } else if (pagestate == 20) {
    image(Dia20, 0, 0);
  } else if (pagestate == 21) {
    image(Dia21, 0, 0);
  } else if (pagestate == 22) {
    image(Dia22, 0, 0);
  } else if (pagestate == 23) {
    image(Dia23, 0, 0);
  } else if (pagestate == 24) {
    image(Dia24, 0, 0);
  } else if (pagestate == 25) {
    image(Dia25, 0, 0);
  } else if (pagestate == 26) {
    image(Dia26, 0, 0);
  } else if (pagestate == 27) {
    image(Dia27, 0, 0);
  } else if (pagestate == 28) {
    image(Dia28, 0, 0);
  } else if (pagestate == 29) {
    image(Dia29, 0, 0);
  } else if (pagestate == 30) {
    image(Dia30, 0, 0);
    textAlign(CENTER);
    fill(255, 255, 255);
    textSize(40);
    text(demandMsg, 370, 430); //display demand
  } else if (pagestate == 31) {
    image(Dia31, 0, 0);
  } else if (pagestate == 32) {
    image(Dia32, 0, 0);
  } else if (pagestate == 33) {
    image(Dia33, 0, 0);
    textAlign(CENTER);
    fill(255, 255, 255);
    textSize(40);
    text(demandMsg, 370, 430); //display demand
  } else if (pagestate == 34) {
    image(Dia34, 0, 0);
  } else if (pagestate == 35) {
    image(Dia35, 0, 0);
  } else if (pagestate == 36) {
    image(Dia36, 0, 0);
  }


  //display round number
  if (pagestate >= 12 && pagestate <= 33) {
    textSize(45);
    fill(255, 255, 255);
    textAlign(CENTER);
    text(roundNo+"/10", 105, 95);
  }

  //display score on score page
  if (pagestate == 34) {
    textAlign(LEFT);
    textSize(50);
    fill(0);
    text("Your score ", 250, 600);
    text(trim(myScore), 700, 600);
    text("Opponent's score ", 250, 750);
    text(trim(oppScore), 700, 750);
    textSize(45);
    fill(255, 255, 255);
    textAlign(CENTER);
    text(roundNo+"/10", 205, 450);
  }
  //display name on name enter page
  if (pagestate == 1 || pagestate == 2) {
    if (name!=null) {
      print("Name: " + name);
      textAlign(CENTER);
      fill(0);
      textSize(30);
      text(name, 400, 780);
    }
  }
  //display name and score for each phase in phase color
  if (pagestate>=12 && pagestate <= 36 && pagestate != 35) {
    //supply phase color
    if (pagestate>= 12 && pagestate <= 16) {    
      fill(SUPPLY_col.r, SUPPLY_col.g, SUPPLY_col.b);
    }
    //assembly phase color
    else if (pagestate>= 17 && pagestate <= 20) {
      fill(ASSEMBLY_col.r, ASSEMBLY_col.g, ASSEMBLY_col.b);
    }
    //logistics phase color
    else if (pagestate>= 21 && pagestate <= 23) {
      fill(LOGISTICS_col.r, LOGISTICS_col.g, LOGISTICS_col.b);
    }
    //transport phase color
    else if (pagestate>= 24 && pagestate <= 28) {
      fill(TRANSPORT_col.r, TRANSPORT_col.g, TRANSPORT_col.b);
    }
    //demand phase color
    else if (pagestate>= 29 && pagestate <= 36 && pagestate != 35) {
      fill(DEMAND_col.r, DEMAND_col.g, DEMAND_col.b);
    }
    textAlign(CENTER);
    textSize(30);
    text(name, 100, 35); //display name
    textAlign(LEFT);
    String score = "Score: " + trim(myScore);
    text(score, 618, 35); //display score
  }

  //display AI advice for each dia
  //edit this to fulfill tasks
  if (pagestate == 12 || pagestate == 16 || pagestate == 17 || pagestate == 20 || pagestate == 21 || pagestate ==23) {
    print(rec);
    String text="";
    textSize(40);
    fill(0,0,0);
    if (pagestate == 12 || pagestate == 16) { //images are 210 and 100
        textSize(37);
        text("Produce", 230, 764);
        textSize(40);
        text(rec[0] + "x ",20,855);
        image(TabletCasing,80,780);
        text(rec[1] + "x ",270,855);
        image(Electronics,330,780);
        text(rec[2] + "x ",520,855);
        image(GameComputerCasing,580,780);
        text(rec[3] + "x ",20,1020);
        image(AssembledTablet,80,940);
        text(rec[4] + "x ",270,1020);
        image(AssembledGameComputer,330,940);
    } else if (pagestate == 17 || pagestate ==20) {
        textSize(37);
        text("Assemble", 230, 764);
        textSize(40);
        text(rec[5] + "x ",20,1020);
        image(AssembledTablet,80,940);
        text(rec[6] + "x ",270,1020);
        image(AssembledGameComputer,330,940);
      // insert pictures here next to text
    } else if (pagestate == 21 || pagestate == 23) {
        textSize(37);
        text("Move", 230, 850);
        textSize(40);
        text(rec[7] + "x ",20,960);
        image(AssembledTablet,80,880);
      // insert pictures here next to text
    }
    textAlign(LEFT);
    text(text, 80, 850); //display name
  }
}
