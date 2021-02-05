import java.util.*;

//global variables
PriorityQueue<Node> highScoreList = new PriorityQueue<Node>(11, new ScoreComparator());
int MAX_HIGHSCORES = 1028;
String filePath = "Data/HighScores.txt";

public void initHighScore(){
  String[] pastScores = loadStrings(filePath);
  for(int i = 0; i < pastScores.length; i++){
    String[] tmp = split(pastScores[i],",");
    addHighScore(tmp[0],Integer.parseInt(tmp[1]));
  }
}

public void addHighScore(String name, int score){
  Node tmpNode = new Node(name, score);
  highScoreList.add(tmpNode);
}

public void saveHighScore(){
  String[] tmp = new String[MAX_HIGHSCORES];
  int i=0;
  Iterator value = highScoreList.iterator();
  while(value.hasNext()){
    Node tmpNode = (Node)value.next();
    tmp[i]+=tmpNode.getname();
    tmp[i]+=",";
    tmp[i]+=tmpNode.getscore();
  }
  saveStrings(filePath,tmp);
}

class Node{

    String name;        // the name
    int score;       // assuming you're using double

    public Node(String name, int score){
        this.name = name;
        this.score = score;         // assuming negative scores are allowed
    }
    
    public String getname(){
      return name;
    }
    
    public int getscore(){
      return score;
    }
}

class ScoreComparator implements Comparator<Node>{ 
// Overriding compare()method of Comparator for descending order of score
  public int compare(Node n1, Node n2) { 
    if (n1.score < n2.score){
      return 1;
    }
    else if (n1.score > n2.score){
      return -1; 
    }
    return 0; 
  } 
}
